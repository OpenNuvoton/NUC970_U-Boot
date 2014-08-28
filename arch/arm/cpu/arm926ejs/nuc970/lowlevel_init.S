/*
 *  lowlevel_init - Low level init code for NUC900 MCUs
 *
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/* Set up PLL, detect SDRAM, remmap */

#include <config.h>

#define ROM_Start       0xA0000000   // Base address of ROM after remapping
#define GTEMP1          0xB0000034   // general temp register
#define CLK_Sel         0xB0000204
#define CLK_Div         0xB0000208
#define PLL_Con0        0xB000020C
#define Clk_Skew        0xB0001700   // Clock skew control register
#define EBI_Ctrl        0xB0001000   // EBI Controle register
#define ROM_Cfg         0xB0001004   // Boot ROM config register
#define SDRAM_config0   0xB0001008   // SDCONF0
#define AIC_MDCR        0xB8002124   // AIC Mask Disable Control Register



.globl  lowlevel_init
lowlevel_init:
                // Disable Interrupt, This is for safe ...
                ldr     r0, =AIC_MDCR
                ldr     r1, =0xFFFFFFFF
                str     r1, [r0]
                mrs     r0, CPSR
                orr     r0, r0, #0xC0
                msr     CPSR_c, r0

                // Adjust boot rom access timing and size  //TODO: get config data for setting
                ldr     r0, =ROM_Cfg
                ldr     r1, =0x40040084
                str     r1, [r0]

                // Let's speed up. Enable PLL.
                ldr     r0, =CLK_Sel
                ldr     r1, =0xFFF
                str     r1, [r0]        // select external clk as CPU clock
                ldr     r0, =CLK_Div
                ldr     r1, =0x05000000
                str     r1, [r0]        // CPU:AHB:APB = 1:2:4
                ldr     r0, =PLL_Con0
                ldr     r1, =0x4F24
                str     r1, [r0]        // 200 MHz

                /* let PLL stable */
                mov     r2, #0x3000
1:
                subs    r2, r2, #0x1
                bne     1b

                ldr     r0, =CLK_Sel
                ldr     r1, =0xFFC
                str     r1, [r0]        // select PLL as CPU clock

                /* another small delay here */
                mov     r2, #0x400
2:
                subs    r2, r2, #0x1
                bne     2b

                mov     pc, lr




