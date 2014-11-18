/*
 * (C) Copyright 2014
 * Nuvoton Technology Corp. <www.nuvoton.com>
 *
 * Configuation settings for the NUC970 EV Board.
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
#include <malloc.h>
#include <mmc.h>
#include "nuc970_mmc.h"

int nuc970_mmc_check_ready_busy(void)
{
        int cnt = 10;
#if 0        
        do {
                udelay(100);
        }while((readl(REG_SDCSR) & 0xF) != 0 && cnt > 0)
#else
        while(cnt-- > 0) {
                writel(readl(REG_SDCSR) | CLK8_OE, REG_SDCSR);
                while(readl(REG_SDCSR) & CLK8_OE);
                if(readl(REG_SDISR) & SDDAT0)
                        break;
        }

#endif
        if(cnt == 0)
                return(-1);
        else
                return(0);
}


#define REG_MFP_GPH_L   0xB00000A8
#define REG_MFP_GPH_H   0xB00000AC

int nuc970_mmc_send_cmd(struct mmc *mmc, struct mmc_cmd *cmd, struct mmc_data *data)
{
        unsigned char *c;
        int i, j, tmp[5];
        unsigned int block_length, blocks;
        unsigned int sdcsr = readl(REG_SDCSR) & DBW;      // keep bus width. config other fields in this fuction

/*
        printf("[%s]REG_SDH_GCTL = 0x%x\n",__FUNCTION__,readl(REG_SDH_GCTL));
        printf("[%s]REG_SDCSR = 0x%x\n",__FUNCTION__,readl(REG_SDCSR));
        printf("[%s]REG_SDIER = 0x%x\n",__FUNCTION__,readl(REG_SDIER));
        printf("[%s]REG_SDISR = 0x%x\n",__FUNCTION__,readl(REG_SDISR));
        printf("[%s]REG_ECTL = 0x%x\n",__FUNCTION__,readl(REG_ECTL));
        printf("[%s]cmd->cmdidx = 0x%x\n",__FUNCTION__,cmd->cmdidx);
        printf("[%s]REG_SDRSP0 = 0x%x\n",__FUNCTION__,readl(REG_SDRSP0));
        printf("[%s]REG_SDRSP1 = 0x%x\n",__FUNCTION__,readl(REG_SDRSP1));
        printf("[%s]REG_MFP_GPH_L = 0x%x\n",__FUNCTION__,readl(REG_MFP_GPH_L));
        printf("[%s]REG_MFP_GPH_H = 0x%x\n",__FUNCTION__,readl(REG_MFP_GPH_H));
	printf("========================================================\n");
*/

        if((readl(REG_SDCSR) & 0xF) != 0)
                if(nuc970_mmc_check_ready_busy() < 0)
                        return(TIMEOUT);
        

        if(mmc_resp_type(cmd) != MMC_RSP_NONE) {
                if(mmc_resp_type(cmd) == MMC_RSP_R2) {
                        sdcsr |= R2_EN;
                } else {
                        sdcsr |= RI_EN;
                }
                
                writel(RITO_IF, REG_SDISR);
                writel(0xFFFF, REG_SDTMOUT);
        }
           
	if (mmc->priv == 0) // SD port 0
        	sdcsr &= ~0x60000000;                    // Select port 0
	else
        	sdcsr = (sdcsr & ~0x60000000) | 0x20000000;                    // Select port 1

        sdcsr |= 0x01010000;                    // Set SDNWR and BLK_CNT to 1, update later
        sdcsr |= (cmd->cmdidx << 8) | CO_EN;
        
        if (data) {
                block_length = data->blocksize;
                blocks = data->blocks;

                //printf("size %d len %d\n", block_length, blocks);
                writel(block_length - 1, REG_SDBLEN);
                
                if (block_length == 0x200) {
                        if (blocks < 256)
                                sdcsr |= (blocks << 16);
                        else
                                printf("NUC970 SD Max block transfer is 255!!\n");
                }                
                //printf("sdcsr 0x%x \n", sdcsr);
                
                if (data->flags == MMC_DATA_READ) {
                        sdcsr |= DI_EN;
                        writel((unsigned int)data->dest, REG_DMACSAR2);
                        
                } else if (data->flags == MMC_DATA_WRITE) {
                        sdcsr |= DO_EN;
                        writel((unsigned int)data->src, REG_DMACSAR2);
                        
                }

        } else {
                block_length = 0;
                blocks = 0;
        }

        
        writel(cmd->cmdarg, REG_SDARG);
        //printf("arg: %x\n", cmd->cmdarg);
        writel(sdcsr, REG_SDCSR);  
        
        while (readl(REG_SDCSR) & CO_EN); //wait 'til command out complete


        
        if(mmc_resp_type(cmd) != MMC_RSP_NONE) {
                //printf("hi there\n");
                if(mmc_resp_type(cmd) == MMC_RSP_R2) {
                        //printf("hi there 2\n");
                        while (readl(REG_SDCSR) & R2_EN); 
                        c = (unsigned char *)SDH_BA;
                        for (i = 0, j = 0; j < 5; i += 4, j++)
                                tmp[j] = (*(c + i) << 24) | 
                                         (*(c + i + 1) << 16) | 
                                         (*(c + i + 2) << 8) | 
                                         (*(c + i + 3));
                        for (i = 0; i < 4; i++)
                                cmd->response[i] = ((tmp[i] & 0x00ffffff) << 8) | 
                                                    ((tmp[i + 1] & 0xff000000) >> 24);                        
                } else {
                        //printf("hi there 1\n");
                        while (1) {
                                if (!(readl(REG_SDCSR) & RI_EN)) 
                                        break;
                                if (readl(REG_SDISR) & RITO_IF) {
                                        writel(RITO_IF, REG_SDISR);
                                        writel(0, REG_SDTMOUT);   
                                        return(TIMEOUT);
                                }
                        } 
                        //printf("=>%x %x %x %x\n", sdcsr, readl(REG_SDBLEN), readl(REG_SDRSP0), readl(REG_SDRSP1));
        		//printf("[xxxx]REG_SDISR = 0x%x\n",readl(REG_SDISR));
                        cmd->response[0] = (readl(REG_SDRSP0) << 8) | (readl(REG_SDRSP1) & 0xff);
                        cmd->response[1] = cmd->response[2] = cmd->response[3] = 0;                        
                }
        }
        if ((readl(REG_SDISR) & CRC_7) == 0) {
                writel(CRC_IF, REG_SDISR);
                if (mmc_resp_type(cmd) & MMC_RSP_CRC) {
			printf("xxxx retrun err!\n");
                        return(COMM_ERR);
                }
                
        }
        
        
        if (data) {
                if (data->flags & MMC_DATA_READ) {
			//printf("R\n");
                        //printf("**** %x %x %x %x\n", readl(REG_DMACCSR), readl(REG_DMACSAR2), readl(REG_DMACBCR), readl(REG_DMACISR));
                        //writel(readl(REG_SDCSR) | DI_EN, REG_SDCSR);
                        
                        while(!(readl(REG_SDISR) & BLKD_IF));                        
                        writel(BLKD_IF, REG_SDISR);
                        //while(1);
                        
                } else if (data->flags & MMC_DATA_WRITE) {
                        //printf("W\n");
                        while(!(readl(REG_SDISR) & BLKD_IF));
                        writel(BLKD_IF, REG_SDISR);
                        
                        writel(readl(REG_SDCSR) | CLK_KEEP0, REG_SDCSR);
                        
                        while (!(readl(REG_SDISR) & SDDAT0)); 
                        writel(readl(REG_SDCSR) & ~CLK_KEEP0, REG_SDCSR);

                }
        }     

        return(0);
        
}

static void nuc970_mmc_set_clock(struct mmc *mmc, uint clock)
{

        uint    div;
     
        if (clock < mmc->f_min)
                clock = mmc->f_min;
        if (clock > mmc->f_max)
                clock = mmc->f_max;

	//printf("clock = %d\n",clock);

        div = MMC_CLK / clock;
	//printf("MMC_CLK = %d, clock=%d,div=0x%x\n",MMC_CLK,clock,div);
        
        if((MMC_CLK % clock) == 0)
                div--;
        
	//printf("bf set CLKDIV : REG_CLKDIVCTL9 = 0x%x\n",readl(REG_CLKDIVCTL9));
        writel((readl(REG_CLKDIVCTL9) & ~0xFF), REG_CLKDIVCTL9); //Set SDH clock source from XIN 
        writel((readl(REG_CLKDIVCTL9) & ~0xFF00) | (div << 8), REG_CLKDIVCTL9); //Set SDH clock divider
	//printf("af set CLKDIV : REG_CLKDIVCTL9 = 0x%x\n",readl(REG_CLKDIVCTL9));
        //printf("set div to %d, clock %d\n", div, clock);
      //  printf("%x %x %x %x %x\n", readl(REG_CLKEN), readl(REG_CLKEN1), readl(REG_MFSEL), readl(REG_GPIOD_DIR), readl(REG_GPIOD_DATAOUT));
#if 0
        if(clock == mmc->f_min) {
                writel(readl(REG_SDCSR) | CLK74_OE, REG_SDCSR);
                while (readl(REG_SDCSR) & CLK74_OE);                
        }
#endif        
        return;
}


void nuc970_mmc_set_ios(struct mmc *mmc)
{
        if(mmc->clock)
                nuc970_mmc_set_clock(mmc, mmc->clock);
        
        if (mmc->bus_width == 4)
                writel(readl(REG_SDCSR) | DBW, REG_SDCSR);
        else
                writel(readl(REG_SDCSR) & ~DBW, REG_SDCSR);        
        
        return;
}


static int _nuc970_mmc_init(struct mmc *mmc)
{
        int volatile i;
        
        writel(GCTL_RST, REG_SDH_GCTL);    
        writel(DMAC_SW_RST, REG_DMACCSR);
        for(i = 0; i < 10; i++);        // Need few clock delay 'til SW_RST auto cleared.
        writel(SD_EN, REG_SDH_GCTL);       
        writel(DMACEN, REG_DMACCSR);

        //writel(readl(REG_ECTL) & ~1, REG_ECTL); // SD port 0 power enable
        //writel(readl(REG_ECTL) & ~2, REG_ECTL); // SD port 1 power enable
        writel(readl(REG_ECTL) & ~3, REG_ECTL); // SD port 0,1 power enable

	if (mmc->priv == 0) { // SD port 0
        	writel(readl(REG_SDCSR) & ~0x60000000, REG_SDCSR); // SD port selection : Select SD0
        	writel(readl(REG_SDIER) | 0x40000000, REG_SDIER); // SD port 0 card detect source set to SD0_nCD
	}
	else {
		writel((readl(REG_SDCSR) & ~0x60000000) | 0x20000000, REG_SDCSR); // SD port selection : Select SD1
        	writel(readl(REG_SDIER) | 0x80000000, REG_SDIER); // SD port 0 card detect source set to SD1_nCD
	}

/*
        printf("[%s]REG_SDH_GCTL = 0x%x\n",__FUNCTION__,readl(REG_SDH_GCTL));
        printf("[%s]REG_SDCSR = 0x%x\n",__FUNCTION__,readl(REG_SDCSR));
        printf("[%s]REG_SDIER = 0x%x\n",__FUNCTION__,readl(REG_SDIER));
        printf("[%s]REG_SDISR = 0x%x\n",__FUNCTION__,readl(REG_SDISR));
        printf("[%s]REG_ECTL = 0x%x\n",__FUNCTION__,readl(REG_ECTL));
        printf("[%s]REG_MFP_GPH_L = 0x%x\n",__FUNCTION__,readl(REG_MFP_GPH_L));
        printf("[%s]REG_MFP_GPH_H = 0x%x\n",__FUNCTION__,readl(REG_MFP_GPH_H));
*/
 
        return(0);
}

//int board_mmc_getcd(u8 *cd, struct mmc *mmc)
int board_mmc_getcd(struct mmc *mmc)
{
	u8 cd;

	//printf("[%s] REG_SDISR = 0x%x\n",__FUNCTION__,readl(REG_SDISR));
	//printf("[%s] REG_SDIER = 0x%x\n",__FUNCTION__,readl(REG_SDIER));

	if (mmc->priv == 0) {
		if (((readl(REG_SDIER) & 0x40000000) >> 30) == 0)  // for DAT3 mode
        		cd = ((readl(REG_SDISR) & (1 << 16)) == (1 << 16)) ? 1 : 0;
		else
        		cd = ((readl(REG_SDISR) & (1 << 16)) == (1 << 16)) ? 0 : 1;
	}
	else {
		if (((readl(REG_SDIER) & 0x80000000) >> 31) == 0)  // for DAT3 mode
        		cd = ((readl(REG_SDISR) & (1 << 17)) == (1 << 17)) ? 1 : 0;
		else
        		cd = ((readl(REG_SDISR) & (1 << 17)) == (1 << 17)) ? 0 : 1;
	}

	//printf("cd = %d\n",cd);

        //return 0;
        return cd;
}

int nuc970_mmc_init(int port)
{
        struct mmc *mmc;
        
        mmc = malloc(sizeof(struct mmc));
        memset(mmc, 0, sizeof(struct mmc));

        sprintf(mmc->name, "mmc");
        //mmc->priv = host;
        mmc->send_cmd = nuc970_mmc_send_cmd;
        mmc->set_ios = nuc970_mmc_set_ios;
        mmc->init = _nuc970_mmc_init;

        mmc->f_min = 400000;
        mmc->f_max = 20000000;
        mmc->voltages = MMC_VDD_33_34 | MMC_VDD_32_33;
        mmc->host_caps = MMC_MODE_4BIT | MMC_MODE_HS | MMC_MODE_HC;

        //mmc->b_max = 0;
        mmc->b_max = 255;

        mmc->priv = port; 
        mmc_register(mmc);

        return(0);
}

