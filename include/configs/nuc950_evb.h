/*
 * (C) Copyright 2012
 * Nuvoton Technology Corp. <www.nuvoton.com>
 *
 * Configuation settings for the NUC950 EV Board.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H


#define CONFIG_SYS_TEXT_BASE		0xA0000000      	/* NOR boot */
//#define CONFIG_SYS_TEXT_BASE		0x8000      	/* RAM boot */
//#define CONFIG_SYS_TEXT_BASE            0x200         /* SPI/NAND boot */
#define CONFIG_SYS_LOAD_ADDR            0x8000
#define CONFIG_EXT_CLK	                15000000        /* 15 MHz crystal */
#define CONFIG_TMR_DIV                  150             /* timer prescaler */  
#define CONFIG_SYS_HZ			1000
#define CONFIG_SYS_MEMTEST_START         0xA00000
#define CONFIG_SYS_MEMTEST_END           0xB00000

#define CONFIG_ARCH_CPU_INIT
#undef  CONFIG_USE_IRQ               

#define CONFIG_CMDLINE_TAG	1	/* enable passing of ATAGs	*/
#define CONFIG_SETUP_MEMORY_TAGS 1
#define CONFIG_INITRD_TAG	1

#define CONFIG_SYS_USE_NORFLASH
//#define CONFIG_SYS_USE_SPIFLASH

#define CONFIG_BOARD_EARLY_INIT_F

#define CONFIG_DISPLAY_CPUINFO

#define CONFIG_BOOTDELAY	3

#define CONFIG_SYS_SDRAM_BASE   0
#define CONFIG_NR_DRAM_BANKS    2     // there are 2 sdram banks for nuc900
#define CONFIG_SYS_INIT_SP_ADDR 0xB000C700

#define CONFIG_BAUDRATE         115200
#define CONFIG_SYS_BAUDRATE_TABLE       {115200, 57600, 38400}

#define CONFIG_NUC900_ETH
#define CONFIG_NUC900_PHY_ADDR          1
#define CONFIG_ETHADDR                  00:00:00:11:66:88
#define CONFIG_SYS_RX_ETH_BUFFER        16 // default is 4, set to 16 here.
#define CONFIG_NUC900_CONSOLE

//#undef CONFIG_SYS_ICACHE_OFF
//#undef CONFIG_SYS_DCACHE_OFF
#define CONFIG_SYS_ICACHE_OFF
#define CONFIG_SYS_DCACHE_OFF

/*
 * BOOTP options
 */
#define CONFIG_BOOTP_BOOTFILESIZE	1
#define CONFIG_BOOTP_BOOTPATH		1
#define CONFIG_BOOTP_GATEWAY		1
#define CONFIG_BOOTP_HOSTNAME		1

/*
 * Command line configuration.
 */
#include <config_cmd_default.h>

#undef CONFIG_CMD_LOADS
#undef CONFIG_CMD_SOURCE

#define CONFIG_CMD_PING		1
#define CONFIG_CMD_DHCP		1
//#define CONFIG_CMD_NAND		1
//#define CONFIG_CMD_USB		1
#define CONFIG_CMD_JFFS2        1


#ifdef CONFIG_SYS_USE_SPIFLASH
#undef CONFIG_CMD_IMLS  //====================> SPI only
#undef CONFIG_CMD_JFFS2
#endif

/* SPI flash test code */
#ifdef CONFIG_SYS_USE_SPIFLASH
#define CONFIG_SYS_NO_FLASH     1       //------------>
//#define CONFIG_SYS_MAX_FLASH_SECT               256  //------------>
//#define CONFIG_SYS_MAX_FLASH_BANKS              1    //------------>
#define CONFIG_NUC900_SPI       1
#define CONFIG_CMD_SPI          1
#define CONFIG_CMD_SF           1
#define CONFIG_SPI              1
#define CONFIG_SPI_FLASH        1
#define CONFIG_SPI_FLASH_MACRONIX   1
#define CONFIG_ENV_IS_IN_SPI_FLASH  1
#define CONFIG_ENV_OFFSET       0x30000
#define CONFIG_ENV_SIZE         0x10000
#define CONFIG_ENV_SECT_SIZE    0x10000
#define CONFIG_ENV_OVERWRITE
#endif

/* NOR flash, if populated */
#ifdef CONFIG_SYS_USE_NORFLASH
#define CONFIG_SYS_FLASH_CFI			1
#define CONFIG_FLASH_CFI_DRIVER			1
#define PHYS_FLASH_1				0xA0000000
#define CONFIG_SYS_FLASH_BASE			PHYS_FLASH_1
#define CONFIG_SYS_MAX_FLASH_SECT		256
#define CONFIG_SYS_MAX_FLASH_BANKS		1
#define CONFIG_SYS_MONITOR_BASE	CONFIG_SYS_FLASH_BASE
#define CONFIG_SYS_MONITOR_LEN	(256 << 10)
#define CONFIG_ENV_IS_IN_FLASH	1
#define CONFIG_ENV_ADDR		(CONFIG_SYS_FLASH_BASE + 0x60000)
/* Address and size of Primary Environment Sector */
#define CONFIG_ENV_SIZE		0x10000

#define CONFIG_ENV_OVERWRITE
#endif //CONFIG_SYS_USE_NORFLASH


#define CONFIG_SYS_PROMPT		"U-Boot> "
#define CONFIG_SYS_CBSIZE		256
#define CONFIG_SYS_MAXARGS		16
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_LONGHELP		1
#define CONFIG_CMDLINE_EDITING		1
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "

/* Following block is for LCD support */
#if 0
#define CONFIG_LCD
#define CONFIG_NUC900_LCD
#define LCD_BPP                         LCD_COLOR16
#define CONFIG_LCD_LOGO
#define CONFIG_LCD_INFO
#define CONFIG_LCD_INFO_BELOW_LOGO
#define CONFIG_SYS_CONSOLE_IS_IN_ENV
#define CONFIG_SYS_CONSOLE_OVERWRITE_ROUTINE
#endif

/* Following block is for MMC support */
#if 0
#define CONFIG_NUC900_MMC
#define CONFIG_CMD_MMC
#define CONFIG_CMD_FAT
#define CONFIG_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_DOS_PARTITION
//#define CONFIG_MMC_TRACE
#endif

/* Following block is for EHCI support*/
#if 0
#define CONFIG_CMD_USB
#define CONFIG_CMD_FAT
#define CONFIG_USB_STORAGE
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_NUC900
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_DOS_PARTITION
#endif

/*
 * Size of malloc() pool
 */
#define CONFIG_SYS_MALLOC_LEN	ROUND(3 * CONFIG_ENV_SIZE + 128*1024, 0x1000)

#define CONFIG_STACKSIZE	(32*1024)	/* regular stack */

#endif
