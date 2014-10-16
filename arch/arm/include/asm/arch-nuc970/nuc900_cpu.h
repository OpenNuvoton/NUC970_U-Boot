/**********************************************************************************************************
 *                                                                          
 * Copyright (c) 2005 - 2013 Winbond Electronics Corp. All rights reserved.      
 *                                                                         
 * FILENAME
 *     nuc900_cpu.h
 *
 * VERSION
 *     1.1
 *
 * DESCRIPTION
 *     This file contains the register map of W90P910 processor. The included H/W Macro functions are
 *     listed below.
 *
 *     0. System Address Map
 *     1. SDRAM Controller
 *     2. Global Controller
 *     3. Clock Controller
 *     4. GPIO
 *     5. Fast Serial Bus
 *     6. Host Bus Interface
 *     7. Flash memory Card Controller (FMI)
 *     8. Audio Interface
 *     9. USB Device
 *    10. LCM
 *    11. Sensor DSP
 *    12. Video Capture Engine
 *    13. JPEG Codec
 *    14. MPEG-4 Video Codec
 *    15. 2-D Graphic Engine
 *    16. Video Processing Engine
 *    17. Motion Estimation Engine
 *    18. High Speed UART
 *    19. UART
 *    20. Timer
 *    21. Interrupt Controller (AIC)
 *    22. Universal Serial Interface
 *    23. External Bus Interface 
 *
 * HISTORY
 *     07/06/2005		 Ver 1.0 Created by PC30 MNCheng
 *
 * REMARK
 *     None
 *     
 **********************************************************************************************************/
#ifndef _NUC900_CPU_H
#define _NUC900_CPU_H

/**********************************************************************************************************
 *                                                               
 * 0. System Address Map - Defines the register base address of each Macro 
 *                         function.
 *
 **********************************************************************************************************/
#define    GCR_BA    0xB0000000 /* Global Control */
#define    CLK_BA    0xB0000200 /* Clock Control */
#define    EBI_BA    0xB0001000 /* EBI Control */
#define    PCI_BA    0xB0002000 /* PCI Control */
#define    EMC_BA    0xB0003000 /* Ethernet MAC */
#define    GDMA_BA   0xB0004000 /* GDMA control */
#define    USBH_BA   0xB0005000 /* USB Host Control */
#define    USBD_BA   0xB0006000 /* USB Device Control */
#define    USBP_BA   0xB0006800 /* USB PHY Control */
#define    DMAC_BA   0xB000C000 /* DMA Control */
#define    FMI_BA    0xB000D000 /* Flash Memory Card Interface */
#define    LCM_BA    0xB0008000 /* Display, LCM Interface & Bypass */
#define    ACTL_BA   0xB0009000 /* Audio Control */
#define    ATA_BA    0xB000A000 /* ATA Interface Control */
#define    GE_BA     0xB000B000 /* 2-D Graphic Engine */

#define    UART0_BA  0xB8000000 /* UART0 Control (console) */
#define    UART1_BA  0xB8000100 /* UART1 Control (Bluetooth) */
#define    UART2_BA  0xB8000200 /* UART2 Control (IrDA) */
#define    UART3_BA  0xB8000300 /* UART3 Control (micro-printer) */
#define    UART4_BA  0xB8000400 /* UART4 Control (console) */
#define    TMR_BA    0xB8001000 /* Timer */
#define    AIC_BA    0xB8002000 /* Interrupt Controller */
#define    GPIO_BA   0xB8003000 /* GPIO Control */
#define    RTC_BA	 0xB8004000 /* Real Time Clock Control */
#define    SC_BA     0xB8005000 /* Smart Card Control */
#define    I2C0_BA   0xB8006000 /* I2C 0 Control */
#define    I2C1_BA   0xB8006100 /* I2C 1 Control */
#define    USI_BA    0xB8006200 /* Synchronous Serial Port */
#define    PWM_BA    0xB8007000 /* Pulse Width Modulation Control */
#define    KPI_BA    0xB8008000 /* Keypad Interface Control */
#define    PS2_BA    0xB8009000 /* PS2 Interface Control */
#define    ADC_BA    0xB800A000 /* ADC Control */

 
 
/**********************************************************************************************************
 *                                                               
 * 1. Global Control Registers  
 *
 **********************************************************************************************************/
#define REG_PDID		(GCR_BA+0x000)    /* Product Identifier Register */
#define REG_PWRON		(GCR_BA+0x004)    /* Power-On Setting Register */
#define REG_ARBCON		(GCR_BA+0x008)    /* Arbitration Control Register */
//#define REG_PINPEC		(GCR_BA+0x00C)    /* Pin Pull-up/down Enable Control Register */
#define REG_MFSEL		(GCR_BA+0x00C)    /* Multiple Function Pin Select Register */



/**********************************************************************************************************
 *                                                               
 * 2. Clock Control Registers  
 *
 **********************************************************************************************************/
#define REG_CLKEN		(CLK_BA + 0x00)    /* Clock Enable Register */
#define REG_CLKSEL		(CLK_BA + 0x04)    /* Clock Select Register */
#define REG_CLKDIV		(CLK_BA + 0x08)    /* Clock Divider Control Register */
#define REG_PLLCON0		(CLK_BA + 0x0C)    /* PLL Control Register 0 */
#define REG_PLLCON1		(CLK_BA + 0x10)	   /* PLL Control Register 1 */
#define REG_PMCON		(CLK_BA + 0x14)	   /* Power Management Control Register */
#define REG_IRQWAKECON	(CLK_BA + 0x18)	   /* IRQ Wakeup Control Register */
#define REG_IRQWAKEFLAG	(CLK_BA + 0x1C)	   /* IRQ Wakeup Flag Register */
#define REG_IPSRST		(CLK_BA + 0x20)    /* IP software reset Register */
#define REG_CLKEN1		(CLK_BA + 0x24)    /* Clock Enable 1 Register */
#define REG_CLKDIV1		(CLK_BA + 0x28)    /* Clock divider control 1 Register */



/**********************************************************************************************************
 *                                                               
 * 3. EBI Control Registers  
 *
 **********************************************************************************************************/
#define REG_EBICON		(EBI_BA + 0x000)    /* EBI control register */
#define REG_ROMCON		(EBI_BA + 0x004)    /* ROM/FLASH control register */
#define REG_SDCONF0		(EBI_BA + 0x008)    /* SDRAM bank 0 configuration register */
#define REG_SDCONF1		(EBI_BA + 0x00C)    /* SDRAM bank 1 configuration register */
#define REG_SDTIME0		(EBI_BA + 0x010)    /* SDRAM bank 0 timing control register */
#define REG_SDTIME1		(EBI_BA + 0x014)    /* SDRAM bank 1 timing control register */
#define REG_EXT0CON		(EBI_BA + 0x018)    /* External I/O 0 control register */
#define REG_EXT1CON		(EBI_BA + 0x01C)    /* External I/O 1 control register */
#define REG_EXT2CON		(EBI_BA + 0x020)    /* External I/O 2 control register */
#define REG_EXT3CON		(EBI_BA + 0x024)    /* External I/O 3 control register */
#define REG_EXT4CON		(EBI_BA + 0x028)    /* External I/O 4 control register */
#define REG_CKSKEW		(EBI_BA + 0x02C)   /* Clock skew control register (for testing) */


/**********************************************************************************************************
 *                                                               
 * 4. GPIO Control Registers  
 *
 **********************************************************************************************************/

/* GPIO Control Registers */
#define REG_GPIOC_DIR		(GPIO_BA+0x04)  /* GPIO portC direction control register */
#define REG_GPIOC_DATAOUT	(GPIO_BA+0x08)  /* GPIO portC data output register */
#define REG_GPIOC_DATAIN	(GPIO_BA+0x0C)  /* GPIO portC data input register */
#define REG_GPIOD_DIR		(GPIO_BA+0x14)  /* GPIO portD direction control register */
#define REG_GPIOD_DATAOUT	(GPIO_BA+0x18)  /* GPIO portD data output register */
#define REG_GPIOD_DATAIN	(GPIO_BA+0x1C)  /* GPIO portD data input register */
#define REG_GPIOE_DIR		(GPIO_BA+0x24)  /* GPIO portE direction control register */
#define REG_GPIOE_DATAOUT	(GPIO_BA+0x28)  /* GPIO portE data output register */
#define REG_GPIOE_DATAIN	(GPIO_BA+0x2C)  /* GPIO portE data input register */
#define REG_GPIOF_DIR		(GPIO_BA+0x34)  /* GPIO portF direction control register */
#define REG_GPIOF_DATAOUT	(GPIO_BA+0x38)  /* GPIO portF data output register */
#define REG_GPIOF_DATAIN	(GPIO_BA+0x3C)  /* GPIO portF data input register */
#define REG_GPIOG_DIR		(GPIO_BA+0x44)  /* GPIO portG direction control register */
#define REG_GPIOG_DATAOUT	(GPIO_BA+0x48)  /* GPIO portG data output register */
#define REG_GPIOG_DATAIN	(GPIO_BA+0x4C)  /* GPIO portG data input register */
#define REG_GPIOH_DBNCE		(GPIO_BA+0x50)  /* GPIO portH input debounce control reg. */
#define REG_GPIOH_DIR		(GPIO_BA+0x54)  /* GPIO portH direction control register */
#define REG_GPIOH_DATAOUT	(GPIO_BA+0x58)  /* GPIO portH data output register */
#define REG_GPIOH_DATAIN	(GPIO_BA+0x5C)  /* GPIO portH data input register */
#define REG_GPIOI_DIR		(GPIO_BA+0x64)  /* GPIO portI direction control register */
#define REG_GPIOI_DATAOUT	(GPIO_BA+0x68)  /* GPIO portI data output register */
#define REG_GPIOI_DATAIN	(GPIO_BA+0x6C)  /* GPIO portI data input register */

/**********************************************************************************************************
 *                                                               
 * 5. Fast Serial Bus Control Registers  
 *
 **********************************************************************************************************/
#define REG_FastSerialBusTrigger		(FSB_BA+0x00)    /* FSB Trigger and SCK Control */
#define REG_FastSerialBusStatus			(FSB_BA+0x04)    /* FSB Status Register */
#define REG_FastSerialBusCR				(FSB_BA+0x08)    /* FSB Control Register */
#define REG_FastSerialBusSCKSDA0		(FSB_BA+0x0C)    /* FSB SCKSDA 0 register */
#define REG_FastSerialBusSCKSDA1		(FSB_BA+0x10)    /* FSB SCKSDA 1 register */
#define REG_SerialBusCR					(FSB_BA+0x14)    /* FSB Software Mode Register */
 
 
/**********************************************************************************************************
 *                                                               
 * 6. Host Bus Interface Control Registers  
 *
 **********************************************************************************************************/
#define REG_HICFCR	(HIC_BA+0x00)		/* HIC function control REGister */
#define REG_HICDAR	(HIC_BA+0x04)		/* HIC software DMA address REGister */
#define REG_HICDLR	(HIC_BA+0x08)		/* HIC software DMA length REGister */
#define REG_HICIER	(HIC_BA+0x0C)		/* HIC interrupt enable REGister */
#define REG_HICISR	(HIC_BA+0x10)		/* HIC interrupt status REGister */
#define REG_HICBIS	(HIC_BA+0x14)		/* HIC BIST REGister */
#define REG_HICCMR	(HIC_BA+0x80)		/* HIC command port REGister */
#define REG_HICPAR	(HIC_BA+0x84)		/* HIC parameter/address port REGister */
#define REG_HICBLR	(HIC_BA+0x88)		/* HIC parameter/burst transfer count REGister */
#define REG_HICDR	(HIC_BA+0x8C)		/* HIC data port REGister */
#define REG_HICSR	(HIC_BA+0x90)		/* HIC status port REGister */
#define REG_HICBUF	(HIC_BA+0x100)		/* HIC internal buffer starting address */
#define _HICBUF_NUM	(64)
 
/**********************************************************************************************************
 *                                                               
 * 7. Flash memory Card Control Registers  
 *
 **********************************************************************************************************/
/* Flash Memory Interface Registers definition */

#define REG_FB0			(DMAC_BA+0x000)	/* Flash buffer 0 ~ 512 */
#define REG_DMACCSR		(DMAC_BA+0x800)	/* DMAC control and status register */
#define REG_DMACSAR1	(DMAC_BA+0x804)	/* DMAC transfer starting address register 1 */
#define REG_DMACSAR2	(DMAC_BA+0x808)	/* DMAC transfer starting address register 2 */
#define REG_DMACBCR		(DMAC_BA+0x80C)	/* DMAC transfer byte count register */
#define REG_DMACIER		(DMAC_BA+0x810)	/* DMAC interrupt enable register */
#define REG_DMACISR		(DMAC_BA+0x814)	/* DMAC interrupt status register */
#define REG_DMACBIST	(DMAC_BA+0x818)	/* DMAC BIST register */


#define REG_FMICSR    (FMI_BA+0x000)   /* FMI control register */
#define REG_FMIIER    (FMI_BA+0x004)   /* FMI interrupt control register */
#define REG_FMIISR    (FMI_BA+0x008)   /* FMI interrupt status register */

/* Secure Digit Registers definition */
#define REG_SDCSR    (FMI_BA+0x020)   /* SD control and status register */
#define REG_SDARG    (FMI_BA+0x024)   /* SD command argument register */
#define REG_SDIER    (FMI_BA+0x028)   /* SD interrupt enable register */
#define REG_SDISR    (FMI_BA+0x02C)   /* SD interrupt status register */
#define REG_SDRSP0   (FMI_BA+0x030)   /* SD receive response token register 0 */
#define REG_SDRSP1   (FMI_BA+0x034)   /* SD receive response token register 1 */
#define REG_SDBLEN   (FMI_BA+0x038)   /* SD block length register */
#define REG_SDTMOUT  (FMI_BA+0x03C)   /* SD response/data-in time out register */
 
/* Memory Stick Registers definition */
#define REG_MSCSR	 (FMI_BA+0x060)   /* MS control and status register */
#define REG_MSIER	 (FMI_BA+0x064)   /* MS interrupt control register */
#define REG_MSISR	 (FMI_BA+0x068)   /* MS interrupt status register */
#define REG_MSBUF1	 (FMI_BA+0x06C)   /* Memory stick register buffer 1 */
#define REG_MSBUF2	 (FMI_BA+0x070)   /* Memory stick register buffer 2 */

/* SmartMedia Registers definition */
#define REG_SMCSR    (FMI_BA+0x0A0)   /* SmartMedia control register */
#define REG_SMTCR    (FMI_BA+0x0A4)   /* SmartMedia control register */
#define REG_SMIER    (FMI_BA+0x0A8)   /* SmartMedia interrupt enable register */
#define REG_SMISR    (FMI_BA+0x0AC)   /* SmartMedia input signal and interrupt status register */
#define REG_SMCMD    (FMI_BA+0x0B0)   /* SmartMedia command port register */
#define REG_SMADDR   (FMI_BA+0x0B4)   /* SmartMedia address port register */
#define REG_SMDATA   (FMI_BA+0x0B8)   /* SmartMedia data port register */
#define REG_SMECC0   (FMI_BA+0x0BC)   /* SmartMedia error correction code 0 regisrer */
#define REG_SMECC1   (FMI_BA+0x0C0)   /* SmartMedia error correction code 1 regisrer */
#define REG_SMECC2   (FMI_BA+0x0C4)   /* SmartMedia error correction code 2 regisrer */
#define REG_SMECC3   (FMI_BA+0x0C8)   /* SmartMedia error correction code 3 regisrer */
#define REG_SMRA_0   (FMI_BA+0x0CC)   /* SmartMedia redundant area register */
#define REG_SMRA_1   (FMI_BA+0x0D0)   /* SmartMedia redundant area register */
#define REG_SMRA_2   (FMI_BA+0x0D4)   /* SmartMedia redundant area register */
#define REG_SMRA_3   (FMI_BA+0x0D8)   /* SmartMedia redundant area register */
#define REG_SMRA_4   (FMI_BA+0x0DC)   /* SmartMedia redundant area register */
#define REG_SMRA_5   (FMI_BA+0x0E0)   /* SmartMedia redundant area register */
#define REG_SMRA_6   (FMI_BA+0x0E4)   /* SmartMedia redundant area register */
#define REG_SMRA_7   (FMI_BA+0x0E8)   /* SmartMedia redundant area register */
#define REG_SMRA_8   (FMI_BA+0x0EC)   /* SmartMedia redundant area register */
#define REG_SMRA_9   (FMI_BA+0x0F0)   /* SmartMedia redundant area register */
#define REG_SMRA_10  (FMI_BA+0x0F4)   /* SmartMedia redundant area register */
#define REG_SMRA_11  (FMI_BA+0x0F8)   /* SmartMedia redundant area register */
#define REG_SMRA_12  (FMI_BA+0x0FC)   /* SmartMedia redundant area register */
#define REG_SMRA_13  (FMI_BA+0x100)   /* SmartMedia redundant area register */
#define REG_SMRA_14  (FMI_BA+0x104)   /* SmartMedia redundant area register */
#define REG_SMRA_15  (FMI_BA+0x108)   /* SmartMedia redundant area register */
#define REG_SMECCAD0 (FMI_BA+0x10C)   /* SmartMedia ECC correction address register 0 */
#define REG_SMECCAD1 (FMI_BA+0x110)   /* SmartMedia ECC correction address register 1 */
#define REG_SMECC4ST (FMI_BA+0x114)   /* SmartMedia ECC4 correction status register */
#define REG_SMECC4F1A1 (FMI_BA+0x118) /* SmartMedia ECC4 field-1 error address-1 register */
#define REG_SMECC4F1A2 (FMI_BA+0x11C) /* SmartMedia ECC4 field-1 error address-2 register */
#define REG_SMECC4F1D  (FMI_BA+0x120) /* SmartMedia ECC4 field-1 error data register */
#define REG_SMECC4F2A1 (FMI_BA+0x124) /* SmartMedia ECC4 field-2 error address-1 register */
#define REG_SMECC4F2A2 (FMI_BA+0x128) /* SmartMedia ECC4 field-2 error address-2 register */
#define REG_SMECC4F2D  (FMI_BA+0x12C) /* SmartMedia ECC4 field-2 error data register */
#define REG_SMECC4F3A1 (FMI_BA+0x130) /* SmartMedia ECC4 field-3 error address-1 register */
#define REG_SMECC4F3A2 (FMI_BA+0x134) /* SmartMedia ECC4 field-3 error address-2 register */
#define REG_SMECC4F3D  (FMI_BA+0x138) /* SmartMedia ECC4 field-3 error data register */
#define REG_SMECC4F4A1 (FMI_BA+0x13C) /* SmartMedia ECC4 field-4 error address-1 register */
#define REG_SMECC4F4A2 (FMI_BA+0x140) /* SmartMedia ECC4 field-4 error address-2 register */
#define REG_SMECC4F4D  (FMI_BA+0x144) /* SmartMedia ECC4 field-4 error data register */

 
/**********************************************************************************************************
 *                                                               
 * 8. Audio Interface Control Registers  
 *
 **********************************************************************************************************/
#define REG_ACTL_CON			(ADO_BA + 0x00)   /* Audio controller control register */
#define REG_ACTL_RESET			(ADO_BA + 0x04)   /* Sub block reset control register */
#define REG_ACTL_RDSTB			(ADO_BA + 0x08)   /* DMA destination base address register for record */
#define REG_ACTL_RDST_LENGTH	(ADO_BA + 0x0C)   /* DMA destination length register for record */
#define REG_ACTL_PDSTB			(ADO_BA + 0x18)   /* DMA destination base address register for play */
#define REG_ACTL_PDST_LENGTH	(ADO_BA + 0x1C)   /* DMA destination length register for play */
#define REG_ACTL_RSR			(ADO_BA + 0x14)   /* Record status register */
#define REG_ACTL_PSR			(ADO_BA + 0x24)   /* Play status register */
#define REG_ACTL_IISCON			(ADO_BA + 0x28)   /* IIS control register */
#define REG_ACTL_ACCON			(ADO_BA + 0x2C)   /* AC-link control register */
#define REG_ACTL_ACOS0			(ADO_BA + 0x30)   /* AC-link out slot 0 */
#define REG_ACTL_ACOS1			(ADO_BA + 0x34)   /* AC-link out slot 1 */
#define REG_ACTL_ACOS2			(ADO_BA + 0x38)   /* AC-link out slot 2 */
#define REG_ACTL_ACIS0			(ADO_BA + 0x3C)   /* AC-link in slot 0 */
#define REG_ACTL_ACIS1			(ADO_BA + 0x40)   /* AC-link in slot 1 */
#define REG_ACTL_ACIS2			(ADO_BA + 0x44)   /* AC-link in slot 2 */
#define REG_ACTL_ADCON			(ADO_BA + 0x48)   /* ADC0 control register */
#define REG_ACTL_M80CON			(ADO_BA + 0x4C)   /* M80 interface control register */
#define REG_ACTL_M80DATA0		(ADO_BA + 0x50)   /* M80 data0 register */
#define REG_ACTL_M80DATA1		(ADO_BA + 0x54)   /* M80 data1 register */
#define REG_ACTL_M80DATA2		(ADO_BA + 0x58)   /* M80 data2 register */
#define REG_ACTL_M80DATA3		(ADO_BA + 0x5C)   /* M80 data3 register */
#define REG_ACTL_M80ADDR		(ADO_BA + 0x60)   /* M80 interface start address register */
#define REG_ACTL_M80SRADDR		(ADO_BA + 0x64)   /* M80 interface start address register of right channel */
#define REG_ACTL_M80SIZE		(ADO_BA + 0x70)   /* M80 interface data size register */
#define REG_ACTL_DACON			(ADO_BA + 0x74)   /* DAC control register */
 
 
/**********************************************************************************************************
 *                                                               
 * 9. USB Device Control Registers  
 *
 **********************************************************************************************************/
#define REG_USBD_IRQ_STAT_L		(USBD_BA+0x00)		/* interrupt status low register */
#define REG_USBD_IRQ_ENB_L		(USBD_BA+0x08)		/* interrupt enable low register */
#define REG_USBD_IRQ_STAT		(USBD_BA+0x10)		/* usb interrupt status register */
#define REG_USBD_IRQ_ENB		(USBD_BA+0x14)		/* usb interrupt enable register */
#define REG_USBD_OPER			(USBD_BA+0x18)		/* usb operation register */
#define REG_USBD_FRAME_CNT		(USBD_BA+0x1c)		/* usb frame count register */
#define REG_USBD_ADDR			(USBD_BA+0x20)		/* usb address register */
#define REG_USBD_TEST			(USBD_BA+0x24)		/* usb test mode register */
#define REG_USBD_CEP_DATA_BUF	(USBD_BA+0x28)		/* control-ep data buffer register */
#define REG_USBD_CEP_CTRL_STAT	(USBD_BA+0x2c)		/* control-ep control and status register */
#define REG_USBD_CEP_IRQ_ENB	(USBD_BA+0x30)		/* control-ep interrupt enable register */
#define REG_USBD_CEP_IRQ_STAT	(USBD_BA+0x34)		/* control-ep interrupt status register */
#define REG_USBD_IN_TRNSFR_CNT	(USBD_BA+0x38)		/* in-transfer data count register */
#define REG_USBD_OUT_TRNSFR_CNT	(USBD_BA+0x3c)		/* out-transfer data count register */
#define REG_USBD_CEP_CNT		(USBD_BA+0x40)		/* control-ep data count register */
#define REG_USBD_SETUP1_0		(USBD_BA+0x44)		/* setup byte1 & byte0 register */
#define REG_USBD_SETUP3_2		(USBD_BA+0x48)		/* setup byte3 & byte2 register */
#define REG_USBD_SETUP5_4		(USBD_BA+0x4c)		/* setup byte5 & byte4 register */
#define REG_USBD_SETUP7_6		(USBD_BA+0x50)		/* setup byte7 & byte6 register */
#define REG_USBD_CEP_START_ADDR	(USBD_BA+0x54)		/* control-ep ram start address register */
#define REG_USBD_CEP_END_ADDR	(USBD_BA+0x58)		/* control-ep ram end address register */
#define REG_USBD_DMA_CTRL_STS	(USBD_BA+0x5c)		/* dma control and status register */
#define REG_USBD_DMA_CNT		(USBD_BA+0x60)		/* dma count register */
/* endpoint A */
#define REG_USBD_EPA_DATA_BUF	(USBD_BA+0x64)		/* endpoint A data buffer register */
#define REG_USBD_EPA_IRQ_STAT	(USBD_BA+0x68)		/* endpoint A interrupt status register */
#define REG_USBD_EPA_IRQ_ENB	(USBD_BA+0x6c)		/* endpoint A interrupt enable register */
#define REG_USBD_EPA_DATA_CNT	(USBD_BA+0x70)		/* data count available in endpoint A buffer */
#define REG_USBD_EPA_RSP_SC		(USBD_BA+0x74)		/* endpoint A response register set/clear */
#define REG_USBD_EPA_MPS		(USBD_BA+0x78)		/* endpoint A max packet size register */
#define REG_USBD_EPA_TRF_CNT	(USBD_BA+0x7c)		/* endpoint A transfer count register */
#define REG_USBD_EPA_CFG		(USBD_BA+0x80)		/* endpoint A configuration register */
#define REG_USBD_EPA_START_ADDR	(USBD_BA+0x84)		/* endpoint A ram start address register */
#define REG_USBD_EPA_END_ADDR	(USBD_BA+0x88)		/* endpoint A ram end address register */
/* endpoint B */
#define REG_USBD_EPB_DATA_BUF	(USBD_BA+0x8c)		/* endpoint B data buffer register */
#define REG_USBD_EPB_IRQ_STAT	(USBD_BA+0x90)		/* endpoint B interrupt status register */
#define REG_USBD_EPB_IRQ_ENB	(USBD_BA+0x94)		/* endpoint B interrupt enable register */
#define REG_USBD_EPB_DATA_CNT	(USBD_BA+0x98)		/* data count available in endpoint B buffer */
#define REG_USBD_EPB_RSP_SC		(USBD_BA+0x9c)		/* endpoint B response register set/clear */
#define REG_USBD_EPB_MPS		(USBD_BA+0xa0)		/* endpoint B max packet size register */
#define REG_USBD_EPB_TRF_CNT	(USBD_BA+0xa4)		/* endpoint B transfer count register */
#define REG_USBD_EPB_CFG		(USBD_BA+0xa8)		/* endpoint B configuration register */
#define REG_USBD_EPB_START_ADDR	(USBD_BA+0xac)		/* endpoint B ram start address register */
#define REG_USBD_EPB_END_ADDR	(USBD_BA+0xb0)		/* endpoint B ram end address register */
/* endpoint C */
#define REG_USBD_EPC_DATA_BUF	(USBD_BA+0xb4)		/* endpoint C data buffer register */
#define REG_USBD_EPC_IRQ_STAT	(USBD_BA+0xb8)		/* endpoint C interrupt status register */
#define REG_USBD_EPC_IRQ_ENB	(USBD_BA+0xbc)		/* endpoint C interrupt enable register */
#define REG_USBD_EPC_DATA_CNT	(USBD_BA+0xc0)		/* data count available in endpoint C buffer */
#define REG_USBD_EPC_RSP_SC		(USBD_BA+0xc4)		/* endpoint C response register set/clear */
#define REG_USBD_EPC_MPS		(USBD_BA+0xc8)		/* endpoint C max packet size register */
#define REG_USBD_EPC_TRF_CNT	(USBD_BA+0xcc)		/* endpoint C transfer count register */
#define REG_USBD_EPC_CFG		(USBD_BA+0xd0)		/* endpoint C configuration register */
#define REG_USBD_EPC_START_ADDR	(USBD_BA+0xd4)		/* endpoint C ram start address register */
#define REG_USBD_EPC_END_ADDR	(USBD_BA+0xd8)		/* endpoint C ram end address register */
/* endpoint D */
#define REG_USBD_EPD_DATA_BUF	(USBD_BA+0xdc)		/* endpoint D data buffer register */
#define REG_USBD_EPD_IRQ_STAT	(USBD_BA+0xe0)		/* endpoint D interrupt status register */
#define REG_USBD_EPD_IRQ_ENB	(USBD_BA+0xe4)		/* endpoint D interrupt enable register */
#define REG_USBD_EPD_DATA_CNT	(USBD_BA+0xe8)		/* data count available in endpoint D buffer */
#define REG_USBD_EPD_RSP_SC		(USBD_BA+0xec)		/* endpoint D response register set/clear */
#define REG_USBD_EPD_MPS		(USBD_BA+0xf0)		/* endpoint D max packet size register */
#define REG_USBD_EPD_TRF_CNT	(USBD_BA+0xf4)		/* endpoint D transfer count register */
#define REG_USBD_EPD_CFG		(USBD_BA+0xf8)		/* endpoint D configuration register */
#define REG_USBD_EPD_START_ADDR	(USBD_BA+0xfc)		/* endpoint D ram start address register */
#define REG_USBD_EPD_END_ADDR	(USBD_BA+0x100)		/* endpoint D ram end address register */
/* endpoint E */
#define REG_USBD_EPE_DATA_BUF	(USBD_BA+0x104)		/* endpoint E data buffer register */
#define REG_USBD_EPE_IRQ_STAT	(USBD_BA+0x108)		/* endpoint E interrupt status register */
#define REG_USBD_EPE_IRQ_ENB	(USBD_BA+0x10c)		/* endpoint E interrupt enable register */
#define REG_USBD_EPE_DATA_CNT	(USBD_BA+0x110)		/* data count available in endpoint E buffer */
#define REG_USBD_EPE_RSP_SC		(USBD_BA+0x114)		/* endpoint E response register set/clear */
#define REG_USBD_EPE_MPS		(USBD_BA+0x118)		/* endpoint E max packet size register */
#define REG_USBD_EPE_TRF_CNT	(USBD_BA+0x11c)		/* endpoint E transfer count register */
#define REG_USBD_EPE_CFG		(USBD_BA+0x120)		/* endpoint E configuration register */
#define REG_USBD_EPE_START_ADDR	(USBD_BA+0x124)		/* endpoint E ram start address register */
#define REG_USBD_EPE_END_ADDR	(USBD_BA+0x128)		/* endpoint E ram end address register */
/* endpoint F */
#define REG_USBD_EPF_DATA_BUF	(USBD_BA+0x12c)		/* endpoint F data buffer register */
#define REG_USBD_EPF_IRQ_STAT	(USBD_BA+0x130)		/* endpoint F interrupt status register */
#define REG_USBD_EPF_IRQ_ENB	(USBD_BA+0x134)		/* endpoint F interrupt enable register */
#define REG_USBD_EPF_DATA_CNT	(USBD_BA+0x138)		/* data count available in endpoint F buffer */
#define REG_USBD_EPF_RSP_SC		(USBD_BA+0x13c)		/* endpoint F response register set/clear */
#define REG_USBD_EPF_MPS		(USBD_BA+0x140)		/* endpoint F max packet size register */
#define REG_USBD_EPF_TRF_CNT	(USBD_BA+0x144)		/* endpoint F transfer count register */
#define REG_USBD_EPF_CFG		(USBD_BA+0x148)		/* endpoint F configuration register */
#define REG_USBD_EPF_START_ADDR	(USBD_BA+0x14c)		/* endpoint F ram start address register */
#define REG_USBD_EPF_END_ADDR	(USBD_BA+0x150)		/* endpoint F ram end address register */
#define REG_USBD_AHB_DMA_ADDR	(USBD_BA+0x700)		/* AHB_DMA address register */
#define REG_USBD_PHY_CTL		(USBD_BA+0x704)		/* PHY control register */


/**********************************************************************************************************
 *                                                               
 * 10. LCM Control Registers  
 *
 **********************************************************************************************************/
#define REG_LCM_DCCS	     (LCM_BA+0x00)    /* Display Controller Control/Status Register */
#define REG_LCM_DEV_CTRL     (LCM_BA+0x04)    /* Display Output Device Control Register */
#define REG_LCM_MPU_CMD	     (LCM_BA+0x08)    /* MPU-Interface LCD Write Command */
#define REG_LCM_INT_CS	     (LCM_BA+0x0c)    /* Interrupt Control/Status Register */
#define REG_LCM_CRTC_SIZE    (LCM_BA+0x10)    /* CRTC Display Size Control Register */
#define REG_LCM_CRTC_DEND    (LCM_BA+0x14)    /* CRTC Display Enable End */
#define REG_LCM_CRTC_HR	     (LCM_BA+0x18)    /* CRTC Internal Horizontal Retrace Control Register */
#define REG_LCM_CRTC_HSYNC   (LCM_BA+0x1C)    /* CRTC Horizontal Sync Control Register */
#define REG_LCM_CRTC_VR	     (LCM_BA+0x20)    /* CRTC Internal Vertical Retrace Control Register */
#define REG_LCM_VA_BADDR0    (LCM_BA+0x24)    /* Video Stream Frame Buffer-0 Starting Address */
#define REG_LCM_VA_BADDR1    (LCM_BA+0x28)    /* Video Stream Frame Buffer-1 Starting Address */
#define REG_LCM_VA_FBCTRL    (LCM_BA+0x2C)    /* Video Stream Frame Buffer Control Register */
#define REG_LCM_VA_SCALE     (LCM_BA+0x30)    /* Video Stream Scaling Control Register */
#define REG_LCM_VA_TEST      (LCM_BA+0x34)    /* Test mode control register */
#define REG_LCM_OSD_WIN_S    (LCM_BA+0x40)    /* OSD Window Starting Coordinates */
#define REG_LCM_OSD_WIN_E    (LCM_BA+0x44)    /* OSD Window Ending Coordinates */
#define REG_LCM_OSD_BADDR    (LCM_BA+0x48)    /* OSD Stream Frame Buffer Starting Address */
#define REG_LCM_OSD_FBCTRL   (LCM_BA+0x4c)    /* OSD Stream Frame Buffer Control Register */
#define REG_LCM_OSD_OVERLAY  (LCM_BA+0x50)    /* OSD Overlay Control Register */
#define REG_LCM_OSD_CKEY     (LCM_BA+0x54)    /* OSD Overlay Color-Key Pattern Register */
#define REG_LCM_OSD_CMASK    (LCM_BA+0x58)    /* OSD Overlay Color-Key Mask Register */


/**********************************************************************************************************
 *                                                               
 * 11. Sensor DSP Control Registers  
 *
 **********************************************************************************************************/
#define REG_DSPFunctionCR		(DSP_BA+0x00)    /* Switches of Sub-blocks */
#define	REG_DSPInterruptCR		(DSP_BA+0x04)    /* Interrupt Mask and Status */

/* Sensor interface control REG_isters */
#define	REG_DSPInterfaceCR		(DSP_BA+0x08)    /* Sensor Interface */
#define REG_DSPHsyncCR1			(DSP_BA+0x0C)    /* Master mode Hsync position */
#define REG_DSPHsyncCR2			(DSP_BA+0x10)    /* Master mode Hsync width */
#define REG_DSPVsyncCR1			(DSP_BA+0x14)    /* Master mode Vsync position */
#define REG_DSPVsyncCR2			(DSP_BA+0x18)    /* Master mode Vsync width */
#define REG_DSPLineCounter		(DSP_BA+0x1C)    /* Current Line Position */

/* Black level control REG_isters */
#define	REG_DSPBlackLMode		(DSP_BA+0x20)    /* Black Level Clamping Mode */
#define REG_DSPBlackLCropCR1	(DSP_BA+0x24)    /* Black Level Clamping Parameters */
#define	REG_DSPBlackLCropCR2	(DSP_BA+0x28)    /* Position of Black Level Window */
#define REG_DSPUserBlackLCR		(DSP_BA+0x2C)    /* User defined black level */
#define	REG_DSPFrameDBL			(DSP_BA+0x30)    /* Frame base detected black level */

/* Cropping window */
#define REG_DSPCropCR1			(DSP_BA+0x34)    /* Starting Address of Cropping Window */
#define	REG_DSPCropCR2			(DSP_BA+0x38)    /* Width and Height of Cropping Window */

/* digital programmable gain multiplier */
#define REG_DSPColorBalCR1		(DSP_BA+0x3C)    /* Gain factor for R and B */
#define	REG_DSPColorBalCR2		(DSP_BA+0x40)    /* Gain factor for Gr and Gb */

/* MCG and edge enhancement */
#define	REG_DSPMCGCR			(DSP_BA+0x44)    /* MCG Threshold */
#define	REG_DSPEdgeConfigCR		(DSP_BA+0x48)    /* Edge Configuration */

/* Color correction matrix */
#define	REG_DSPColorMatrixCR1	(DSP_BA+0x4C)    /* Color Correction Matrix -1 */
#define	REG_DSPColorMatrixCR2	(DSP_BA+0x50)    /* Color Correction Matrix -2 */
#define	REG_DSPColorMatrixCR3	(DSP_BA+0x54)    /* Color Correction Matrix -3 */

/* High saturation suppression */
#define	REG_DSPHSSCR			(DSP_BA+0x60)    /* High Saturation Suppression */

/* Gamma correction matrix */
#define	REG_DSPGammaCR			(DSP_BA+0x64)    /* Gamma Correction Type Selection */
#define	REG_DSPGammaTbl1		(DSP_BA+0x70)    /* Gamma Correction Table 0x70 ~ 0xAC */
#define	REG_DSPGammaTbl2		(DSP_BA+0x74)
#define	REG_DSPGammaTbl3		(DSP_BA+0x78)
#define	REG_DSPGammaTbl4		(DSP_BA+0x7C)
#define	REG_DSPGammaTbl5		(DSP_BA+0x80)
#define	REG_DSPGammaTbl6		(DSP_BA+0x84)
#define	REG_DSPGammaTbl7		(DSP_BA+0x88)
#define	REG_DSPGammaTbl8		(DSP_BA+0x8C)
#define	REG_DSPGammaTbl9		(DSP_BA+0x90)
#define	REG_DSPGammaTbl10		(DSP_BA+0x94)
#define	REG_DSPGammaTbl11		(DSP_BA+0x98)
#define	REG_DSPGammaTbl12		(DSP_BA+0x9C)
#define	REG_DSPGammaTbl13		(DSP_BA+0xA0)
#define	REG_DSPGammaTbl14		(DSP_BA+0xA4)
#define	REG_DSPGammaTbl15		(DSP_BA+0xA8)
#define	REG_DSPGammaTbl16		(DSP_BA+0xAC)

/* Auto Exposure */
#define REG_DSPAECCR1			(DSP_BA+0xB0)    /* Foreground Window */
#define REG_DSPAECAvg			(DSP_BA+0xB4)    /* AEC Statistics */

/* Auto White Balance */
#define	REG_DSPAWBWndCR			(DSP_BA+0xB8)    /* Window for AWB */
#define	REG_DSPAWBWOCR1			(DSP_BA+0xBC)    /* White object definition */
#define	REG_DSPAWBWOCR2			(DSP_BA+0xC0)    /* White object definition */
#define REG_DSPAWBCR			(DSP_BA+0xC4)    /* White object definition */
#define	REG_DSPAWBWOCount		(DSP_BA+0xC8)    /* Total White Points */
#define	REG_DSPAWBWOAvg			(DSP_BA+0xCC)    /* Average R, G, B of White Points */

/* Auto Focus */
#define	REG_DSPAFCR				(DSP_BA+0xD0)    /* AF Window Setting */
#define	REG_DSPAFreport			(DSP_BA+0xD4)    /* Auto Focus Statistics */

/* Histogram */
#define	REG_DSPHistoCR			(DSP_BA+0xD8)    /* Histogram Configuration */
#define	REG_DSPHistoReport1		(DSP_BA+0xDC)    /* Histogram Statistics -1 */
#define	REG_DSPHistoReport2		(DSP_BA+0xE0)    /* Histogram Statistics -2 */
#define	REG_DSPHistoReport3		(DSP_BA+0xE4)    /* Histogram Statistics -3 */
#define	REG_DSPHistoReport4		(DSP_BA+0xE8)    /* Histogram Statistics -4 */
#define	REG_DSPHistoReport5		(DSP_BA+0xEC)    /* Histogram Statistics -5 */
#define	REG_DSPHistoReport6 	(DSP_BA+0xF0)    /* Histogram Statistics -6 */

/* Sub-Windows */
#define REG_DSPSubWndCR1		(DSP_BA+0xF4)    /* Sub-Windows Definitions */
#define REG_DSPSubWndCR2		(DSP_BA+0xF8)    /* Sub-Windows Definitions */
#define REG_DSPSubWndAvgY1		(DSP_BA+0x100)   /* Reported values 0x100 ~ 0x13C */
#define REG_DSPSubWndAvgY2		(DSP_BA+0x104)
#define REG_DSPSubWndAvgY3		(DSP_BA+0x108)
#define REG_DSPSubWndAvgY4		(DSP_BA+0x10C)
#define REG_DSPSubWndAvgR1		(DSP_BA+0x110)
#define REG_DSPSubWndAvgR2		(DSP_BA+0x114)
#define REG_DSPSubWndAvgR3		(DSP_BA+0x118)
#define REG_DSPSubWndAvgR4		(DSP_BA+0x11C)
#define REG_DSPSubWndAvgG1		(DSP_BA+0x120)
#define REG_DSPSubWndAvgG2		(DSP_BA+0x124)
#define REG_DSPSubWndAvgG3		(DSP_BA+0x128)
#define REG_DSPSubWndAvgG4		(DSP_BA+0x12C)
#define REG_DSPSubWndAvgB1		(DSP_BA+0x130)
#define REG_DSPSubWndAvgB2		(DSP_BA+0x134)
#define REG_DSPSubWndAvgB3		(DSP_BA+0x138)
#define REG_DSPSubWndAvgB4		(DSP_BA+0x13C)

/* Bad uPixels */
#define	REG_DSPBadPixelCR		(DSP_BA+0x140)    /* Bad-Pixel Positions */
#define REG_DSPBadPixelIndex	(DSP_BA+0x144)    /* Bad-Pixel Index */
 
 
/**********************************************************************************************************
 *                                                               
 * 12. Video Capture Engine Control Registers  
 *
 **********************************************************************************************************/
#define REG_CAPEngine             (CAP_BA+0x00)   /* VPRE Engine Status */
#define REG_CAPFuncEnable         (CAP_BA+0x04)   /* Sub-Function Control */
#define REG_CAPColorMode          (CAP_BA+0x08)   /* VPRE Color Feature Register */
#define REG_CAPRotationMode       (CAP_BA+0x0C)   /* VPRE Rotation Mode Register */
#define REG_CAPInterfaceConf      (CAP_BA+0x10)   /* VPRE interface configuration */
#define REG_CAPCropWinStarPos     (CAP_BA+0x14)   /* VPRE Cropping Window Starting Position */
#define REG_CAPCropWinSize        (CAP_BA+0x18)   /* VPRE Cropping Window Size */
#define REG_CAPDownScaleFilter    (CAP_BA+0x1C)   /* Down Scale Factor Register */
#define REG_CAPPlaDownScale       (CAP_BA+0x20)   /* Planar Scale Factor Register */
#define REG_CAPlaRealSize         (CAP_BA+0x24)   /* Planar Real Image Size */
#define REG_CAPPacDownScale       (CAP_BA+0x28)   /* Packet Scale Factor Register */
#define REG_CAPPacRealSize        (CAP_BA+0x2C)   /* Packet Real Image Size */
#define REG_CAPPlaYFB0StaAddr     (CAP_BA+0x30)   /* Planar Y pipe Frame buffer 0 starting address */
#define REG_CAPPlaUFB0StaAddr     (CAP_BA+0x34)   /* Planar U pipe Frame buffer 0 starting address */
#define REG_CAPPlaVFB0StaAddr     (CAP_BA+0x38)   /* Planar V pipe Frame buffer 0 starting addres */
#define REG_CAPPacFB0StaAddr      (CAP_BA+0x3C)   /* Packet pipe Frame buffer 0 starting address */
#define REG_CAPPlaYFB1StaAddr     (CAP_BA+0x40)   /* Planar Y pipe Frame buffer 1 starting address */
#define REG_CAPPlaUFB1StaAddr     (CAP_BA+0x44)   /* Planar U pipe Frame buffer 1 starting address */
#define REG_CAPPlaVFB1StaAddr     (CAP_BA+0x48)   /* Planar V pipe Frame buffer 1 starting address */
#define REG_CAPPacFB1StaAddr      (CAP_BA+0x4C)   /* Packet pipe Frame buffer 1 starting address */
#define REG_CAPPlaMaskStaAddr     (CAP_BA+0x50)   /* Planar Overlap mask starting address */
#define REG_CAPPacMaskStaAddr     (CAP_BA+0x54)   /* Packet Overlap mask starting address */
#define REG_CAPPlaLineOffset      (CAP_BA+0x58)   /* Planar line offset value */
#define REG_CAPPacLineOffset      (CAP_BA+0x5C)   /* Packet line offset value */
#define REG_CAPFIFOThreshold      (CAP_BA+0x60)   /* FIFO threshold value */
#define	REG_LensShadingCR1		  (CAP_BA+0x64)   /* CFA type and shift value */
#define	REG_LensShadingCR2	      (CAP_BA+0x68)   /* The center position */
#define	REG_LensShadingCR3	      (CAP_BA+0x6C)   /* The coefficients for Y or R */
#define	REG_LensShadingCR4	      (CAP_BA+0x70)   /* The coefficients for U or G */
#define	REG_LensShadingCR5	      (CAP_BA+0x74)   /* The coefficients for V or B */
#define	REG_DSPVideoQuaCR1	      (CAP_BA+0x78)   /* Brightness and Contrast */
#define	REG_DSPVideoQuaCR2	      (CAP_BA+0x7C)   /* Hue and Saturation */

 
/**********************************************************************************************************
 *                                                               
 * 13. JPEG Codec Control Registers  
 *
 **********************************************************************************************************/
#define JREGNUM			(0x1BC / 4 + 1)    /* The number of JPEG registers. */

#define REG_JMCR		(JPG_BA + 0x000)   /* JPEG Mode Control Register */
#define REG_JHEADER		(JPG_BA + 0x004)   /* JPEG Encode Header Control Register */
#define REG_JITCR		(JPG_BA + 0x008)   /* JPEG Image Type Control Register */
#define REG_JPRIQC		(JPG_BA + 0x010)   /* JPEG Encode Primary Q-Table Control Register */
#define REG_JTHBQC		(JPG_BA + 0x014)   /* JPEG Encode Thumbnail Q-Table Control Register */
#define REG_JPRIWH		(JPG_BA + 0x018)   /* JPEG Encode Primary Width/Height Register */
#define REG_JTHBWH		(JPG_BA + 0x01C)   /* JPEG Encode Thumbnail Width/Height Register */
#define REG_JPRST		(JPG_BA + 0x020)   /* JPEG Encode Primary Restart Interval Register */
#define REG_JTRST		(JPG_BA + 0x024)   /* JPEG Encode Thumbnail Restart Interval Register */
#define REG_JDECWH		(JPG_BA + 0x028)   /* JPEG Decode Image Width/Height Register */
#define REG_JINTCR		(JPG_BA + 0x02C)   /* JPEG Interrupt Control and Status Register */
#define REG_JDCOLC		(JPG_BA + 0x030)   /* JPEG Decode Image Color Control Register */
#define REG_JDCOLP		(JPG_BA + 0x034)   /* JPEG Decode Image Color Pattern Register */
#define REG_JTEST		(JPG_BA + 0x040)   /* JPEG Test Mode Control Register */
#define REG_JWINDEC0	(JPG_BA + 0x044)   /* JPEG Window Decode Mode Control Register 0 */
#define REG_JWINDEC1	(JPG_BA + 0x048)   /* JPEG Window Decode Mode Control Register 1 */
#define REG_JWINDEC2	(JPG_BA + 0x04C)   /* JPEG Window Decode Mode Control Register 2 */
#define REG_JMACR		(JPG_BA + 0x050)   /* JPEG Memory Address Mode Control Registe */
#define REG_JPSCALU		(JPG_BA + 0x054)   /* JPEG Primary Scaling-Up Control Register */
#define REG_JPSCALD		(JPG_BA + 0x058)   /* JPEG Primary Scaling-Down Control Register */
#define REG_JTSCALD		(JPG_BA + 0x05C)   /* JPEG Thumbnail Scaling-Down Control Register */
#define REG_JDBCR		(JPG_BA + 0x060)   /* Dual-Buffer Control Register */
#define REG_JRESERVE	(JPG_BA + 0x070)   /* Primary Encode Bit-stream Reserved Size Register */
#define REG_JOFFSET		(JPG_BA + 0x074)   /* Address Offset Between Primary/Thumbnail Register */
#define REG_JFSTRIDE	(JPG_BA + 0x078)   /* JPEG Encode Bit-stream Frame Stride Register */
#define REG_JYADDR0		(JPG_BA + 0x07C)   /* Y Component Frame Buffer-0 Start Address Register */
#define REG_JUADDR0		(JPG_BA + 0x080)   /* U Component Frame Buffer-0 Start Address Register */
#define REG_JVADDR0		(JPG_BA + 0x084)   /* V Component Frame Buffer-0 Start Address Register */
#define REG_JYADDR1		(JPG_BA + 0x088)   /* Y Component Frame Buffer-1 Start Address Register */
#define REG_JUADDR1		(JPG_BA + 0x08C)   /* U Component Frame Buffer-1 Start Address Register */
#define REG_JVADDR1		(JPG_BA + 0x090)   /* V Component Frame Buffer-1 Start Address Register */
#define REG_JYSTRIDE	(JPG_BA + 0x094)   /* Y Component Frame Buffer Stride Register */
#define REG_JUSTRIDE	(JPG_BA + 0x098)   /* U Component Frame Buffer Stride Register */
#define REG_JVSTRIDE	(JPG_BA + 0x09C)   /* V Component Frame Buffer Stride Register */
#define REG_JIOADDR0	(JPG_BA + 0x0A0)   /* Bit-stream Frame Buffer-0 Start Address Register */
#define REG_JIOADDR1	(JPG_BA + 0x0A4)   /* Bit-stream Frame Buffer-1 Start Address Register */
#define REG_JPRI_SIZE	(JPG_BA + 0x0A8)   /* JPEG Encode Primary Bit-stream Size Register */
#define REG_JTHB_SIZE	(JPG_BA + 0x0AC)   /* JPEG Encode Thumbnail Bit-stream Size Register */
#define REG_JSRCWH		(JPG_BA + 0x0B0)   /* JPEG Encode Source Image Width/Height Register */

/* QTable 0 ~ 2 
   1. Needs to be programmed in byte unit.
   2. While reading QTAB, it must read twice to get the correct data due to the QTAB is realized by SRAM. */
#define REG_JQTAB0		(JPG_BA + 0x100) /* The start address of JPEG Quantization-Table 0. 0x100 ~ 0x13C */
#define REG_JQTAB1		(JPG_BA + 0x140) /* The start address of JPEG Quantization-Table 1. 0x140 ~ 0x17C */
#define REG_JQTAB2		(JPG_BA + 0x180) /* The start address of JPEG Quantization-Table 2. 0x180 ~ 0x1BC */
 
 
/**********************************************************************************************************
 *                                                               
 * 14. MPEG-4 Video Codec Control Registers  
 *
 **********************************************************************************************************/
/* Encoder DMA map */ 
#define	REG_MP4_INTERRUPT					(MP4_BA+0x038) 
#define	REG_MP4_HOSTIF_DELAY				(MP4_BA+0x040) 

/* Encoder DMA map */ 
#define	REG_ENC_MBF_DMA_CSR				    (MP4_BA+0x3A8)
#define	REG_ENC_MBF_DMA_THSH				(MP4_BA+0x3B0)
#define	REG_ENC_MBF_DMA_ADRS				(MP4_BA+0x3B8)

#define	REG_ENC_MOF_DMA_CSR				    (MP4_BA+0x3C8)
#define	REG_ENC_MOF_DMA_THSH				(MP4_BA+0x3D0)
#define	REG_ENC_MOF_DMA_ADRS				(MP4_BA+0x3D8)

#define	REG_ENC_MIF_DMA_CSR				    (MP4_BA+0x3E8)
#define	REG_ENC_MIF_DMA_THSH				(MP4_BA+0x3F0)
#define	REG_ENC_MIF_DMA_ADRS				(MP4_BA+0x3F8)

/* Decoder DMA map */ 
#define	REG_DEC_MBF_DMA_CSR				    (MP4_BA+0x308)
#define	REG_DEC_MBF_DMA_THSH				(MP4_BA+0x310)
#define	REG_DEC_MBF_DMA_ADRS				(MP4_BA+0x318)

#define	REG_DEC_MOF_DMA_CSR				    (MP4_BA+0x328)
#define	REG_DEC_MOF_DMA_THSH				(MP4_BA+0x330)
#define	REG_DEC_MOF_DMA_ADRS				(MP4_BA+0x338)

#define	REG_DEC_MIF_DMA_CSR				    (MP4_BA+0x348)
#define	REG_DEC_MIF_DMA_THSH				(MP4_BA+0x350)
#define	REG_DEC_MIF_DMA_ADRS				(MP4_BA+0x358)

/* Encoder register map */
#define	REG_ENCODER_CONTROL					(MP4_BA+0x400)	
#define REG_ENCODER_CONTROL_WIDTH			(MP4_BA+0x404)
#define REG_ENCODER_STATUS					(MP4_BA+0x408)	
#define REG_ENCODER_CONTROL_HEIGHT			(MP4_BA+0x40c)	

#define REG_ENCODER_CONTROL_IRQ				(MP4_BA+0x410)
#define REG_ENCODER_CONTROL_VTI				(MP4_BA+0x414)
#define REG_ENCODER_COUNT					(MP4_BA+0x418)
#define REG_ENCODER_CONTROL_FUP				(MP4_BA+0x41C)

#define REG_ENCODER_RATE_OUT				(MP4_BA+0x420)
#define REG_ENCODER_CONFIG					(MP4_BA+0x424)
#define REG_ENCODER_IDENTITY				(MP4_BA+0x428)
#define REG_ENCODER_RATE_IN					(MP4_BA+0x42C)

#define REG_ENCODER_RATE_EX					(MP4_BA+0x430)
#define REG_ENCODER_CONTROL_IO				(MP4_BA+0x434)
#define REG_ENCODER_ADR_INP_Y				(MP4_BA+0x438)
#define REG_ENCODER_ADR_INP_U				(MP4_BA+0x43C)

#define REG_ENCODER_ADR_INP_V				(MP4_BA+0x440)
#define REG_ENCODER_ADR_REF					(MP4_BA+0x444)
#define REG_ENCODER_ADR_REC					(MP4_BA+0x448)	
#define REG_ENCODER_ADR_BIT					(MP4_BA+0x44C)

#define REG_ENCODER_RATE_MAD         		(MP4_BA+0x450)
#define REG_ENCODER_CONTROL_FILTER   		(MP4_BA+0x454)
#define REG_ENCODER_DISTORTION      		(MP4_BA+0x458)
#define REG_ENCODER_QUANT_MATRIX     		(MP4_BA+0x45C)

/*  Decoder REG_ister map */
#define REG_DECODER_CONTROL		     		(MP4_BA+0x600)
#define REG_DECODER_CONTROL_WIDTH	 		(MP4_BA+0x604)
#define REG_DECODER_STATUS            		(MP4_BA+0x608)
#define REG_DECODER_CONTROL_HEIGHT    		(MP4_BA+0x60C)

#define REG_DECODER_CONTROL_IRQ		 		(MP4_BA+0x610)
#define REG_DECODER_CONTROL_IO        		(MP4_BA+0x614)
#define REG_DECODER_STATUS_VTI		 		(MP4_BA+0x618)
#define REG_DECODER_ADR_BIT           		(MP4_BA+0x61C)

#define REG_DECODER_COUNT             		(MP4_BA+0x620)
#define REG_DECODER_ADR_REF_Y         		(MP4_BA+0x624)
#define REG_DECODER_IDENTITY         		(MP4_BA+0x628)
#define REG_DECODER_ADR_REF_U        		(MP4_BA+0x62C)

#define REG_DECODER_ADR_REF_V        		(MP4_BA+0x630)
#define REG_DECODER_ADR_OUT_Y         		(MP4_BA+0x634)
#define REG_DECODER_ADR_OUT_U         		(MP4_BA+0x638)
#define REG_DECODER_ADR_OUT_V         		(MP4_BA+0x63C)

#define REG_DECODER_QUANT_MATRIX      		(MP4_BA+0x640) 
 
 
/**********************************************************************************************************
 *                                                               
 * 15. 2-D Graphic Engine Control Registers  
 *
 **********************************************************************************************************/
#define REG_GE_TRIGGER                  (GE_BA+0x000)      /* trigger control */
#define REG_GE_SRC_START00_ADDR         (GE_BA+0x004)      /* source start (0,0) of X/Y mode */
#define REG_GE_FILTER_SRC_ADDR          (GE_BA+0x004)      /* shared with CR004 */
#define REG_GE_FILTER0                  (GE_BA+0x008)      /* 3x3 filter parameter 1 */
#define REG_GE_FILTER1                  (GE_BA+0x00C)      /* 3x3 filter parameter 2 */
#define REG_GE_INTS                     (GE_BA+0x010)      /* interrupt status */
#define REG_GE_PAT_START_ADDR           (GE_BA+0x014)      /* pattern start */
#define REG_GE_ERR_TERM_STEP_CONST      (GE_BA+0x018)      /* line parameter 1 */
#define REG_GE_FILTER_THRESHOLD         (GE_BA+0x018)      /* 3x3 filter threshold for max clamping */
#define REG_GE_INIT_ERR_COUNT           (GE_BA+0x01C)      /* line parameter 2 */
#define REG_GE_CONTROL                  (GE_BA+0x020)      /* graphics engine control */
#define REG_GE_BACK_COLOR               (GE_BA+0x024)      /* background color */
#define REG_GE_FORE_COLOR               (GE_BA+0x028)      /* foreground color */
#define REG_GE_TRANSPARENCY_COLOR       (GE_BA+0x02C)      /* color key */
#define REG_GE_TRANSPARENCY_MASK        (GE_BA+0x030)      /* color key mask */
#define REG_GE_DEST_START00_ADDR        (GE_BA+0x034)      /* dest start (0,0) of X/Y mode */
#define REG_GE_FILTER_DEST_ADDR         (GE_BA+0x034)      /* shared with CR034 */
#define REG_GE_PITCH                    (GE_BA+0x038)      /* pitch of X/Y mode */
#define REG_GE_FILTER_PITCH             (GE_BA+0x038)      /* shared with CR038 */
#define REG_GE_SRC_START_ADDR           (GE_BA+0x03C)      /* source start of BitBLT */
#define REG_GE_DEST_START_ADDR          (GE_BA+0x040)      /* dest start of BitBLT */
#define REG_GE_DIMENSION                (GE_BA+0x044)      /* width/height of X/Y BitBLT */
#define REG_GE_FILTER_DIMENSION         (GE_BA+0x044)      /* shared with CR044 */
#define REG_GE_CLIP_TL                  (GE_BA+0x048)      /* clip top-left */
#define REG_GE_CLIP_BR                  (GE_BA+0x04C)      /* clip bottom-right */
#define REG_GE_PATA                     (GE_BA+0x050)      /* mono pattern 1 */
#define REG_GE_PATB                     (GE_BA+0x054)      /* mono pattern 2 */
#define REG_GE_WRITE_MASK               (GE_BA+0x058)      /* write plane mask */
#define REG_GE_MISC                     (GE_BA+0x05C)      /* misc control */
#define REG_GE_DATA_PORT0               (GE_BA+0x060)      /* HostBLT data port 0x60 ~ 0x7C */
#define REG_GE_DATA_PORT1               (GE_BA+0x064)
#define REG_GE_DATA_PORT2               (GE_BA+0x068)
#define REG_GE_DATA_PORT3               (GE_BA+0x06C)
#define REG_GE_DATA_PORT4               (GE_BA+0x070)
#define REG_GE_DATA_PORT5               (GE_BA+0x074)
#define REG_GE_DATA_PORT6               (GE_BA+0x078)
#define REG_GE_DATA_PORT7               (GE_BA+0x07C) 
 
 
 
/**********************************************************************************************************
 *                                                               
 * 16. Video Processing Engine Control Registers  
 *
 **********************************************************************************************************/
#define REG_VPEEngineTrigger		(VPE_BA+0x00)   /* Video Process Engine Trigger Control Register */
#define REG_VPEDdaFilterCC			(VPE_BA+0x04)   /* DDA 3X3 Filter Central Pixel Tap Coefficient */
#define REG_VPEDdaFilterLU			(VPE_BA+0x08)   /* DDA 3X3 Filter0, Other 4 Coefficients Around Central Pixel */
#define REG_VPEDdaFilterRB			(VPE_BA+0x0C)   /* DDA 3X3 Filter1, Other 4 Coefficients Around Central Pixel */
#define REG_VPEIntStatus			(VPE_BA+0x10)   /* Interrupt Status Register */
#define REG_VPEPacSrcStaAddr		(VPE_BA+0x14)   /* Packet Source Start Address */
#define REG_VPEUVDesSrcPitch		(VPE_BA+0x18)   /* Planar U/V Destination/Source Pitch Register */
#define REG_VPEReset				(VPE_BA+0x1C)   /* Video Process Engine Reset Control Register */
#define REG_VPECommand				(VPE_BA+0x20)   /* Video Process Engine Command Control Register */
#define REG_VPEYSrcStaAddr			(VPE_BA+0x24)   /* Video Process Engine Y Space Planar Start Address */
#define REG_VPEUSrcStaAddr			(VPE_BA+0x28)   /* Video Process Engine U Space Planar Start Address */
#define REG_VPEVSrcStaAddr			(VPE_BA+0x2C)   /* Video Process Engine V Space Planar Start Address */
#define REG_VPEDdaFactor			(VPE_BA+0x30)   /* DDA Vertical and Horizontal Scaling Factor N/M Register */
#define REG_VPEFcPacDesStaAddr		(VPE_BA+0x34)   /* Data Format Conversion Packet Destination Start Address */
#define REG_VPEDesSrcPtich			(VPE_BA+0x38)   /* Video Process Engine Destination/Source Pitch Register */
#define REG_VPEDdaPacDesStaAddr		(VPE_BA+0x3C)   /* DDA Down Scaling Packet Destination Start Address */
#define REG_VPERotRefPacDesStaAddr	(VPE_BA+0x40)   /* Rotate Reference Packet Destination Start Address */
#define REG_VPESrcHW				(VPE_BA+0x44)   /* Video Process Engine Width/Height Dimension Register */
#define REG_VPEDdaYDesStaAddr		(VPE_BA+0x48)   /* DDA Y Pipe Destination Start Address */
#define REG_VPEDdaUDesStaAddr		(VPE_BA+0x4C)   /* DDA U Pipe Destination Start Address */
#define REG_VPEDdaVDesStaAddr		(VPE_BA+0x50)   /* DDA V Pipe Destination Start Address */
#define REG_VPERotRefYDesStaAddr	(VPE_BA+0x54)   /* Rotate Reference Y Pipe Destination Start Address */
#define REG_VPERotRefUDesStaAddr	(VPE_BA+0x58)   /* Rotate Reference U Pipe Destination Start Address */
#define REG_VPERotRefVDesStaAddr	(VPE_BA+0x5C)   /* Rotate Reference V Pipe Destination Start Address */ 
 
 
/**********************************************************************************************************
 *                                                               
 * 17. Motion Estimation Engine Control Registers  
 *
 **********************************************************************************************************/
#define REG_ME_CR			(ME_BA+0x000)    /* ME Control Register */
#define REG_ME_INTCR		(ME_BA+0x004)    /* ME Interrupt Control Register */
#define REG_ME_CADDR		(ME_BA+0x008)    /* Y Component of Current Block Start Address Register */
#define REG_ME_CSTRIDE		(ME_BA+0x00C)    /* Y Component of Current Frame Buffer Stride Register */
#define REG_ME_PADDR		(ME_BA+0x010)    /* Y Component of Previous Block Start Address Register */
#define REG_ME_PSTRIDE		(ME_BA+0x014)    /* Y Component of Previous Frame Buffer Stride Register */
#define REG_ME_MV			(ME_BA+0x018)    /* Motion Vector */
#define REG_ME_MAD			(ME_BA+0x01C)    /* Mean Absolute Difference of Motion Vector */
   
 
/**********************************************************************************************************
 *                                                               
 * 18. High Speed UART Control Registers  
 *
 **********************************************************************************************************/
#define REG_HS_COM_RX     (HSU_BA+0x00)     /* (R) RX buffer */
#define REG_HS_COM_TX     (HSU_BA+0x00)     /* (W) TX buffer */
#define REG_HS_COM_IER    (HSU_BA+0x04)     /* Interrupt enable register */
#define REG_HS_COM_LSB    (HSU_BA+0x00)     /* Divisor latch LSB */
#define REG_HS_COM_MSB    (HSU_BA+0x04)     /* Divisor latch MSB */
#define REG_HS_COM_IIR    (HSU_BA+0x08)     /* (R) Interrupt ident. register */
#define REG_HS_COM_FCR    (HSU_BA+0x08)     /* (W) FIFO control register */
#define REG_HS_COM_LCR    (HSU_BA+0x0C)     /* Line control register */
#define REG_HS_COM_MCR    (HSU_BA+0x10)     /* Modem control register */
#define	REG_HS_COM_LSR    (HSU_BA+0x14)     /* (R) Line status register */
#define	REG_HS_COM_TOR    (HSU_BA+0x1C)     /* Time out register */ 
 
 
/**********************************************************************************************************
 *                                                               
 * 19. UART0 Control Registers  
 *
 **********************************************************************************************************/
#define REG_COM_TX     (UART_BA+0x0)    /* (W) TX buffer */
#define REG_COM_RX     (UART_BA+0x0)    /* (R) RX buffer */
#define REG_COM_LSB    (UART_BA+0x0)    /* Divisor latch LSB */
#define REG_COM_MSB    (UART_BA+0x04)   /* Divisor latch MSB */
#define REG_COM_IER    (UART_BA+0x04)   /* Interrupt enable register */
#define REG_COM_IIR    (UART_BA+0x08)   /* (R) Interrupt ident. register */
#define REG_COM_FCR    (UART_BA+0x08)   /* (W) FIFO control register */
#define REG_COM_LCR    (UART_BA+0x0C)   /* Line control register */
#define REG_COM_MCR    (UART_BA+0x10)   /* Modem control register */
#define	REG_COM_LSR    (UART_BA+0x14)   /* (R) Line status register */
#define REG_COM_MSR    (UART_BA+0x18)   /* (R) Modem status register */
#define	REG_COM_TOR    (UART_BA+0x1C)   /* (R) Time out register */
#define REG_COM_IRCR   (UART2_BA+0x20)   /* (R/W) IrDA control register */
 
/**********************************************************************************************************
 *                                                               
 * 20. Timer Control Registers  
 *
 **********************************************************************************************************/
#define	REG_TCSR0    (TMR_BA+0x0)    /* Control Register 0 */
#define	REG_TCSR1    (TMR_BA+0x04)   /* Control Register 1 */
#define	REG_TICR0    (TMR_BA+0x08)   /* Initial Control Register 0 */
#define	REG_TICR1    (TMR_BA+0x0C)   /* Initial Control Register 1 */
#define	REG_TDR0     (TMR_BA+0x10)   /* Data Register 0 */
#define	REG_TDR1     (TMR_BA+0x14)   /* Data Register 1 */
#define	REG_TISR     (TMR_BA+0x18)   /* Interrupt Status Register */
#define REG_WTCR     (TMR_BA+0x1C)   /* Watchdog Timer Control Register */
#define	REG_TCSR2    (TMR_BA+0x20)   /* Control Register 2 */
#define	REG_TCSR3    (TMR_BA+0x24)   /* Control Register 3 */
#define	REG_TICR2    (TMR_BA+0x28)   /* Initial Control Register 2 */
#define	REG_TICR3    (TMR_BA+0x2C)   /* Initial Control Register 3 */
#define	REG_TDR2     (TMR_BA+0x30)   /* Data Register 2 */
#define	REG_TDR3     (TMR_BA+0x34)   /* Data Register 3 */
#define	REG_TCSR4    (TMR_BA+0x40)   /* Control Register 4 */
#define	REG_TICR4    (TMR_BA+0x48)   /* Initial Control Register 4 */
#define	REG_TDR4     (TMR_BA+0x50)   /* Data Register 2 */

 
/**********************************************************************************************************
 *                                                               
 * 21. Interrupt Control Registers  
 *
 **********************************************************************************************************/
#define REG_AIC_SCR1    (AIC_BA+0x04)    /* Source control register 1 */
#define REG_AIC_SCR2    (AIC_BA+0x08)    /* Source control register 2 */
#define REG_AIC_SCR3    (AIC_BA+0x0C)    /* Source control register 3 */
#define REG_AIC_SCR4    (AIC_BA+0x10)    /* Source control register 4 */
#define REG_AIC_SCR5    (AIC_BA+0x14)    /* Source control register 5 */
#define REG_AIC_SCR6    (AIC_BA+0x18)    /* Source control register 6 */
#define REG_AIC_SCR7    (AIC_BA+0x1C)    /* Source control register 7 */
#define REG_AIC_SCR8    (AIC_BA+0x20)    /* Source control register 8 */
#define REG_AIC_SCR9    (AIC_BA+0x24)    /* Source control register 9 */
#define REG_AIC_SCR10   (AIC_BA+0x28)    /* Source control register 10 */
#define REG_AIC_SCR11   (AIC_BA+0x2C)    /* Source control register 11 */
#define REG_AIC_SCR12   (AIC_BA+0x30)    /* Source control register 12 */
#define REG_AIC_SCR13   (AIC_BA+0x34)    /* Source control register 13 */
#define REG_AIC_SCR14   (AIC_BA+0x38)    /* Source control register 14 */
#define REG_AIC_SCR15   (AIC_BA+0x3C)    /* Source control register 15 */
#define REG_AIC_SCR16   (AIC_BA+0x40)    /* Source control register 16 */
#define REG_AIC_SCR17   (AIC_BA+0x44)    /* Source control register 17 */
#define REG_AIC_SCR18   (AIC_BA+0x48)    /* Source control register 18 */
#define REG_AIC_SCR19   (AIC_BA+0x4C)    /* Source control register 19 */
#define REG_AIC_SCR20   (AIC_BA+0x50)    /* Source control register 20 */
#define REG_AIC_SCR21   (AIC_BA+0x54)    /* Source control register 21 */
#define REG_AIC_SCR22   (AIC_BA+0x58)    /* Source control register 22 */
#define REG_AIC_SCR23   (AIC_BA+0x5C)    /* Source control register 23 */
#define REG_AIC_SCR24   (AIC_BA+0x60)    /* Source control register 24 */
#define REG_AIC_SCR25   (AIC_BA+0x64)    /* Source control register 25 */
#define REG_AIC_SCR26   (AIC_BA+0x68)    /* Source control register 26 */
#define REG_AIC_SCR27   (AIC_BA+0x6C)    /* Source control register 27 */
#define REG_AIC_SCR28   (AIC_BA+0x70)    /* Source control register 28 */
#define REG_AIC_SCR29   (AIC_BA+0x74)    /* Source control register 29 */
#define REG_AIC_SCR30   (AIC_BA+0x78)    /* Source control register 30 */
#define REG_AIC_SCR31   (AIC_BA+0x7C)    /* Source control register 31 */
#define REG_AIC_IRQSC	(AIC_BA+0x80)	 /* External Interrupt Control Register */
#define REG_AIC_GEN		(AIC_BA+0x84)	 /* Interrupt Group Enable Control Register */
#define REG_AIC_GRSR	(AIC_BA+0x88)	 /* Interrupt Group Raw Status Register */
#define REG_AIC_IRSR    (AIC_BA+0x100)   /* Interrupt raw status register */
#define REG_AIC_IASR    (AIC_BA+0x104)   /* Interrupt active status register */
#define REG_AIC_ISR     (AIC_BA+0x108)   /* Interrupt status register */
#define REG_AIC_IPER    (AIC_BA+0x10C)   /* Interrupt priority encoding register */
#define REG_AIC_ISNR    (AIC_BA+0x110)   /* Interrupt source number register */
#define REG_AIC_IMR     (AIC_BA+0x114)   /* Interrupt mask register */
#define REG_AIC_OISR    (AIC_BA+0x118)   /* Output interrupt status register */
#define REG_AIC_MECR    (AIC_BA+0x120)   /* Mask enable command register */
#define REG_AIC_MDCR    (AIC_BA+0x124)   /* Mask disable command register */
#define REG_AIC_SSCR    (AIC_BA+0x128)   /* Source set command register */
#define REG_AIC_SCCR    (AIC_BA+0x12C)   /* Source clear command register */
#define REG_AIC_EOSCR   (AIC_BA+0x130)   /* End of service command register */
#define REG_AIC_TEST    (AIC_BA+0x200)   /* ICE/Debug mode register */

  
/**********************************************************************************************************
 *                                                               
 * 22. Universal Serial Interface Control Registers  
 *
 **********************************************************************************************************/
#define	REG_USI_CNTRL		(USI_BA+0x0)     /* Control and Status Register */
#define	REG_USI_DIVIDER		(USI_BA+0x04)    /* Clock Divider Register */
#define	REG_USI_SSR			(USI_BA+0x08)    /* Slave Select Register */
#define	REG_USI_Rx0			(USI_BA+0x10)    /* Data Receive Register 0 */
#define	REG_USI_Rx1			(USI_BA+0x14)    /* Data Receive Register 1 */
#define	REG_USI_Rx2			(USI_BA+0x18)    /* Data Receive Register 2 */
#define	REG_USI_Rx3			(USI_BA+0x1C)    /* Data Receive Register 3 */
#define	REG_USI_Tx0			(USI_BA+0x10)    /* Data Transmit Register 0 */
#define	REG_USI_Tx1			(USI_BA+0x14)    /* Data Transmit Register 1 */
#define	REG_USI_Tx2			(USI_BA+0x18)    /* Data Transmit Register 2 */
#define	REG_USI_Tx3			(USI_BA+0x1C)    /* Data Transmit Register 3 */
 
 
/**********************************************************************************************************
 *                                                               
 * 23. External Bus Interface Control Registers  
 *
 **********************************************************************************************************/
#define	REG_EBISIZE0	(EBI_BA + 0x00)    /* EBI Bank 0 Size Control Register */
#define	REG_EBISIZE1	(EBI_BA + 0x04)    /* EBI Bank 1 Size Control Register */
#define	REG_EBISIZE2	(EBI_BA + 0x08)    /* EBI Bank 2 Size Control Register */
#define	REG_EBISIZE3	(EBI_BA + 0x0C)    /* EBI Bank 3 Size Control Register */
#define	REG_EBITIM0		(EBI_BA + 0x10)    /* EBI Bank0 Timing Control Register */
#define	REG_EBITIM1		(EBI_BA + 0x14)    /* EBI Bank1 Timing Control Register */ 
#define	REG_EBITIM2		(EBI_BA + 0x18)    /* EBI Bank2 Timing Control Register */ 
#define	REG_EBITIM3		(EBI_BA + 0x1C)    /* EBI Bank3 Timing Control Register */ 

 
#endif /* _NUC900_CPU_H */