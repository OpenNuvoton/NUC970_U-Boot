/*
 *  Copyright (c) 2012 Nuvoton Technology Corp.
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
#define REG_CLKEN       0xB0000200
#define REG_CLKSEL      0xB0000204
#define REG_CLKDIV      0xB0000208
#define REG_CLKEN1      0xB0000224
#define REG_MFSEL       0xB000000C

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
        gd->bd->bi_dram[0].size = sdram_size(readl(REG_SDCONF0));
        gd->bd->bi_dram[0].start = gd->bd->bi_dram[0].size == 0 ? 0 : (readl(REG_SDCONF0) & 0xFFF80000) >> 1;

        gd->bd->bi_dram[1].size = sdram_size(readl(REG_SDCONF1));
        gd->bd->bi_dram[1].start = gd->bd->bi_dram[1].size == 0 ? 0 : (readl(REG_SDCONF1) & 0xFFF80000) >> 1;

}

int dram_init(void)
{
        gd->ram_size = sdram_size(readl(REG_SDCONF0)) + sdram_size(readl(REG_SDCONF1));

        return(0);
}

#ifdef CONFIG_NUC900_ETH
extern int nuc900_eth_register(void);

int board_eth_init(bd_t *bis)
{
        writel(readl(REG_CLKEN) | 0x80, REG_CLKEN);   // MAC clk
        writel(readl(REG_CLKEN1) | 0x4, REG_CLKEN1);  // RMII clk
        writel(readl(REG_MFSEL) | 0x2, REG_MFSEL);    // MAC pins

        return(nuc900_eth_register());
}

#endif

#ifdef CONFIG_NUC900_MMC
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

        return(0);
}