/*
 * Copyright 2017 NXP
 * Copyright 2018-2019 Variscite Ltd.
 * Copyright 2019 KOPERA
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __MX8M_KOPERA_POLAR0_H
#define __MX8M_KOPERA_POLAR0_H

#include <linux/sizes.h>
#include <asm/arch/imx-regs.h>

#ifdef CONFIG_SECURE_BOOT
#define CONFIG_CSF_SIZE			0x2000 /* 8K region */
#endif

#define CONFIG_SPL_TEXT_BASE		0x7E1000
#define CONFIG_SPL_MAX_SIZE		(148 * 1024)
#define CONFIG_SYS_MONITOR_LEN		(512 * 1024)
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_USE_SECTOR
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR	0x300
#define CONFIG_SYS_MMCSD_FS_BOOT_PARTITION	1

#ifdef CONFIG_SPL_BUILD
#define CONFIG_SPL_WATCHDOG_SUPPORT
#define CONFIG_SPL_DRIVERS_MISC_SUPPORT
#define CONFIG_SPL_POWER_SUPPORT
#define CONFIG_SPL_I2C_SUPPORT
#define CONFIG_SPL_LDSCRIPT		"arch/arm/cpu/armv8/u-boot-spl.lds"
#define CONFIG_SPL_STACK		0x187FF0
#define CONFIG_SPL_LIBCOMMON_SUPPORT
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SPL_GPIO_SUPPORT
#define CONFIG_SPL_MMC_SUPPORT
#define CONFIG_SPL_BSS_START_ADDR      0x00180000
#define CONFIG_SPL_BSS_MAX_SIZE        0x2000	/* 8 KB */
#define CONFIG_SYS_SPL_MALLOC_START    0x42200000
#define CONFIG_SYS_SPL_MALLOC_SIZE     0x80000	/* 512 KB */
#define CONFIG_SYS_SPL_PTE_RAM_BASE    0x41580000
#define CONFIG_SYS_ICACHE_OFF
#define CONFIG_SYS_DCACHE_OFF

#define CONFIG_MALLOC_F_ADDR		0x182000 /* malloc f used before GD_FLG_FULL_MALLOC_INIT set */

#define CONFIG_SPL_ABORT_ON_RAW_IMAGE /* For RAW image gives a error info not panic */

#undef CONFIG_DM_MMC

#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG

#define CONFIG_POWER
#define CONFIG_POWER_I2C
#define CONFIG_POWER_PFUZE100
#define CONFIG_POWER_PFUZE100_I2C_ADDR 0x08
#endif

#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC_I2C1		/* enable I2C bus 1 */
#define CONFIG_SYS_I2C_MXC_I2C2		/* enable I2C bus 2 */
#define CONFIG_SYS_I2C_MXC_I2C3		/* enable I2C bus 3 */

#define CONFIG_REMAKE_ELF

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOARD_POSTCLK_INIT
#define CONFIG_BOARD_LATE_INIT

/* Flat Device Tree Definitions */
#define CONFIG_OF_BOARD_SETUP

#undef CONFIG_CMD_EXPORTENV
#undef CONFIG_CMD_IMPORTENV
#undef CONFIG_CMD_IMLS

#undef CONFIG_CMD_CRC32
#undef CONFIG_BOOTM_NETBSD

/* ENET Config */
/* ENET1 */
#if defined(CONFIG_CMD_NET)
#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_MII
#define CONFIG_MII
#define CONFIG_ETHPRIME                 "FEC"

#define CONFIG_FEC_MXC
#define CONFIG_FEC_XCV_TYPE             RGMII
#define CONFIG_FEC_MXC_PHYADDR          0
#define FEC_QUIRK_ENET_MAC

#define CONFIG_PHY_GIGE
#define IMX_FEC_BASE			0x30BE0000

#define CONFIG_PHYLIB
#define CONFIG_PHY_ATHEROS
#endif

#define CONFIG_SYS_BOOTM_LEN		0xF000000

/* Initial environment variables */
#define CONFIG_EXTRA_ENV_SETTINGS		\
	"bootdir=/boot\0" \
	"image=Image.gz\0" \
	"fdt_file=" CONFIG_DEFAULT_FDT_FILE "\0" \
	"console=ttymxc0,115200 earlycon=ec_imx6q,0x30860000,115200\0" \
	\
	"img_addr=0x42000000\0" \
	"fdt_addr=0x43000000\0" \
	"fdt_high=0xffffffffffffffff\0" \
	"initrd_addr=0x43800000\0" \
	"initrd_high=0xffffffffffffffff\0" \
	\
	"BOOT_ORDER=A B\0" \
	"BOOT_A_LEFT=3\0" \
	"BOOT_B_LEFT=3\0" \
	\
	"mmcdev="__stringify(CONFIG_SYS_MMC_ENV_DEV)"\0" \
	"mmcpart=" __stringify(CONFIG_SYS_MMC_IMG_LOAD_PART) "\0" \
	"mmcroot=" CONFIG_MMCROOT "\0" \
	"mmcloadimagecmd=load mmc ${mmcdev}:${mmcpart} ${img_addr} ${bootdir}/${image}; " \
		"unzip ${img_addr} ${loadaddr};\0" \
	"mmcloadfdtcmd=load mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${bootdir}/${fdt_file};\0" \
	"mmcselectpartcmd=setenv mmcpart; " \
		"for BOOT_SLOT in \"${BOOT_ORDER}\"; do " \
			"if test \"x${mmcpart}\" = \"x\"; then " \
				"if test \"x${BOOT_SLOT}\" = \"xA\"; then " \
					"if test ${BOOT_A_LEFT} -gt 0; then " \
						"setexpr BOOT_A_LEFT ${BOOT_A_LEFT} - 1; " \
						"echo \"Found valid slot A, ${BOOT_A_LEFT} attempts remaining\"; " \
						"setenv mmcpart 1; " \
					"fi; " \
				"elif test \"x${BOOT_SLOT}\" = \"xB\"; then " \
					"if test ${BOOT_B_LEFT} -gt 0; then " \
						"setexpr BOOT_B_LEFT ${BOOT_B_LEFT} - 1; " \
						"echo \"Found valid slot B, ${BOOT_B_LEFT} attempts remaining\"; " \
						"setenv mmcpart 2; " \
					"fi; " \
				"fi; " \
			"fi; " \
		"done; " \
		"if test -n \"${mmcpart}\"; then " \
			"setenv mmcroot /dev/mmcblk${mmcdev}p${mmcpart}; " \
			"saveenv; " \
		"else " \
			"echo \"No valid slot found, resetting tries to 3\"; " \
			"setenv BOOT_A_LEFT 3; " \
			"setenv BOOT_B_LEFT 3; " \
			"saveenv; " \
			"reset; " \
		"fi;\0" \
	"mmcbootcmd=if run mmcselectpartcmd && run mmcloadimagecmd && run mmcloadfdtcmd; then " \
		"setenv bootargs console=${console} root=${mmcroot} rootwait vt.global_cursor_default=0 panic=-1; " \
		"booti ${loadaddr} - ${fdt_addr}; " \
	"fi;\0" \
	"mmcrecoverycmd=echo \"Initialising recovery\"; " \
		"if usb reset && load usb 0 ${loadaddr} polarfw.bin; then " \
			"echo \"Firmware image size : 0x${filesize} bytes.\"; " \
			"echo \"Flashing image to mmc 0\"; " \
			"echo \"DO NOT REMOVE POWER OR RESET UNTIL THIS PROCESS IS FINISHED\"; " \
			"gzwrite mmc 0 ${loadaddr} ${filesize}; " \
			"reset; " \
		"else " \
			"echo \"Failed to load firmware file from USB 0\"; " \
		"fi;\0" \

#define CONFIG_BOOTCOMMAND \
	"run mmcbootcmd;"

/* Link Definitions */
#define CONFIG_LOADADDR			0x40480000
#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR

#define CONFIG_SYS_INIT_RAM_ADDR        0x40000000
#define CONFIG_SYS_INIT_RAM_SIZE        0x80000
#define CONFIG_SYS_INIT_SP_OFFSET \
        (CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
        (CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

#define CONFIG_ENV_OVERWRITE
#define CONFIG_ENV_OFFSET               (64 * SZ_64K)
#define CONFIG_ENV_SIZE			0x1000
#define CONFIG_SYS_MMC_ENV_DEV		0   /* eMMC */

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		((CONFIG_ENV_SIZE + (2*1024) + (16*1024)) * 1024)

#define CONFIG_SYS_SDRAM_BASE           0x40000000
#define PHYS_SDRAM                      0x40000000
#define PHYS_SDRAM_SIZE                 0xC0000000 /* 3GB DDR */
#define DEFAULT_DRAM_SIZE_MB            512
#define CONFIG_NR_DRAM_BANKS            1

#define CONFIG_SYS_MEMTEST_START	PHYS_SDRAM
#define CONFIG_SYS_MEMTEST_END 		(CONFIG_SYS_MEMTEST_START + (256 * SZ_1M))

#define CONFIG_BAUDRATE			115200

#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE		UART1_BASE_ADDR

/* Monitor Command Prompt */
#undef CONFIG_SYS_PROMPT
#define CONFIG_SYS_PROMPT		"u-boot=> "
#define CONFIG_SYS_PROMPT_HUSH_PS2     "> "
#define CONFIG_SYS_CBSIZE              2048
#define CONFIG_SYS_MAXARGS             64
#define CONFIG_SYS_BARGSIZE CONFIG_SYS_CBSIZE
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_IMX_BOOTAUX

#define CONFIG_CMD_MMC
#define CONFIG_FSL_ESDHC
#define CONFIG_FSL_USDHC

#define CONFIG_SYS_FSL_USDHC_NUM	2
#define CONFIG_SYS_FSL_ESDHC_ADDR       0

#define CONFIG_SUPPORT_EMMC_BOOT	/* eMMC specific */
#define CONFIG_MMCROOT			"/dev/mmcblk0p1"  /* eMMC : partition 1 */
#define CONFIG_SYS_MMC_IMG_LOAD_PART	1

#define CONFIG_MXC_GPIO

#define CONFIG_MXC_OCOTP
#define CONFIG_CMD_FUSE

/* I2C Configs */
#define CONFIG_SYS_I2C_SPEED		  100000

/* USB configs */
#ifndef CONFIG_SPL_BUILD
#define CONFIG_CMD_USB
#define CONFIG_USB_STORAGE
#define CONFIG_CMD_READ
#endif

#define CONFIG_SERIAL_TAG
#define CONFIG_FASTBOOT_USB_DEV 0

#define CONFIG_USB_MAX_CONTROLLER_COUNT         2

#define CONFIG_USBD_HS
#define CONFIG_USB_GADGET_VBUS_DRAW 2

#define CONFIG_OF_SYSTEM_SETUP

/* Framebuffer */
#ifdef CONFIG_VIDEO
#define CONFIG_VIDEO_IMXDCSS
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_SPLASH_SCREEN
#define CONFIG_BMP_16BPP
#define CONFIG_VIDEO_LOGO
#define CONFIG_VIDEO_BMP_LOGO
#define CONFIG_IMX_VIDEO_SKIP
#endif

/* Splash Screen  */
#ifdef CONFIG_SPLASH_SCREEN
#define CONFIG_CMD_BMP
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_SPLASH_SOURCE
#endif

#endif
