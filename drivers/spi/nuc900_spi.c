/*
 * (C) Copyright 2012
 * Nuvoton Technology Corp. <www.nuvoton.com>
 *
 * SPI driver for NUC900 MCUs.
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
#include <spi.h>
#include <malloc.h>
#include <asm/io.h>

#include "nuc900_spi.h"

#define REG_CLKEN       0xB0000200
#define REG_MFSEL       0xB000000C


void spi_init()
{


        return;
}

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
                        unsigned int max_hz, unsigned int mode)
{
        struct nuc900_spi_slave  *ns;
        
        ns = malloc(sizeof(struct nuc900_spi_slave));
        if (!ns)
                return NULL;

        ns->slave.bus = bus;
        ns->slave.cs = cs;     
        ns->max_hz = max_hz;
        ns->mode = mode;
 
        
        return &ns->slave;        
}


void spi_free_slave(struct spi_slave *slave)
{
        struct nuc900_spi_slave *ns = to_nuc900_spi(slave);

        free(ns);
        
        return;
}

int spi_claim_bus(struct spi_slave *slave)
{
        struct nuc900_spi_slave *ns = to_nuc900_spi(slave);
        
        writel(readl(REG_CLKEN) | 0x20000000, REG_CLKEN);       // SPI clk
        writel(readl(REG_MFSEL) | 0x28000, REG_MFSEL);          // SPI pins
        
        writel(SPI_8BIT, SPICTL);
        
        if(ns->mode & SPI_CS_HIGH)
                writel(SPI_SS_HIGH, SPISSR);
        else
                writel(0, SPISSR);
        if(ns->mode & SPI_CPOL)
                writel(readl(SPICTL) | SPI_CLKPOL, SPICTL);
        else
                writel(readl(SPICTL) & ~SPI_CLKPOL, SPICTL);
        
        if(ns->mode & SPI_CPHA)
                writel(readl(SPICTL) | SPI_CLKPHAINV, SPICTL);
        else
                writel(readl(SPICTL) | SPI_CLKPHA, SPICTL);   
        
        spi_set_speed(slave, ns->max_hz);
        
        return(0);
        
}


void spi_release_bus(struct spi_slave *slave)
{
        writel(readl(REG_CLKEN) & ~0x20000000, REG_CLKEN);       // SPI clk
        writel(readl(REG_MFSEL) & ~0x28000, REG_MFSEL);          // SPI pins  
        
}


int spi_xfer(struct spi_slave *slave, unsigned int bitlen,
                const void *dout, void *din, unsigned long flags)
{
        unsigned int len;
        unsigned int i;
        unsigned char *tx = dout;
        unsigned char *rx = din;        

        //printf("entr xfr\n");
        
        if(bitlen == 0)
                goto out;
        
        if(bitlen % 8) {
                /* Errors always terminate an ongoing transfer */
                flags |= SPI_XFER_END;
                goto out;
        }

        len = bitlen / 8;
        
   
        
        if(flags & SPI_XFER_BEGIN) {
                spi_cs_activate(slave);
        }
        
        //printf("SPICTL=>%x\n", readl(SPICTL));
        //printf("SPISSR=>%x\n", readl(SPISSR));
        //printf("SPIDIV=>%x\n", readl(SPIDIV));     
        
        for (i = 0; i < len; i++) {
                if(tx)
                        writel(*tx++, SPITX0);
                else
                        writel(0, SPITX0);
                
                writel(readl(SPICTL) | SPI_BUSY, SPICTL);
                while (readl(SPICTL) & SPI_BUSY);
                
                if(rx)
                        *rx++ = (unsigned char)readl(SPIRX0);
        }        

out:
        
        if (flags & SPI_XFER_END) {
                /*
                 * Wait until the transfer is completely done before
                 * we deactivate CS.
                 */
                while (readl(SPICTL) & SPI_BUSY);

                spi_cs_deactivate(slave);
        }

        //printf("exit xfr\n");
        return 0;        
        
}

int  spi_cs_is_valid(unsigned int bus, unsigned int cs)
{
        
        return(1);
}

void spi_cs_activate(struct spi_slave *slave)
{
        writel(readl(SPISSR) | SPI_SS_ACT, SPISSR);
        return;
        
}

void spi_cs_deactivate(struct spi_slave *slave)
{
        writel(readl(SPISSR) & ~SPI_SS_ACT, SPISSR);        
        return;
}


void spi_set_speed(struct spi_slave *slave, uint hz)
{
        unsigned int div;
        
        div = SPI_CLK / (hz * 2);
        
        if((SPI_CLK % (hz * 2)) == 0)
                div--;
        
        if(div == 0)
                div = 1;  // div should at lease be 1
                
        if(div > 0xFFFF)
                div = 0xFFFF; // 16 bits only
        
        writel(div, SPIDIV);
        
        return;
}

