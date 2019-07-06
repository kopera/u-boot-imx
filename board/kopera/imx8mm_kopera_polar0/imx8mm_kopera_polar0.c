/*
 * Copyright 2018 NXP
 * Copyright 2019 Variscite Ltd.
 * Copyright 2019 KOPERA
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <miiphy.h>
#include <asm/mach-imx/iomux-v3.h>
#include <asm-generic/gpio.h>
#include <asm/arch/imx8mm_pins.h>
#include <asm/arch/sys_proto.h>
#include <asm/mach-imx/gpio.h>
#include <asm/mach-imx/mxc_i2c.h>
#include <asm/arch/clock.h>
#include <usb.h>

#include "../common/variscite/imx8m_eeprom.h"

DECLARE_GLOBAL_DATA_PTR;

int board_early_init_f(void)
{
	return 0;
}

#ifdef CONFIG_BOARD_POSTCLK_INIT
int board_postclk_init(void)
{
	/* TODO */
	return 0;
}
#endif

/* Return DRAM size in bytes */
static unsigned long long get_dram_size(void)
{
	int ret;
	u32 dram_size_mb;
	struct var_eeprom eeprom;

	var_eeprom_read_header(&eeprom);
	ret = var_eeprom_get_dram_size(&eeprom, &dram_size_mb);
	if (ret)
		return (1ULL * DEFAULT_DRAM_SIZE_MB ) << 20;

	return (1ULL * dram_size_mb) << 20;
}

int dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = CONFIG_SYS_SDRAM_BASE;
	gd->bd->bi_dram[0].size = get_dram_size();

	return 0;
}

int dram_init(void)
{
	unsigned long long mem_size, max_low_size, dram_size;

	max_low_size = 0x100000000ULL - CONFIG_SYS_SDRAM_BASE;
	dram_size = get_dram_size();
	
	if (dram_size > max_low_size)
		mem_size = max_low_size;
	else
		mem_size = dram_size;

	/* rom_pointer[1] contains the size of TEE occupies */
	gd->ram_size = mem_size - rom_pointer[1];

	return 0;
}

#ifdef CONFIG_OF_BOARD_SETUP
int ft_board_setup(void *blob, bd_t *bd)
{
	return 0;
}
#endif

#define VOLUME_UP_KEY		IMX_GPIO_NR(3, 25)
#define VOLUME_UP_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_HYS | PAD_CTL_PUE)

static iomux_v3_cfg_t const volume_up_pads[] = {
	IMX8MM_PAD_SAI5_RXD3_GPIO3_IO24 | MUX_PAD_CTRL(VOLUME_UP_PAD_CTRL),
};

static int board_gpio_init(void)
{
	/* We are only interested in the '+' key i.e. volume up key.
		We use it as a signal to check for recovery mode
	*/
	imx_iomux_v3_setup_multiple_pads(volume_up_pads, ARRAY_SIZE(volume_up_pads));
	gpio_request(VOLUME_UP_KEY, "VOLUME_UP");
	gpio_direction_input(VOLUME_UP_KEY);

	return 0;
}

#ifdef CONFIG_CI_UDC
int board_usb_init(int index, enum usb_init_type init)
{
	imx8m_usb_power(index, true);

	return 0;
}

int board_usb_cleanup(int index, enum usb_init_type init)
{
	imx8m_usb_power(index, false);

	return 0;
}
#endif

int board_init(void)
{
	board_gpio_init();

	return 0;
}

int board_mmc_get_env_dev(int devno)
{
	return devno - 1;
}

int mmc_map_to_kernel_blk(int devno)
{
	return devno + 1;
}

int board_late_init(void)
{
	env_set("board_name", "imx8mm-kopera-polar0");
	env_set("board_rev", "0");

#ifdef CONFIG_ENV_IS_IN_MMC
	board_late_mmc_env_init();
#endif

	if (gpio_get_value(VOLUME_UP_KEY) == 1) {
		printf("Recovery mode enabled\n");
		env_set("bootcmd", "run mmcrecoverycmd");
	} else {
		env_set("bootcmd", "run mmcbootcmd");
	}

	return 0;
}
