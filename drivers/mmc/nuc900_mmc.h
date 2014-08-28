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


#ifndef _NUC900_MMC_
#define _NUC900_MMC_

#define DMAC_BA   0xB000C000 /* DMA Control */
#define FMI_BA    0xB000D000 /* Flash Memory Card Interface */

/* DMAC Control Registers */
#define REG_DMACCSR             (DMAC_BA+0x800)  /* DMAC Control and Status Register */
#define DMACEN           0x1
#define DMAC_SW_RST      0x2
//#define REG_DMACSAR1            (DMAC_BA+0x804)  /* DMAC Transfer Starting Address Register 1 */
#define REG_DMACSAR2            (DMAC_BA+0x808)  /* DMAC Transfer Starting Address Register 2 */
#define REG_DMACBCR             (DMAC_BA+0x80C)  /* DMAC Transfer Byte Count Register */
#define REG_DMACIER             (DMAC_BA+0x810)  /* DMAC Interrupt Enable Register */
#define REG_DMACISR             (DMAC_BA+0x814)  /* DMAC Interrupt Status Register */

/* Flash Memory Card Interface Registers */
#define REG_FMICSR              (FMI_BA+0x000)   /* Global Control and Status Register */
#define FMI_SW_RST      0x1
#define SD_EN           0x2

#define REG_FMIIER              (FMI_BA+0x004)   /* Global Interrupt Control Register */
#define REG_FMIISR              (FMI_BA+0x008)   /* Global Interrupt Status Register */

/* Secure Digit Registers */
#define REG_SDCSR               (FMI_BA+0x020)   /* SD control and status register */
#define CO_EN           0x01
#define RI_EN           0x02
#define DI_EN           0x04
#define DO_EN           0x08
#define R2_EN           0x10
#define CLK74_OE        0x20
#define CLK8_OE         0x40
#define CLK_KEEP0       0x80
#define DBW             0x8000
#define REG_SDARG               (FMI_BA+0x024)   /* SD command argument register */
#define REG_SDIER               (FMI_BA+0x028)   /* SD interrupt enable register */
#define REG_SDISR               (FMI_BA+0x02C)   /* SD interrupt status register */
#define BLKD_IF         0x01
#define CRC_IF          0x02
#define CRC_7           0x04
#define CRC_16          0x08
#define SDDAT0          0x80
#define RITO_IF         0x1000
#define REG_SDRSP0              (FMI_BA+0x030)   /* SD receive response token register 0 */
#define REG_SDRSP1              (FMI_BA+0x034)   /* SD receive response token register 1 */
#define REG_SDBLEN              (FMI_BA+0x038)   /* SD block length register */
#define REG_SDTMOUT             (FMI_BA+0x03C)   /* SD Response/Data-in timeout register */

#define REG_CLKDIV1             0xB0000228

#define MMC_CLK                 100000000  /* PLL clk / 2*/

// copy from linux/mmc/core.h
#define mmc_resp_type(cmd)      ((cmd)->resp_type & (MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC|MMC_RSP_BUSY|MMC_RSP_OPCODE))

#endif