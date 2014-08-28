/*
 * (C) Copyright 2012
 * Nuvoton Technology Corp. <www.nuvoton.com>
 *
 * SPI header for NUC900 MCUs.
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

#ifndef _NUC900_SPI_H_
#define _NUC900_SPI_H_


#define SPI_BASE 0xB8006200

#define SPICTL         (SPI_BASE + 0x00)
#define SPIDIV         (SPI_BASE + 0x04)
#define SPISSR         (SPI_BASE + 0x08)
#define SPIRX0         (SPI_BASE + 0x10)
#define SPITX0         (SPI_BASE + 0x10)
#define SPIRX1         (SPI_BASE + 0x14)
#define SPITX1         (SPI_BASE + 0x14)
#define SPIRX2         (SPI_BASE + 0x18)
#define SPITX2         (SPI_BASE + 0x18)
#define SPIRX3         (SPI_BASE + 0x1C)
#define SPITX3         (SPI_BASE + 0x1C)

#define SPI_BUSY        0x00000001
#define SPI_CLKPOL      0x80000000
#define SPI_CLKPHAMSK   0x00000006
#define SPI_CLKPHAINV   0x00000002
#define SPI_CLKPHA      0x00000004
#define SPI_8BIT        0x00000040
#define SPI_DUALM	0x00400000
#define SPI_QUADM	0x00200000
#define SPI_SS_ACT      0x00000001
#define SPI_SS_HIGH     0x00000004

#define SPI_CLK         50000000        // From APB clock

struct nuc900_spi_slave {
        struct spi_slave slave;
        unsigned int max_hz;
        unsigned int mode;
};

static inline struct nuc900_spi_slave *to_nuc900_spi(struct spi_slave *slave)
{
        return container_of(slave, struct nuc900_spi_slave, slave);
}

#endif //_NUC900_SPI_H_

