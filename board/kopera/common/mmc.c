/*
 * Copyright (C) 2016 Freescale Semiconductor, Inc.
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <asm/arch/sys_proto.h>
#include <linux/errno.h>
#include <asm/io.h>
#include <stdbool.h>
#include <mmc.h>

static int check_mmc_autodetect(void)
{
	return env_get_yesno("mmcautodetect") == 1 ? 1 : 0;
}

/* This should be defined for each board */
__weak int mmc_map_to_kernel_blk(int devno)
{
	return devno;
}

void board_late_mmc_env_init(void)
{
	char cmd[32];
	char mmcroot[32];
	u32 devno;
	ulong mmcpart;
	ulong mmcrootdev;

	if (!check_mmc_autodetect())
		return;

	devno = mmc_get_env_dev();
	mmcpart = env_get_ulong("mmcpart", 10, 1);

	env_set_ulong("mmcdev", devno);

	/* Set mmcroot env */
	mmcrootdev = mmc_map_to_kernel_blk(devno);

	env_set_ulong("mmcrootdev", mmcrootdev);
	sprintf(mmcroot, "/dev/mmcblk%up%lu", mmcrootdev, mmcpart);
	env_set("mmcroot", mmcroot);

	sprintf(cmd, "mmc dev %u", devno);
	run_command(cmd, 0);
}