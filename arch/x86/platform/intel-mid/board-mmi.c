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

/*
 * I2C devices
 */
#include "device_libs/platform_atmxt-224s.h"
#include "device_libs/platform_max17042.h"

/*
 * WIFI devices
 */
#include "device_libs/platform_wl12xx.h"

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
	{},
};
