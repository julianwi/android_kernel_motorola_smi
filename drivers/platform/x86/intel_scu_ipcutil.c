/*
 * intel_scu_ipc.c: Driver for the Intel SCU IPC mechanism
 *
 * (C) Copyright 2008-2010 Intel Corporation
 * Author: Sreedhara DS (sreedhara.ds@intel.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 *
 * This driver provides ioctl interfaces to call intel scu ipc driver api
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <asm/intel_scu_ipc.h>
#include <asm/intel_scu_ipcutil.h>

/* Mode for Audio clock */
static DEFINE_MUTEX(osc_clk0_lock);
static unsigned int osc_clk0_mode;

int intel_scu_ipc_osc_clk(u8 clk, unsigned int khz)
{
	/* SCU IPC COMMAND(osc clk on/off) definition:
	 * ipc_wbuf[0] = clock to act on {0, 1, 2, 3}
	 * ipc_wbuf[1] =
	 * bit 0 - 1:on  0:off
	 * bit 1 - if 1, read divider setting from bits 3:2 as follows:
	 * bit [3:2] - 00: clk/1, 01: clk/2, 10: clk/4, 11: reserved
	 */
	unsigned int base_freq;
	unsigned int div;
	u8 ipc_wbuf[2];
	int ipc_ret;

	if (clk > 3)
		return -EINVAL;

	ipc_wbuf[0] = clk;
	ipc_wbuf[1] = 0;
	if (khz) {
#ifdef CONFIG_CTP_CRYSTAL_38M4
		base_freq = 38400;
#else
		base_freq = 19200;
#endif
		div = fls(base_freq / khz) - 1;
		if (div >= 3 || (1 << div) * khz != base_freq)
			return -EINVAL;	/* Allow only exact frequencies */
		ipc_wbuf[1] = 0x03 | (div << 2);
	}

	ipc_ret = intel_scu_ipc_command(IPCMSG_OSC_CLK, 0,
					ipc_wbuf, 2, NULL, 0);
	if (ipc_ret != 0)
		pr_err("%s: failed to set osc clk(%d) output\n", __func__, clk);

	return ipc_ret;
}
EXPORT_SYMBOL_GPL(intel_scu_ipc_osc_clk);

/*
 * OSC_CLK_AUDIO is connected to the MSIC as well as Audience, so it should be
 * turned on if any one of them requests it to be on and it should be turned off
 * only if no one needs it on.
 */
int intel_scu_ipc_set_osc_clk0(unsigned int enable, enum clk0_mode mode)
{
	int ret = 0, clk_enable;
	static const unsigned int clk_khz = 19200;

	pr_debug("set_clk0 request %s for Mode 0x%x\n",
				enable ? "ON" : "OFF", mode);
	mutex_lock(&osc_clk0_lock);
	if (mode == CLK0_QUERY) {
		ret = osc_clk0_mode;
		goto out;
	}
	if (enable) {
		/* if clock is already on, just add new user */
		if (osc_clk0_mode) {
			osc_clk0_mode |= mode;
			goto out;
		}
		osc_clk0_mode |= mode;
		pr_debug("set_clk0: enabling clk, mode 0x%x\n", osc_clk0_mode);
		clk_enable = 1;
	} else {
		osc_clk0_mode &= ~mode;
		pr_debug("set_clk0: disabling clk, mode 0x%x\n", osc_clk0_mode);
		/* others using the clock, cannot turn it of */
		if (osc_clk0_mode)
			goto out;
		clk_enable = 0;
	}
	pr_debug("configuring OSC_CLK_AUDIO now\n");
	ret = intel_scu_ipc_osc_clk(OSC_CLK_AUDIO, clk_enable ? clk_khz : 0);
out:
	mutex_unlock(&osc_clk0_lock);
	return ret;
}
EXPORT_SYMBOL_GPL(intel_scu_ipc_set_osc_clk0);

#define MSIC_VPROG1_CTRL        0xD6
#define MSIC_VPROG2_CTRL        0xD7
#define MSIC_VPROG_ON           0xFF
#define MSIC_VPROG_OFF          0

int intel_scu_ipc_msic_vprog1(int on)
{
	return intel_scu_ipc_iowrite8(MSIC_VPROG1_CTRL,
			on ? MSIC_VPROG_ON : MSIC_VPROG_OFF);
}
EXPORT_SYMBOL_GPL(intel_scu_ipc_msic_vprog1);

static int major;

/* ioctl commnds */
#define	INTE_SCU_IPC_REGISTER_READ	0
#define INTE_SCU_IPC_REGISTER_WRITE	1
#define INTE_SCU_IPC_REGISTER_UPDATE	2

/**
 *	scu_reg_access		-	implement register access ioctls
 *	@cmd: command we are doing (read/write/update)
 *	@data: kernel copy of ioctl data
 *
 *	Allow the user to perform register accesses on the SCU via the
 *	kernel interface
 */

static int scu_reg_access(u32 cmd, struct scu_ipc_data  *data)
{
	int count = data->count;

	if (count == 0 || count == 3 || count > 4)
		return -EINVAL;

	switch (cmd) {
	case INTE_SCU_IPC_REGISTER_READ:
		return intel_scu_ipc_readv(data->addr, data->data, count);
	case INTE_SCU_IPC_REGISTER_WRITE:
		return intel_scu_ipc_writev(data->addr, data->data, count);
	case INTE_SCU_IPC_REGISTER_UPDATE:
		return intel_scu_ipc_update_register(data->addr[0],
						    data->data[0], data->mask);
	default:
		return -ENOTTY;
	}
}

/**
 *	scu_ipc_ioctl		-	control ioctls for the SCU
 *	@fp: file handle of the SCU device
 *	@cmd: ioctl coce
 *	@arg: pointer to user passed structure
 *
 *	Support the I/O and firmware flashing interfaces of the SCU
 */
static long scu_ipc_ioctl(struct file *fp, unsigned int cmd,
							unsigned long arg)
{
	int ret;
	struct scu_ipc_data  data;
	void __user *argp = (void __user *)arg;

	if (!capable(CAP_SYS_RAWIO))
		return -EPERM;

	if (copy_from_user(&data, argp, sizeof(struct scu_ipc_data)))
		return -EFAULT;
	ret = scu_reg_access(cmd, &data);
	if (ret < 0)
		return ret;
	if (copy_to_user(argp, &data, sizeof(struct scu_ipc_data)))
		return -EFAULT;
	return 0;
}

static const struct file_operations scu_ipc_fops = {
	.unlocked_ioctl = scu_ipc_ioctl,
};

static int __init ipc_module_init(void)
{
	major = register_chrdev(0, "intel_mid_scu", &scu_ipc_fops);
	if (major < 0)
		return major;

	return 0;
}

static void __exit ipc_module_exit(void)
{
	unregister_chrdev(major, "intel_mid_scu");
}

module_init(ipc_module_init);
module_exit(ipc_module_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Utility driver for intel scu ipc");
MODULE_AUTHOR("Sreedhara <sreedhara.ds@intel.com>");
