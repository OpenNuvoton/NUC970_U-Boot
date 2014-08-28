/*
 * cmd_decrypt.c - decrypt image
 *
 * Copyright (c) 2014 Nuvoton technology
 *
 */

#include <common.h>
#include <command.h>

#include <asm/io.h>
#include "register.h"

#define USE_OTP_KEY

#define Polling_Loop 0x100000

/* Per the 4 boot mode,rearrange the sequence of key.dat of AES encrypt tool */
u32 otp_key[4][8] = {
{0x43521234, 0x87901ac4, 0x80ae4ca1, 0x9087c3c4, 0x87a12c67, 0xa185f7d4, 0xc5a12f56, 0x98115678},
{0xc5a12f56, 0x98115678, 0x43521234, 0x87901ac4, 0x80ae4ca1, 0x9087c3c4, 0x87a12c67, 0xa185f7d4},
{0x87a12c67, 0xa185f7d4, 0xc5a12f56, 0x98115678, 0x43521234, 0x87901ac4, 0x80ae4ca1, 0x9087c3c4},
{0x80ae4ca1, 0x9087c3c4, 0x87a12c67, 0xa185f7d4, 0xc5a12f56, 0x98115678, 0x43521234, 0x87901ac4}
};

static void SetTimer(unsigned int count)
{
        /* Set timer0 */
        writel(readl(REG_PCLKEN0) | 0x100, REG_PCLKEN0); /* enable timer engine clock */
        writel(0x1, REG_TMR_TISR);
        writel(count, REG_TMR_TICR0);  /* set timer init counter value */
        writel(0xC000000B, REG_TMR_TCSR0); /* one-shot mode, prescale = 12 */
}

#ifdef USE_OTP_KEY
static int dump_OTP_Key(void)
{
	printf("Key 0 : 0x%x\n",readl(0xB800C060));
	printf("Key 1 : 0x%x\n",readl(0xB800C064));
	printf("Key 2 : 0x%x\n",readl(0xB800C068));
	printf("Key 3 : 0x%x\n",readl(0xB800C06C));
	printf("Key 4 : 0x%x\n",readl(0xB800C070));
	printf("Key 5 : 0x%x\n",readl(0xB800C074));
	printf("Key 6 : 0x%x\n",readl(0xB800C078));
	printf("Key 7 : 0x%x\n",readl(0xB800C07C));

	return 0;
}

static int otp_init(void)
{
	int volatile loop;
	unsigned int volatile reg;
	
	writel(readl(0xB0000004) | 0x10000, 0xB0000004);
	printf("Unlock OTP...\n");
	writel(readl(REG_PCLKEN1) | (1 << 26), REG_PCLKEN1);
	writel(0x59, REG_OTP_PROTECT);
	writel(0x16, REG_OTP_PROTECT);
	writel(0x88, REG_OTP_PROTECT);

	if (readl(REG_OTP_PROTECT) == 0x1)
	{
		printf("OTP is unlocked...\n");
		writel(readl(REG_OTP_KEYEN) | 0x1, REG_OTP_KEYEN);
		for (loop = 0; loop < 0x100000; loop++)
		{
			/* check key valid(STATUS) */
			reg = readl(REG_OTP_STATUS);
			if (reg & 1)
			{
				if ( ((reg >> 16) & 0xf) == 0)
				{
					if (reg & 4)
					{
						printf("OTP enabled, No Key programmed\n");
						return 1;
					}
				}
				else if (reg & 2)
				{
					printf("OTP enabled, and key valid\n");
					dump_OTP_Key();
					return 0;
				}
			}
		}
	}

	printf("OTP enable failed, OTP status is 0x%x\n",reg);
	return -1;
}
#endif

static int do_decrypt_aes(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	u32 src, dst, len;
	u32 volatile reg;
#ifdef USE_OTP_KEY
	int ret;
#endif

        if (argc < 3)
                return CMD_RET_USAGE;

#ifdef USE_OTP_KEY
	if ((ret = otp_init()) == 0)
	{	
		printf("Init crypto engine...\n");

		/* initial CRYPTO engine */
		writel(readl(REG_HCLKEN) | 0x800000, REG_HCLKEN);// enable CRYPTO engine clock 
		writel(0x3, REG_CRPT_IPSEC_INT_EN);
		writel(0, REG_CRPT_AES_IV0);
		writel(0, REG_CRPT_AES_IV1);
		writel(0, REG_CRPT_AES_IV2);
		writel(0, REG_CRPT_AES_IV3);

		writel(0x00C000D9, REG_CRPT_AES_CTL);
	}
	else if (ret == 1)
	{
		printf("No key programmed, pls program key to OTP\n");
		return 1;
	}
#else
	printf("Init crypto engine...\n");

	/* initial CRYPTO engine */
	writel(readl(REG_HCLKEN) | 0x800000, REG_HCLKEN);// enable CRYPTO engine clock 
	writel(0x3, REG_CRPT_IPSEC_INT_EN);
	writel(0, REG_CRPT_AES_IV0);
	writel(0, REG_CRPT_AES_IV1);
	writel(0, REG_CRPT_AES_IV2);
	writel(0, REG_CRPT_AES_IV3);

	/* use internal key */
	writel(otp_key[0][0], REG_CRPT_AES_KEY0);
	writel(otp_key[0][1], REG_CRPT_AES_KEY1);
	writel(otp_key[0][2], REG_CRPT_AES_KEY2);
	writel(otp_key[0][3], REG_CRPT_AES_KEY3);
	writel(otp_key[0][4], REG_CRPT_AES_KEY4);
	writel(otp_key[0][5], REG_CRPT_AES_KEY5);
	writel(otp_key[0][6], REG_CRPT_AES_KEY6);
	writel(otp_key[0][7], REG_CRPT_AES_KEY7);

	writel(0x00C000C9, REG_CRPT_AES_CTL);//internal key
#endif

	src = simple_strtoul(argv[1], NULL, 0);
	dst = simple_strtoul(argv[2], NULL, 0);
	len = simple_strtoul(argv[3], NULL, 0);

	printf("decrypt source address is 0x%x\n",src);
	printf("decrypt destination address is 0x%x\n",dst);
	printf("decrypt length is %d\n",len);

	writel(src, REG_CRPT_AES_SADR);
	writel(dst, REG_CRPT_AES_DADR);
	writel(len, REG_CRPT_AES_CNT);

	/* clear interrupt flag */
	writel(0x3, REG_CRPT_IPSEC_INT_FLAG);

#ifdef USE_OTP_KEY
	/* SECURE_AES_CTL_START | SECURE_AES_CTL_EXT_KEY | SECURE_AES_CTL_DMA_EN | SECURE_AES_CTL_DMA_CASCADE */
	writel(readl(REG_CRPT_AES_CTL) | 0xd1, REG_CRPT_AES_CTL);
#else
	/*SECURE_AES_CTL_START|SECURE_AES_CTL_DMA_EN|SECURE_AES_CTL_DMA_CASCADE*/
	writel(readl(REG_CRPT_AES_CTL) | 0xc1, REG_CRPT_AES_CTL);
#endif

	SetTimer(1000);
	while(!(reg = readl(REG_CRPT_IPSEC_INT_FLAG) & 0x3))
	{
		if (readl(REG_TMR_TISR) & 0x1)
		{
			writel(0x1, REG_TMR_TISR);
			printf("Decrpt Timeout\n");
			break;
		}
	}
	printf("REG_CRPT_IPSEC_INT_FLAG = 0x%x\n",reg);
	writel(0x1, REG_CRPT_IPSEC_INT_FLAG);

	printf("Dump AES register\n");
	reg = readl(REG_CRPT_AES_CTL);
	printf("CTL : 0x%x\n",reg);
	reg = readl(REG_CRPT_AES_IV0);
	printf("IV0 : 0x%x\n",reg);
	reg = readl(REG_CRPT_AES_IV1);
	printf("IV1 : 0x%x\n",reg);
	reg = readl(REG_CRPT_AES_IV2);
	printf("IV2 : 0x%x\n",reg);
	reg = readl(REG_CRPT_AES_IV3);
	printf("IV3 : 0x%x\n",reg);
#ifndef USE_OTP_KEY
	reg = readl(REG_CRPT_AES_KEY0);
	printf("KEY0 : 0x%x\n",reg);
	reg = readl(REG_CRPT_AES_KEY1);
	printf("KEY1 : 0x%x\n",reg);
	reg = readl(REG_CRPT_AES_KEY2);
	printf("KEY2 : 0x%x\n",reg);
	reg = readl(REG_CRPT_AES_KEY3);
	printf("KEY3 : 0x%x\n",reg);
	reg = readl(REG_CRPT_AES_KEY4);
	printf("KEY4 : 0x%x\n",reg);
	reg = readl(REG_CRPT_AES_KEY5);
	printf("KEY5 : 0x%x\n",reg);
	reg = readl(REG_CRPT_AES_KEY6);
	printf("KEY6 : 0x%x\n",reg);
	reg = readl(REG_CRPT_AES_KEY7);
	printf("KEY7 : 0x%x\n",reg);
#endif
	reg = readl(REG_CRPT_AES_SADR);
	printf("SADR : 0x%x\n",reg);
	reg = readl(REG_CRPT_AES_DADR);
	printf("DADR : 0x%x\n",reg);
	reg = readl(REG_CRPT_AES_CNT);
	printf("CNT : 0x%x\n",reg);

	return 0;
}

#ifdef USE_OTP_KEY
static int program_otp(u32 key[8]) 
{
	int  loop;
	u32 volatile reg;

	//Set to program mode
	writel((readl(REG_OTP_MODE) & 0x3) | 0x2, REG_OTP_MODE);
	writel(0x60AE, REG_OTP_CYCLE);

	writel(key[0], REG_OTP_VALUE0);
	writel(key[1], REG_OTP_VALUE1);
	writel(key[2], REG_OTP_VALUE2);
	writel(key[3], REG_OTP_VALUE3);
	writel(key[4], REG_OTP_VALUE4);
	writel(key[5], REG_OTP_VALUE5);
	writel(key[6], REG_OTP_VALUE6);
	writel(key[7], REG_OTP_VALUE7);

	writel(0xa3, REG_OTP_OPTION);
        
	writel(0x1, REG_OTP_START);
        
        for (loop = 0; loop < Polling_Loop; loop++)
        {
                if (readl(REG_OTP_START) == 0)
                        break;
        }
        if (loop >= Polling_Loop)
        {
                printf("OTP_START not cleared!\n");
                return -1;
        }

	if ((reg = readl(REG_OTP_STATUS)) & 0x10) //Program fail
        {
                printf("OTP key program failed! [0x%x]\n", reg);
                return -1;
        }
        
        printf("OPT key program OK, COUNT = %d\n",(reg >> 16) & 0xf);
	return 0;
}
#endif

static int do_decrypt_program(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#ifdef USE_OTP_KEY
	volatile int boot_mode;
#endif

        if (argc < 1)
                return CMD_RET_USAGE;

#ifdef USE_OTP_KEY
	if (otp_init() < 0)
		return -1;

	boot_mode = readl(0xB0000004) & 0x3;
	printf("Boot mode is %d\n", boot_mode);

	printf("Program key to OTP\n");

	if (program_otp(otp_key[boot_mode]) < 0)
		printf("Program OTP Failed!\n");
	else
	{
		printf("Program OTP Successful!\n");
		dump_OTP_Key(); 
	}
#else
	printf("USE_OTP_KEY not support!\n");
#endif
	return 0;
}

static cmd_tbl_t cmd_decrypt_sub[] ={
U_BOOT_CMD_MKENT(aes, CONFIG_SYS_MAXARGS, 1, do_decrypt_aes, "", ""),
U_BOOT_CMD_MKENT(program, CONFIG_SYS_MAXARGS, 1, do_decrypt_program, "", ""),
};

static char decrypt_help_text[]=
"decrypt aes SrcAddr DstAddr Length - Decrypt the image from SrcAddr to DstAddr with lenth [Length].\n"
"decrypt program - program key to OTP.\n"
"Example : decrypt aes 0x8000 0x10000 0x200- decrypt the image from 0x8000 to 0x10000 and lenth is 0x200\n";

int do_decrypt(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

        cmd_tbl_t *cp;

	printf("Hello! Decrypt kernel start!\n");


        /* drop initial "env" arg */
        argc--;
        argv++;

        cp = find_cmd_tbl(argv[0], cmd_decrypt_sub, ARRAY_SIZE(cmd_decrypt_sub));

        if (cp)
                return cp->cmd(cmdtp, flag, argc, argv);

        return CMD_RET_USAGE;
}

U_BOOT_CMD(decrypt, 5, 1, do_decrypt,
	"Decrypt image(kernel)",
	decrypt_help_text);
