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

#define REG_SDCONF0     0xB0001008   // SDCONF0
#define REG_SDCONF1     0xB000100C   // SDCONF1
#define REG_HCLKEN      0xB0000210
#define REG_CLKSEL      0xB0000204
#define REG_CLKDIV      0xB0000208
#define REG_CLKDIVCTL8  0xB0000240
#define REG_PCLKEN0     0xB0000218
#define REG_CLKEN1      0xB0000224
#define REG_MFSEL       0xB000000C
#define REG_MFP_GPA_L	0xB0000070
#define REG_MFP_GPA_H	0xB0000074
#define REG_MFP_GPE_L	0xB0000090
#define REG_MFP_GPE_H	0xB0000094
#define REG_MFP_GPF_L	0xB0000098
#define REG_MFP_GPF_H	0xB000009C

#define REG_GPIOD_DIR           0xB8003014
#define REG_GPIOD_DATAOUT       0xB8003018


DECLARE_GLOBAL_DATA_PTR;

int board_early_init_f (void)
{

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
	//set to 16 Mbyte directly, not to read register
        //gd->bd->bi_dram[0].size = sdram_size(readl(REG_SDCONF0));
        gd->bd->bi_dram[0].size = 0x800000;
        gd->bd->bi_dram[0].start = gd->bd->bi_dram[0].size == 0 ? 0 : (readl(REG_SDCONF0) & 0xFFF80000) >> 1;

	//set to 16 Mbyte directly, not to read register
        //gd->bd->bi_dram[1].size = sdram_size(readl(REG_SDCONF1));
        gd->bd->bi_dram[1].size = 0x800000;
        gd->bd->bi_dram[1].start = gd->bd->bi_dram[1].size == 0 ? 0 : (readl(REG_SDCONF1) & 0xFFF80000) >> 1;

}

int dram_init(void)
{
	//set to 16 Mbyte directly, not to read register
        //gd->ram_size = sdram_size(readl(REG_SDCONF0)) + sdram_size(readl(REG_SDCONF1));
        gd->ram_size = 0x1000000;

        return(0);
}

#ifdef CONFIG_NUC970_ETH
extern int nuc970_eth_register(void);

int board_eth_init(bd_t *bis)
{
        writel(readl(REG_HCLKEN) | 0x10000, REG_HCLKEN);   // EMAC0 clk
        writel(readl(REG_HCLKEN) | 0x20000, REG_HCLKEN);   // EMAC1 clk

        writel(readl(REG_CLKDIVCTL8) | 0x10, REG_CLKDIVCTL8); //MII management interface clock 

	//Init multi-function pin for RMII
#ifdef CONFIG_NUC970_EMAC0
//	writel(0x11111111, REG_MFP_GPA_L);	// pin A0~A7 for RMII0
//	writel((readl(REG_MFP_GPA_H) & ~0xff) | 0x11, REG_MFP_GPA_H);// pin A8~A9 for RMII0
	writel(0x11111111, REG_MFP_GPF_L);	// pin F0~F7 for RMII0
	writel((readl(REG_MFP_GPF_H) & ~0xff) | 0x11, REG_MFP_GPF_H);// pin F8~F9 for RMII0
#else //CONFIG_NUC970_EMAC1
	writel((readl(REG_MFP_GPE_L) & ~0xffffff00) | 0x11111100, REG_MFP_GPE_L);// pin E2~E7 for RMII1
	writel((readl(REG_MFP_GPE_H) & ~0xffff) | 0x1111, REG_MFP_GPE_H);// pin E8~E11 for RMII1
#endif

        return(nuc970_eth_register());
}

#endif

#ifdef CONFIG_NUC970_MMC
int board_mmc_init(bd_t *bd)
{
        writel(readl(REG_CLKEN) | 0x30, REG_CLKEN);   // FMI & DMAC clk
        writel(readl(REG_CLKEN1) | 0x2, REG_CLKEN1);  // SD clk
        writel(readl(REG_CLKSEL) | 0x1000, REG_CLKSEL);  // SD clk
        writel((readl(REG_MFSEL) & ~0xF0) | 0xA0, REG_MFSEL);    // SD pins   
        writel(readl(REG_GPIOD_DIR) | 0x100, REG_GPIOD_DIR);     // GPIOD8 outoput
        writel(readl(REG_GPIOD_DATAOUT) & ~0x100, REG_GPIOD_DATAOUT);  // Turn on SD power
        
        return(nuc900_mmc_init());
        
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
        //icache_enable();
        //dcache_enable();
        
        return;
}       


int board_init(void)
{
        gd->bd->bi_boot_params = 0x100;

        writel(readl(REG_PCLKEN0) | 0x10000, REG_PCLKEN0);   // UART clk
        writel(readl(REG_PCLKEN0) | 0x100, REG_PCLKEN0);   // Timer clk

        return(0);
}
