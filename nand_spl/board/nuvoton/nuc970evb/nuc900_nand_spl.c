/*
 * Copyright ? 2009 Nuvoton technology corporation.
 *
 * Tanshi Li <tanshi_li@163.com>
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

#include <common.h>

#include <nand.h>
#include <asm/arch/nuc900_cpu.h>
#include <asm/arch/nuc900_nand.h>
#include <asm/io.h>
#include <asm/errno.h>
#include <malloc.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#define RESET_FMI	0x01
#define NAND_EN		0x08
#define READYBUSY	(0x01 << 18)
//add by tanshi 2012.10.08
#define NANDREADYBUSYBIT 0x400
#ifndef CONFIG_SYS_NAND_RESET_CNT
#define CONFIG_SYS_NAND_RESET_CNT 200000
#endif
//#define DEBUG_NAND 0
#define ENDADDR		(0x01 << 31)
#define page2k 1
//end add tanshi

#define SWRST		0x01
#define PSIZE		(0x01 << 3)
#define DMARWEN		(0x03 << 1)
#define BUSWID		(0x01 << 4)
#define ECC4EN		(0x01 << 5)
#define WP		(0x01 << 24)
#define NANDCS		(0x01 << 25)
#define ENDADDR		(0x01 << 31)

#define NUC900_SMCSR_CS        (0x03<<25)

#ifdef CONFIG_NAND_SPL
#define MAX_CHIPS	2
typedef char *            PINT8;
   static int nand_cs[MAX_CHIPS] = {0, 1};
   unsigned char vaddr[CONFIG_SYS_NAND_PAGE_SIZE+CONFIG_SYS_NAND_OOBSIZE];
extern   void sysprintf(PINT8 pcStr,...);
#define printf(fmt, arg...) sysprintf(fmt, ##arg)
//#define printf(arg...) do {} while (0)
#else
	 unsigned char * vaddr;
#endif

/* functions */
int fmiSMCheckRB(void)
{
        while (1) {
                if (readl(REG_SMISR) & 0x400) {
                        writel(0x400,REG_SMISR);
                        return 1;
                }
        }
        return 0;
}

// SM functions
int fmiSM_Reset(void)
{

        u32 volatile i;

        writel(0xff,REG_SMCMD);
        for (i=100; i>0; i--);

        if (!fmiSMCheckRB())
                return -1;
        return 0;
}

int fmiSMCorrectData_2K(u32 uDAddr)
{

        u32 volatile byte, correct_addr;
        unsigned int volatile reg, data, errCount;

        reg = nuc900_nand_read(REG_SMECC4ST);

        if ((reg & 0x02) || (reg & 0x200) || (reg & 0x20000) || (reg & 0x2000000)) {
#if 0
                printk("uncorrectable!!\n");
#endif
                return FMI_SM_ECC_ERROR;
        } else if (reg & 0x01) {
                errCount = (nuc900_nand_read(REG_SMECC4ST) >> 2) & 0x7;
                correct_addr = uDAddr;
                switch (errCount) {
                case 1:
                        byte = nuc900_nand_read(REG_SMECC4F1A1) & 0x1ff;
                        data = nuc900_nand_read(REG_SMECC4F1D) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 1 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        break;

                case 2:
                        byte = nuc900_nand_read(REG_SMECC4F1A1) & 0x1ff;
                        data = nuc900_nand_read(REG_SMECC4F1D) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 1 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        byte = (nuc900_nand_read(REG_SMECC4F1A1) >> 16) & 0x1ff;
                        data = (nuc900_nand_read(REG_SMECC4F1D) >> 8) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 2 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        break;

                case 3:
                        byte = nuc900_nand_read(REG_SMECC4F1A1) & 0x1ff;
                        data = nuc900_nand_read(REG_SMECC4F1D) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 1 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        byte = (nuc900_nand_read(REG_SMECC4F1A1) >> 16) & 0x1ff;
                        data = (nuc900_nand_read(REG_SMECC4F1D) >> 8) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 2 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        byte = nuc900_nand_read(REG_SMECC4F1A2) & 0x1ff;
                        data = (nuc900_nand_read(REG_SMECC4F1D) >> 16) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 3 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        break;

                case 4:
                        byte = nuc900_nand_read(REG_SMECC4F1A1) & 0x1ff;
                        data = nuc900_nand_read(REG_SMECC4F1D) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 1 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        byte = (nuc900_nand_read(REG_SMECC4F1A1) >> 16) & 0x1ff;
                        data = (nuc900_nand_read(REG_SMECC4F1D) >> 8) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 2 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        byte = nuc900_nand_read(REG_SMECC4F1A2) & 0x1ff;
                        data = (nuc900_nand_read(REG_SMECC4F1D) >> 16) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 3 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        byte = (nuc900_nand_read(REG_SMECC4F1A2) >> 16) & 0x1ff;
                        data = (nuc900_nand_read(REG_SMECC4F1D) >> 24) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 4 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        break;
                }
        }
        if (reg & 0x100) {
                errCount = (nuc900_nand_read(REG_SMECC4ST) >> 10) & 0x7;
                correct_addr = uDAddr + 0x200;
                switch (errCount) {
                case 1:
                        byte = nuc900_nand_read(REG_SMECC4F2A1) & 0x1ff;
                        data = nuc900_nand_read(REG_SMECC4F2D) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 1 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        break;

                case 2:
                        byte = nuc900_nand_read(REG_SMECC4F2A1) & 0x1ff;
                        data = nuc900_nand_read(REG_SMECC4F2D) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 1 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        byte = (nuc900_nand_read(REG_SMECC4F2A1) >> 16) & 0x1ff;
                        data = (nuc900_nand_read(REG_SMECC4F2D) >> 8) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 2 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        break;

                case 3:
                        byte = nuc900_nand_read(REG_SMECC4F2A1) & 0x1ff;
                        data = nuc900_nand_read(REG_SMECC4F2D) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 1 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        byte = (nuc900_nand_read(REG_SMECC4F2A1) >> 16) & 0x1ff;
                        data = (nuc900_nand_read(REG_SMECC4F2D) >> 8) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 2 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        byte = nuc900_nand_read(REG_SMECC4F2A2) & 0x1ff;
                        data = (nuc900_nand_read(REG_SMECC4F2D) >> 16) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 3 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        break;

                case 4:
                        byte = nuc900_nand_read(REG_SMECC4F2A1) & 0x1ff;
                        data = nuc900_nand_read(REG_SMECC4F2D) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 1 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        byte = (nuc900_nand_read(REG_SMECC4F2A1) >> 16) & 0x1ff;
                        data = (nuc900_nand_read(REG_SMECC4F2D) >> 8) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 2 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        byte = nuc900_nand_read(REG_SMECC4F2A2) & 0x1ff;
                        data = (nuc900_nand_read(REG_SMECC4F2D) >> 16) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 3 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        byte = (nuc900_nand_read(REG_SMECC4F2A2) >> 16) & 0x1ff;
                        data = (nuc900_nand_read(REG_SMECC4F2D) >> 24) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 4 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        break;
                }
        }
        if (reg & 0x10000) {
                errCount = (nuc900_nand_read(REG_SMECC4ST) >> 18) & 0x7;
                correct_addr = uDAddr + 0x400;
                switch (errCount) {
                case 1:
                        byte = nuc900_nand_read(REG_SMECC4F3A1) & 0x1ff;
                        data = nuc900_nand_read(REG_SMECC4F3D) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 1 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        break;

                case 2:
                        byte = nuc900_nand_read(REG_SMECC4F3A1) & 0x1ff;
                        data = nuc900_nand_read(REG_SMECC4F3D) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 1 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        byte = (nuc900_nand_read(REG_SMECC4F3A1) >> 16) & 0x1ff;
                        data = (nuc900_nand_read(REG_SMECC4F3D) >> 8) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 2 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        break;

                case 3:
                        byte = nuc900_nand_read(REG_SMECC4F3A1) & 0x1ff;
                        data = nuc900_nand_read(REG_SMECC4F3D) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 1 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        byte = (nuc900_nand_read(REG_SMECC4F3A1) >> 16) & 0x1ff;
                        data = (nuc900_nand_read(REG_SMECC4F3D) >> 8) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 2 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        byte = nuc900_nand_read(REG_SMECC4F3A2) & 0x1ff;
                        data = (nuc900_nand_read(REG_SMECC4F3D) >> 16) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 3 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        break;

                case 4:
                        byte = nuc900_nand_read(REG_SMECC4F3A1) & 0x1ff;
                        data = nuc900_nand_read(REG_SMECC4F3D) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 1 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        byte = (nuc900_nand_read(REG_SMECC4F3A1) >> 16) & 0x1ff;
                        data = (nuc900_nand_read(REG_SMECC4F3D) >> 8) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 2 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        byte = nuc900_nand_read(REG_SMECC4F3A2) & 0x1ff;
                        data = (nuc900_nand_read(REG_SMECC4F3D) >> 16) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 3 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        byte = (nuc900_nand_read(REG_SMECC4F3A2) >> 16) & 0x1ff;
                        data = (nuc900_nand_read(REG_SMECC4F3D) >> 24) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 4 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        break;
                }
        }
        if (reg & 0x1000000) {
                errCount = (nuc900_nand_read(REG_SMECC4ST) >> 26) & 0x7;
                correct_addr = uDAddr + 0x600;
                switch (errCount) {
                case 1:
                        byte = nuc900_nand_read(REG_SMECC4F4A1) & 0x1ff;
                        data = nuc900_nand_read(REG_SMECC4F4D) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 1 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        break;

                case 2:

                        byte = nuc900_nand_read(REG_SMECC4F4A1) & 0x1ff;
                        data = nuc900_nand_read(REG_SMECC4F4D) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 1 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        byte = (nuc900_nand_read(REG_SMECC4F4A1) >> 16) & 0x1ff;
                        data = (nuc900_nand_read(REG_SMECC4F4D) >> 8) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 2 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        break;

                case 3:

                        byte = nuc900_nand_read(REG_SMECC4F4A1) & 0x1ff;
                        data = nuc900_nand_read(REG_SMECC4F4D) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 1 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        byte = (nuc900_nand_read(REG_SMECC4F4A1) >> 16) & 0x1ff;
                        data = (nuc900_nand_read(REG_SMECC4F4D) >> 8) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 2 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        byte = nuc900_nand_read(REG_SMECC4F4A2) & 0x1ff;
                        data = (nuc900_nand_read(REG_SMECC4F4D) >> 16) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 3 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        break;

                case 4:

                        byte = nuc900_nand_read(REG_SMECC4F4A1) & 0x1ff;
                        data = nuc900_nand_read(REG_SMECC4F4D) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 1 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        byte = (nuc900_nand_read(REG_SMECC4F4A1) >> 16) & 0x1ff;
                        data = (nuc900_nand_read(REG_SMECC4F4D) >> 8) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 2 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        byte = nuc900_nand_read(REG_SMECC4F4A2) & 0x1ff;
                        data = (nuc900_nand_read(REG_SMECC4F4D) >> 16) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 3 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        byte = (nuc900_nand_read(REG_SMECC4F4A2) >> 16) & 0x1ff;
                        data = (nuc900_nand_read(REG_SMECC4F4D) >> 24) & 0xff;
                        __raw_writeb(__raw_readb(correct_addr+byte)^(data & 0xff), correct_addr+byte);
#ifdef DEBUG
                        printk("correctable 4 byte!! correct_addr[%x], byte[%x], data[%x]\n", correct_addr, byte, data);
                        printk("correct data [%x]\n", __raw_readb(correct_addr+byte));
#endif
                        break;
                }
        }
        return 0;
}


int fmiHWInit(void)
{
	//add by tanshi 2012.10.08
	// enable FMI and DMAC
	writel(readl(REG_CLKEN) | 0x30,REG_CLKEN);
	writel(((readl(REG_MFSEL) & 0xFFFFFFF3) | 0x00000004),REG_MFSEL); /* select NAND function pins */
	// DMAC Initial
	writel( 0x00000003, REG_DMACCSR );
	writel( 0x00000001, REG_DMACCSR );
	writel( 0x00000001, REG_DMACIER );
	writel( 0x01, REG_FMICSR );		// reset FMI engine
	
	// enable SM
	writel( 0x08 , REG_FMICSR );
	
	/* init SM interface */
	writel( (readl(REG_SMCSR)&0xf8ffffc0)|0x01000028 , REG_SMCSR);//wp=1, ecc4_en=1,psize=1/2048+64
	
	fmiSM_Reset();
	return 0;
}

static void nuc900_hwcontrol(struct mtd_info *mtd, int cmd, unsigned int ctrl)//change by tanshi 2012.10.08
{
	struct nand_chip *chip = mtd->priv;
	
	if (ctrl & NAND_CTRL_CHANGE) {
		ulong IO_ADDR_W = (ulong)REG_SMDATA;

		if ((ctrl & NAND_CLE))
			IO_ADDR_W = REG_SMCMD;
		if ((ctrl & NAND_ALE))
			IO_ADDR_W = REG_SMADDR;

		chip->IO_ADDR_W = (void *)IO_ADDR_W;
	}
	if (cmd != NAND_CMD_NONE){
		writel(cmd, chip->IO_ADDR_W);
	}

}

/**
 * nand_command_lp - [DEFAULT] Send command to NAND large page device
 * @mtd:	MTD device structure
 * @command:	the command to be sent
 * @column:	the column address for this command, -1 if none
 * @page_addr:	the page address for this command, -1 if none
 *
 * Send command to NAND device. This is the version for the new large page
 * devices We dont have the separate regions as we have in the small page
 * devices.  We must emulate NAND_CMD_READOOB to keep the code compatible.
 */
static void nuc900_nand_command_lp(struct mtd_info *mtd, unsigned int command,
			    int column, int page_addr)
{
	register struct nand_chip *chip = mtd->priv;
	uint32_t rst_sts_cnt = CONFIG_SYS_NAND_RESET_CNT;
	printf("nuc900_nand_command:command=%x,column=%x,page_addr=%x\n",command,column,page_addr);
	/* clear R/B flag */
 // if(command == NAND_CMD_ERASE1){ 
   	  while( !(readl( REG_SMISR )& 0x40000)); 
      writel(0x400,REG_SMISR); 
  //}
	/* Emulate NAND_CMD_READOOB */
	if (command == NAND_CMD_READOOB) {
		column += mtd->writesize;
		command = NAND_CMD_READ0;
	}
  printf("nuc900_nand_command:column=%x\n",column);
	/* Command latch cycle */
	chip->cmd_ctrl(mtd, command & 0xff,
		       NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);

	if (column != -1 || page_addr != -1) {
		int ctrl = NAND_CTRL_CHANGE | NAND_NCE | NAND_ALE;

		/* Serially input address */
		if (column != -1) {
			/* Adjust columns for 16 bit buswidth */
			if (chip->options & NAND_BUSWIDTH_16)
				column >>= 1;
			chip->cmd_ctrl(mtd, column& 0xff, ctrl);
			ctrl &= ~NAND_CTRL_CHANGE;
			chip->cmd_ctrl(mtd, ((column >> 8)& 0xff) | ENDADDR , ctrl);
		}
		if (page_addr != -1) {
			chip->cmd_ctrl(mtd, page_addr, ctrl);

			/* One more address cycle for devices > 128MiB */
			if (chip->chipsize > (128 << 20)){
				chip->cmd_ctrl(mtd, (page_addr >> 8)& 0xff ,
				       NAND_NCE | NAND_ALE);
				chip->cmd_ctrl(mtd, ((page_addr >> 16)& 0xff) | ENDADDR,
					       NAND_NCE | NAND_ALE);
			}
			else
				chip->cmd_ctrl(mtd, ((page_addr >> 8)& 0xff)| ENDADDR ,
				       NAND_NCE | NAND_ALE);
		}
	}
	chip->cmd_ctrl(mtd, NAND_CMD_NONE, NAND_NCE | NAND_CTRL_CHANGE);

	/*
	 * program and erase have their own busy handlers
	 * status, sequential in, and deplete1 need no delay
	 */
	switch (command) {

	case NAND_CMD_CACHEDPROG:
	case NAND_CMD_PAGEPROG:
	case NAND_CMD_ERASE1:
	
	case NAND_CMD_SEQIN:
	case NAND_CMD_RNDIN:
	case NAND_CMD_STATUS:
	case NAND_CMD_DEPLETE1:
		return;
	case NAND_CMD_RESET:	

		if (chip->dev_ready)
			break;
		//udelay(chip->chip_delay);
		while (!chip->dev_ready(mtd));
		chip->cmd_ctrl(mtd, NAND_CMD_STATUS,
			       NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);
		chip->cmd_ctrl(mtd, NAND_CMD_NONE,
			       NAND_NCE | NAND_CTRL_CHANGE);
		while (!(chip->read_byte(mtd) & NAND_STATUS_READY) &&
			(rst_sts_cnt--));
		return;
	case NAND_CMD_ERASE2:	
	  	//ndelay(10);
			if (!fmiSMCheckRB()){
				printf("NAND_CMD_ERASE2 error!\n");

			}
			return ;	
			
			
		/*
		 * read error status commands require only a short delay
		 */
	case NAND_CMD_STATUS_ERROR:
	case NAND_CMD_STATUS_ERROR0:
	case NAND_CMD_STATUS_ERROR1:
	case NAND_CMD_STATUS_ERROR2:
	case NAND_CMD_STATUS_ERROR3:
		//udelay(chip->chip_delay);
		return;
	case NAND_CMD_RNDOUT:
		/* No ready / busy check necessary */
		chip->cmd_ctrl(mtd, NAND_CMD_RNDOUTSTART,
			       NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);
		chip->cmd_ctrl(mtd, NAND_CMD_NONE,
			       NAND_NCE | NAND_CTRL_CHANGE);
		//ndelay(10);       
     
		return;

	case NAND_CMD_READ0:
		chip->cmd_ctrl(mtd, NAND_CMD_READSTART,
			       NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);
		chip->cmd_ctrl(mtd, NAND_CMD_NONE,
			       NAND_NCE | NAND_CTRL_CHANGE);

		/* This applies to read commands */
	default:
		/*
		 * If we don't have access to the busy pin, we apply the given
		 * command delay
		 */
		if (!chip->dev_ready) {
			//udelay(chip->chip_delay);
			return;
		}
	}

	/* Apply this short delay always to ensure that we do wait tWB in
	 * any case on any machine. */
	//ndelay(100);
	while (!chip->dev_ready(mtd));
	//nand_wait_ready(mtd);
}

static int nuc900_dev_ready(struct mtd_info *mtd)//change by tanshi 2012.10.08
{
	if( readl( REG_SMISR )& (1<<18) )
		return 1;
	return 0;
}
static unsigned char nuc900_nand_read_byte(struct mtd_info *mtd)
{
	unsigned char ret;
	ret = readl(REG_SMDATA);
	return ret;
}

static void nuc900_nand_read_buf(struct mtd_info *mtd, u_char *buf, int len)
{
		int i;
	struct nand_chip *chip = mtd->priv;
	//printf("nuc900nand read buf:len=%d\n",len);

	if(len==mtd->oobsize){// read oob data
		chip->cmdfunc(mtd, NAND_CMD_RNDOUT, mtd->writesize, -1);
		
	  for (i = 0; i < len; i++)
		   buf[i] = readl(REG_SMDATA)& 0xff;
		return;
	}
	//normal page read use dma
	while (readl(REG_DMACCSR)&DMACCSR_FMI_BUSY); //Wait IP finished... for safe
	writel((unsigned long)vaddr,REG_DMACSAR2);
	writel(readl(REG_SMCSR) | 0x02,REG_SMCSR); //enable DMA read
	while(!(readl(REG_SMISR)&0x01));//wait for dma finished
	writel(readl(REG_SMISR)|0x01,REG_SMISR);  //clear DMA finished flag
	for (i = 0; i < len; i++)
	{
		buf[i]=vaddr[i];
	}
	
  return;
}
static void nuc900_nand_write_buf(struct mtd_info *mtd, const u_char *buf, int len)
{
		return;
}

static void nuc900_nand_select_chip(struct mtd_info *mtd, int chipnr)
{
		//struct nand_chip *chip = mtd->priv;

	switch (chipnr) {
	case -1://no chip selected 
		writel(readl(REG_SMCSR) | NUC900_SMCSR_CS,
			       REG_SMCSR);
		break;
	case 0://select nand chip 0
		 writel(readl(REG_SMCSR) & ~NUC900_SMCSR_CS,
			       REG_SMCSR);
		break;

	default:
#ifdef CONFIG_NAND_SPL
    break;
#else		
		BUG();
#endif		
		
	}
}

#ifdef CONFIG_NUC900_NAND_HWECC
/*
 * Enable HW ECC : unused on most chips
 */
void nuc900_nand_enable_hwecc(struct mtd_info *mtd, int mode)
{
	/*
	struct nand_chip *chip = mtd->priv;
	debug("nuc900_nand_enable_hwecc(%p, %d)\n", mtd, mode);
	
	*/
}

static int nuc900_nand_calculate_ecc(struct mtd_info *mtd, const u_char *dat,
				      u_char *ecc_code)
{
	struct nand_chip *chip = mtd->priv;
	unsigned int ecc_value;
	//printf("nuc900_nand_calculate_ecc\n");
	/* get the first 3 ECC bytes */
	ecc_value = readl(REG_SMECC0);
	ecc_code[0] = ecc_value & 0xFF;
	ecc_code[1] = (ecc_value >> 8) & 0xFF;
	ecc_code[2] = (ecc_value >> 16) & 0xFF;
	
	/* get the second 3 ECC bytes */
	ecc_value = readl(REG_SMECC1);

	ecc_code[3] = ecc_value & 0xFF;
	ecc_code[4] = (ecc_value >> 8) & 0xFF;
	ecc_code[5] = (ecc_value >> 16) & 0xFF;
	if (chip->ecc.bytes==12)
	{
		/* get the third 3 ECC bytes */
	ecc_value = readl(REG_SMECC2);
	ecc_code[6] = ecc_value & 0xFF;
	ecc_code[7] = (ecc_value >> 8) & 0xFF;
	ecc_code[8] = (ecc_value >> 16) & 0xFF;
	
	/* get the fourth 3 ECC bytes */
	ecc_value = readl(REG_SMECC3);

	ecc_code[9] = ecc_value & 0xFF;
	ecc_code[10] = (ecc_value >> 8) & 0xFF;
	ecc_code[11] = (ecc_value >> 16) & 0xFF;	
	}     	       	       
	return 0;
}

/*
 * HW ECC Correction
 *
 * function called after a read
 *
 * mtd:        MTD block structure
 * dat:        raw data read from the chip
 * read_ecc:   ECC from the chip (unused)
 * isnull:     unused
 *
 * Detect and correct a 1 bit error for a page
 */
static int nuc900_nand_correct_data(struct mtd_info *mtd, u_char *dat,
				     u_char *read_ecc, u_char *calc_ecc)
{
	//struct nand_chip *nand_chip = mtd->priv;
	unsigned int ecc_status;
	
	/* get the status from the Status Register */
	ecc_status = readl(REG_SMISR);

	/* if there's no error */
	if (likely(!(ecc_status & 0x02)))
		return 0;
		
  writel(0x02,REG_SMISR);
  
  if (fmiSMCorrectData_2K((unsigned long)dat)==0){
  		printk(KERN_WARNING "nuvoton_nand : error corrected\n");
	    return 1;
   }
  /* 
	* We can't correct so many errors */
	printk(KERN_WARNING "nuvoton_nand : multiple errors detected."
				" Unable to correct.\n");
	return -EIO;	
}
#endif
int board_nand_init(struct nand_chip *nand)
{
	debug("board_nand_init()\n");
	printf("board_nand_init()\n");
	fmiHWInit();
	printf("board_nand_init() 2\n");
//add end
	/* initialize nand_chip data structure */
	nand->IO_ADDR_R = (void *)REG_SMDATA;
	nand->IO_ADDR_W = (void *)REG_SMDATA;
	
	printf("board_nand_init() 3\n");
	/* read_buf and write_buf are default */
	/* read_byte and write_byte are default */
#ifdef CONFIG_NAND_SPL
	nand->read_buf = nand_read_buf;
#endif

	printf("board_nand_init() 4\n");
	/* hwcontrol always must be implemented */
	nand->cmd_ctrl = nuc900_hwcontrol;
	nand->cmdfunc = nuc900_nand_command_lp;
	nand->dev_ready = nuc900_dev_ready;
	nand->select_chip = nuc900_nand_select_chip;
  
	nand->read_byte	 = nuc900_nand_read_byte;
	nand->write_buf	= nuc900_nand_write_buf;
	nand->read_buf	= nuc900_nand_read_buf;
//nand->verify_buf	= nuc900_verify_buf;
	//nand->chip_delay	= 50;
	
#ifdef CONFIG_NUC900_NAND_HWECC

	nand->ecc.mode = NAND_ECC_HW;
	nand->ecc.calculate = nuc900_nand_calculate_ecc;
	nand->ecc.correct = nuc900_nand_correct_data;
	nand->ecc.hwctl = nuc900_nand_enable_hwecc;
  nand->ecc.bytes = CONFIG_SYS_NAND_ECCBYTES;	
	static int chip_n;
	if (chip_n >= MAX_CHIPS)
		return -ENODEV;
  nand->ecc.size=CONFIG_SYS_NAND_PAGE_SIZE;
  writel( (readl(REG_SMCSR)&0xfffffff0)|0x00000008 , REG_SMCSR );	// psize:2048; wp# set 1
  nand->priv		= nand_cs + chip_n++;

#else
	nand->ecc.mode = NAND_ECC_SOFT;
#endif

	nand->options = 0;
	
	printf("board_nand_init() 5\n");
	debug("end of nand_init\n");

	return 0;
}

void board_init_f(unsigned long bootflag)
{
	
	   nand_boot();
	   //relocate_code(CONFIG_SYS_TEXT_BASE - TOTAL_MALLOC_LEN, NULL,
			//CONFIG_SYS_TEXT_BASE);
}

