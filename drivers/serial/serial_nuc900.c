/*
 * Copyright (c) 2004	Nuvoton Technology Corp.
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
 *
 * Description:   NUC900 UART driver
 */
#include <common.h>
#include <asm/io.h>

#include <serial.h>

#include "serial_nuc900.h"

DECLARE_GLOBAL_DATA_PTR;
/*
 * Initialise the serial port with the given baudrate. The settings are always 8n1.
 */

u32 baudrate = CONFIG_BAUDRATE;
u32 ext_clk  = CONFIG_EXT_CLK;

int nuc900_serial_init (void)
{
	unsigned int temp, temp1 = ext_clk % (baudrate * 16);

	// Configure UART0 GPIO pin
	__raw_writel(__raw_readl(REG_MFSEL) | 0x100, REG_MFSEL);

	// Reset the TX/RX FIFOs
	__raw_writel(TX_RX_FIFO_RESET, UART0_BASE + REG_COM_FCR);

        // calculate baudrate divisor
        temp = ext_clk / (baudrate * 16);
        temp -= 2;
        if(temp1 > (baudrate * 16 / 2))
                temp +=1;

        // set the divisor latch bit
        __raw_writel(ENABLE_DLAB, UART0_BASE + REG_COM_LCR);

        //write the LSB of the baud rate divisor
        __raw_writel(temp & 0xFF, UART0_BASE + REG_COM_LSB);

        // write the MSB of the baud rate divisor
        __raw_writel(temp >> 8, UART0_BASE + REG_COM_MSB);

        // turn off divisor latch bit
        __raw_writel(__raw_readl(UART0_BASE + REG_COM_LCR) & DISABLE_DLAB, UART0_BASE + REG_COM_LCR);

        // 8n1
        __raw_writel(WB_PARITY_NONE | WB_DATA_BITS_8 | WB_STOP_BITS_1, UART0_BASE + REG_COM_LCR);

        // enable rx timeout
        __raw_writel(ENABLE_TIME_OUT, UART0_BASE + REG_COM_TOR);

        // config FIFO trigger level
        __raw_writel(LEVEL_1_BYTE, UART0_BASE + REG_COM_FCR);

	return 0;
}

void nuc900_serial_putc (const char c)
{
        /* Wait until Tx buffer empty */
        while (!(__raw_readl(UART0_BASE + REG_COM_LSR) & THR_EMPTY));
        
        if(c == 0x0A) {
                __raw_writel(0x0D, UART0_BASE + REG_COM_TX);
                while (!(__raw_readl(UART0_BASE + REG_COM_LSR) & THR_EMPTY));
        }
        /* Transmit the character */
        __raw_writel(c, UART0_BASE + REG_COM_TX);
}

void nuc900_serial_puts (const char *s)
{
	while (*s) {
		nuc900_serial_putc (*s++);
	}
}

int nuc900_serial_getc (void)
{
        while (1)  // loop 'til data available
        {
                if (__raw_readl(UART0_BASE + REG_COM_LSR) & RX_DATA_READY)
                        return(__raw_readl(UART0_BASE + REG_COM_RX));
        }
}

int nuc900_serial_tstc (void)
{
	return (__raw_readl(UART0_BASE + REG_COM_LSR) & RX_DATA_READY);
}

void nuc900_serial_setbrg (void)
{

        return;
}

static struct serial_device nuc900_serial_drv = {
        .name   = "nuc900_serial",
        .start  = nuc900_serial_init,
        .stop   = NULL,
        .setbrg = nuc900_serial_setbrg,
        .putc   = nuc900_serial_putc,
        .puts   = nuc900_serial_puts,
        .getc   = nuc900_serial_getc,
        .tstc   = nuc900_serial_tstc,
};

void nuc900_serial_initialize(void)
{
        serial_register(&nuc900_serial_drv);
}

__weak struct serial_device *default_serial_console(void)
{
        return &nuc900_serial_drv;
}

