/*
 *  Copyright (c) 2014 Nuvoton Technology Corp.
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
#include <common.h>
#include <asm/io.h>
#include "../../../drivers/gpio/nuc970_gpio.h"

#define REG_SDIC_SIZE0  0xB0001810   
#define REG_SDIC_SIZE1  0xB0001814  
#define REG_HCLKEN      0xB0000210
#define REG_CLKDIVCTL8  0xB0000240
#define REG_PCLKEN0     0xB0000218
#define REG_WDT_CTL	0xB8001800
#define REG_MFP_GPA_L	0xB0000070
#define REG_MFP_GPA_H	0xB0000074
#define REG_MFP_GPC_L	0xB0000080
#define REG_MFP_GPD_L	0xB0000088
#define REG_MFP_GPE_L	0xB0000090
#define REG_MFP_GPE_H	0xB0000094
#define REG_MFP_GPF_L	0xB0000098
#define REG_MFP_GPF_H	0xB000009C
#define REG_MFP_GPH_L	0xB00000A8
#define REG_MFP_GPH_H	0xB00000AC
#define REG_MFP_GPI_L	0xB00000B0
#define REG_MFP_GPI_H	0xB00000B4

int NUC970_cleanup(void);
extern int spi_flash_reset(void);

DECLARE_GLOBAL_DATA_PTR;

int board_early_init_f (void)
{
    writel(readl(REG_PCLKEN0) | 0x10000, REG_PCLKEN0);   // UART clk
    writel(readl(REG_PCLKEN0) | 0x100, REG_PCLKEN0);   // Timer clk
#ifdef CONFIG_NUC970_GPIO
    writel(readl(REG_PCLKEN0) | 0x8, REG_PCLKEN0);   // GPIO clk
#endif

    return 0;
}


static unsigned int sdram_size(unsigned int config)
{
    unsigned int size = 0;

    config &= 0x7;

    switch (config) {
    case 0:
        size = 0;
        break;
    case 1:
        size = 0x200000;
        break;
    case 2:
        size = 0x400000;
        break;
    case 3:
        size = 0x800000;
        break;
    case 4:
        size = 0x1000000;
        break;
    case 5:
        size = 0x2000000;
        break;
    case 6:
        size = 0x4000000;
        break;
    case 7:
        size = 0x8000000;
        break;
    }

    return(size);
}

void dram_init_banksize(void)
{
    gd->bd->bi_dram[0].size = sdram_size(readl(REG_SDIC_SIZE0));
    gd->bd->bi_dram[0].start = gd->bd->bi_dram[0].size == 0 ? 0 : (readl(REG_SDIC_SIZE0) & 0x1FE00000);

    gd->bd->bi_dram[1].size = sdram_size(readl(REG_SDIC_SIZE1));
    gd->bd->bi_dram[1].start = gd->bd->bi_dram[1].size == 0 ? 0 : (readl(REG_SDIC_SIZE1) & 0x1FE00000);
}

int dram_init(void)
{
    gd->ram_size = sdram_size(readl(REG_SDIC_SIZE0)) + sdram_size(readl(REG_SDIC_SIZE1));

    return(0);
}

#ifdef CONFIG_NUC970_ETH
extern int nuc970_eth_register(void);

int board_eth_init(bd_t *bis)
{
#ifdef CONFIG_NUC970_EMAC0
    writel(readl(REG_HCLKEN) | 0x10000, REG_HCLKEN);   // EMAC0 clk
    writel(readl(REG_CLKDIVCTL8) | 0x10, REG_CLKDIVCTL8); //MII management interface clock
    //Init multi-function pin for RMII
    writel(0x11111111, REG_MFP_GPF_L);	// pin F0~F7 for RMII0
    writel((readl(REG_MFP_GPF_H) & ~0xff) | 0x11, REG_MFP_GPF_H);// pin F8~F9 for RMII0
#else //CONFIG_NUC970_EMAC1
    writel(readl(REG_HCLKEN) | 0x20000, REG_HCLKEN);   // EMAC1 clk
    writel(readl(REG_CLKDIVCTL8) | 0x10, REG_CLKDIVCTL8); //MII management interface clock
    //Init multi-function pin for RMII
    writel((readl(REG_MFP_GPE_L) & ~0xffffff00) | 0x11111100, REG_MFP_GPE_L);// pin E2~E7 for RMII1
    writel((readl(REG_MFP_GPE_H) & ~0xffff) | 0x1111, REG_MFP_GPE_H);// pin E8~E11 for RMII1
#endif

    return(nuc970_eth_register());
}

#endif

#ifdef CONFIG_NUC970_MMC
#define REG_PWRON       0xB0000004
#define REG_CLKDIVCTL3  0xB000022C
#define REG_CLKDIVCTL9  0xB0000244
#define FMI_BA    	0xB000D000 /* Flash Memory Card Interface */
#define SDH_BA          0xB000C000 /* SD Host */
#define REG_SDH_GCTL    (SDH_BA+0x800)   /* Global Control and Status Register */
#define REG_ECTL        (SDH_BA+0x840)   /* SD Host extend control register */
#define REG_FMICTL      (FMI_BA+0x800)  /* FMI control and status register */
#define GCTL_RST        0x1
#define SD_EN           0x2
#define EMMC_EN         0x2

int board_mmc_init(bd_t *bd)
{

    volatile int i;

#if defined(CONFIG_NUC970_SD_PORT0) || defined(CONFIG_NUC970_SD_PORT1)
    writel(readl(REG_HCLKEN) | 0x40000000, REG_HCLKEN);   // SDH clk
    writel((readl(REG_CLKDIVCTL9) & ~0x18), REG_CLKDIVCTL9); //Set SDH clock source from XIN
    writel((readl(REG_CLKDIVCTL9) & ~0xFF00) | (0x1d << 8), REG_CLKDIVCTL9); //Set SDH clock divider => 400 KHz

    writel(GCTL_RST, REG_SDH_GCTL);
    for(i = 0; i < 10; i++);        // Need few clock delay 'til SW_RST auto cleared.
    writel(SD_EN, REG_SDH_GCTL);

    writel(readl(REG_ECTL) & ~3, REG_ECTL); // SD port 0,1 power enable
#endif

#ifdef CONFIG_NUC970_SD_PORT0
    writel(0x66666666, REG_MFP_GPD_L);   // Set GPD for SD0

    nuc970_mmc_init(0); // init for SD port 0
#endif
#ifdef CONFIG_NUC970_SD_PORT1
    writel((readl(REG_MFP_GPH_L) & ~0xff000000) | 0x66000000, REG_MFP_GPH_L); // pin H6~H7 for SD1
    writel((readl(REG_MFP_GPH_H) & ~0x00ffffff) | 0x666666, REG_MFP_GPH_H); // pin H8~H13 for SD1

    nuc970_mmc_init(1); // init for SD port 1
#endif

#ifdef CONFIG_NUC970_EMMC
#ifdef CONFIG_NAND_NUC970
# error Don't enable eMMC(CONFIG_NUC970_EMMC) and NAND(CONFIG_NAND_NUC970) at the same time!
#endif

    writel(readl(REG_HCLKEN) | 0x00700000, REG_HCLKEN);   // eMMC & NAND & FMI clk
    writel((readl(REG_CLKDIVCTL3) & ~0x18), REG_CLKDIVCTL3); //Set eMMC clock source from XIN
    writel((readl(REG_CLKDIVCTL3) & ~0xFF00) | (0x28 << 8), REG_CLKDIVCTL3); //Set eMMC clock divider => 300 KHz

    // Set MFP the same port as NAND according to power-on setting register
    if (readl(REG_PWRON) & 0x08000000)
    {
        writel((readl(REG_MFP_GPI_L) & ~0xfff00000) | 0x66600000, REG_MFP_GPI_L);   // pin I5~7 for eMMC
        writel((readl(REG_MFP_GPI_H) & ~0x00000fff) | 0x00000666, REG_MFP_GPI_H);   // pin I8~10 for eMMC
    }
    else
    {
        writel((readl(REG_MFP_GPC_L) & ~0x00ffffff) | 0x00666666, REG_MFP_GPC_L);   // pin C0~5 for eMMC
    }

    nuc970_mmc_init(2); // init for eMMC
#endif

    return 0;

}
#endif

#ifdef CONFIG_SYS_CONSOLE_OVERWRITE_ROUTINE
int overwrite_console(void)
{

    return 1;  // force serial interface as stdin/stdout/stderr even lcd interface enabled
}
#endif

void enable_caches(void)
{
#ifndef CONFIG_SYS_ICACHE_OFF
    icache_enable();
#endif
#ifndef CONFIG_SYS_DCACHE_OFF
    dcache_enable();
#endif

    return;
}


int board_init(void)
{
    gd->bd->bi_boot_params = 0x100;

    return(0);
}

int board_late_init(void)
{

#ifdef CONFIG_HW_WATCHDOG
    hw_watchdog_init();
#endif

    return 0;
}

int NUC970_cleanup(void)
{

#ifdef CONFIG_SYS_USE_SPIFLASH
    spi_flash_reset();
#endif

    return 0;
}

