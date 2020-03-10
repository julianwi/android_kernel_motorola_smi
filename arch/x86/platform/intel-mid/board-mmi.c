/*
 * board-mmi.c: Motorola Medfield-based devices
 *
 * Copyright 2012 Motorola Mobility, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sfi.h>
#include <linux/input/touch_platform.h>

#include <asm/intel-mid.h>

/*
 * IPC devices
 */
#include "device_libs/platform_ipc.h"
#include "device_libs/platform_msic_power_btn.h"
#include "device_libs/platform_msic_battery.h"
#include "device_libs/platform_msic_adc.h"
#include "device_libs/platform_msic_audio.h"

/*
 * I2C devices
 */
#include "device_libs/platform_atmxt-224s.h"
#include "device_libs/platform_max17042.h"
#include "device_libs/platform_mt9e013.h"
#include "device_libs/platform_sh833su.h"
#include "device_libs/platform_ov7736.h"

/*
 * WIFI devices
 */
#include "device_libs/platform_wl12xx.h"

/*
 * HSI devices
 */
#include "device_libs/platform_hsi_modem.h"

/*
 * Miscellaneous devices
 */
#include "device_libs/platform_mmi-sensors.h"
#include "device_libs/platform_camera.h"

static void __init *no_platform_data(void *info)
{
	return NULL;
}

const struct intel_v4l2_subdev_id v4l2_ids[] = {
	{"mt9e013", RAW_CAMERA, ATOMISP_CAMERA_PORT_PRIMARY},
	{"lc898211", CAMERA_MOTOR, ATOMISP_CAMERA_PORT_PRIMARY},
	{"ov7736", SOC_CAMERA, ATOMISP_CAMERA_PORT_SECONDARY},
	{"lm3556", LED_FLASH, -1},
	{},
};

const struct devs_id __initconst device_ids[] = {
	{"msic_power_btn", SFI_DEV_TYPE_IPC, 1, &msic_power_btn_platform_data,
					&ipc_device_handler},
	{ATMXT_I2C_NAME, SFI_DEV_TYPE_I2C, 0, &mot_setup_touch_atmxt,
					NULL},
	{"max17042", SFI_DEV_TYPE_I2C, 1, &max17042_platform_data, NULL},
	{"msic_adc", SFI_DEV_TYPE_IPC, 1, &msic_adc_platform_data,
					&ipc_device_handler},
	{"msic_battery", SFI_DEV_TYPE_IPC, 1, &msic_battery_platform_data,
					&ipc_device_handler},
	{"wl12xx_clk_vmmc", SFI_DEV_TYPE_SD, 0, &wl12xx_platform_data, NULL},
	{"msic_audio", SFI_DEV_TYPE_IPC, 1, &msic_audio_platform_data,
					&ipc_device_handler},
	{"hsi_ifx_modem", SFI_DEV_TYPE_HSI, 0, &hsi_modem_platform_data, NULL},
	/*
	 * I2C devices for camera image subsystem which will not be load into
	 * I2C core while initialize
	 */
	{"mt9e013", SFI_DEV_TYPE_I2C, 0, &mt9e013_platform_data,
					&intel_ignore_i2c_device_register},
	{"lc898211", SFI_DEV_TYPE_I2C, 0, &sh833su_platform_data,
					&intel_ignore_i2c_device_register},
	{"ov7736", SFI_DEV_TYPE_I2C, 0, &ov7736_platform_data,
					&intel_ignore_i2c_device_register},
	{"lm3556", SFI_DEV_TYPE_I2C, 0, &no_platform_data,
					&intel_ignore_i2c_device_register},
	{"ov7736", SFI_DEV_TYPE_I2C, 0, &ov7736_platform_data,
					&intel_ignore_i2c_device_register},
	{"lm3556", SFI_DEV_TYPE_I2C, 0, &no_platform_data,
					&intel_ignore_i2c_device_register},
	{},
};

static int __init mmi_platform_init(void)
{
	mmi_register_board_i2c_devs();
	mmi_sensors_init();
	return 0;
}
device_initcall(mmi_platform_init);
