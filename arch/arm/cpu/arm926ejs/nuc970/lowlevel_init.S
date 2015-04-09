/*
 *  lowlevel_init - Low level init code for NUC970 MCUs
 *
 *  Copyright (c) 2015 Nuvoton Technology Corp.
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

#include <config.h>

#define AIC_MDCR        0xB8002138   // AIC Mask Disable Command Register
#define AIC_MDCRH       0xB800213C   // AIC Mask Disable Command(High) Register


.globl  lowlevel_init
lowlevel_init:
                // Disable Interrupt, This is for safe ...
                ldr     r0, =AIC_MDCR
                ldr     r1, =0xFFFFFFFF
                str     r1, [r0]
                ldr     r0, =AIC_MDCRH
                str     r1, [r0]
                mrs     r0, CPSR
                orr     r0, r0, #0xC0
                msr     CPSR_c, r0

                mov     pc, lr

