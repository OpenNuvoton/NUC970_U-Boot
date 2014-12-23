/*
 * Copyright © 2014 Nuvoton technology corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation;version 2 of the License.
 *
 */

#include <common.h>

#include <asm/io.h>
#include "../../../board/nuvoton/nuc970evb/register.h"

extern   void sysprintf(char* pcStr,...);
#define printf(fmt, arg...) sysprintf(fmt, ##arg) //CWWeng add

extern void nuc970_serial_initialize(void);
extern int nuc970_serial_init (void);

void board_init_f(unsigned long bootflag)
{

    //mem_malloc_init (0xa00000, 0x100000);

    writel(readl(REG_PCLKEN0) | 0x10000, REG_PCLKEN0);   // UART clk
    writel(readl(REG_PCLKEN0) | 0x100, REG_PCLKEN0);   // Timer clk

    nuc970_serial_initialize();
    nuc970_serial_init();

    printf("nand_boot\n");

    nand_boot();
    //relocate_code(CONFIG_SYS_TEXT_BASE - TOTAL_MALLOC_LEN, NULL,
                        //CONFIG_SYS_TEXT_BASE);
}

