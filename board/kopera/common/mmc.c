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
	return env_get_yesno("mmcautodetect") == 1;
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
	ulong mmcroot_blk;
	ulong mmcroot_part;

	if (!check_mmc_autodetect())
		return;

	devno = mmc_get_env_dev();
	mmcpart = env_get_ulong("mmcpart", 10, 1);

	env_set_ulong("mmcdev", devno);

	/* Set mmcroot env */
	mmcroot_blk = mmc_map_to_kernel_blk(devno);
	mmcroot_part = mmcpart;

	env_set_ulong("mmcroot_blk", mmcroot_blk);
	env_set_ulong("mmcroot_part", mmcroot_part);
	sprintf(mmcroot, "/dev/mmcblk%up%lu", mmcroot_blk, mmcroot_part);
	env_set("mmcroot", mmcroot);

	sprintf(cmd, "mmc dev %u", devno);
	run_command(cmd, 0);
}
