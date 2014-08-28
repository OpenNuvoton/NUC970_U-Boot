/*
 * cmd_decrypt.c - decrypt image
 *
 * Copyright (c) 2013 Nuvoton technology
 *
 */

#include <common.h>
#include <command.h>

static int do_decrypt_src(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	printf("decrypt source address is 0x%s\n",argv[1]);
	return 0;
}

static int do_decrypt_dest(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	printf("decrypt destination address is 0x%s\n",argv[1]);
	return 0;
}

static cmd_tbl_t cmd_decrypt_sub[] ={
U_BOOT_CMD_MKENT(src, CONFIG_SYS_MAXARGS, 1, do_decrypt_src, "", ""),
U_BOOT_CMD_MKENT(dest, CONFIG_SYS_MAXARGS, 1, do_decrypt_dest, "", ""),
};

static char decrypt_help_text[]=
"src addr - assign the source address of the image to be decript.\n"
"decrypt dest addr - assign the destination address of the decrypted image.\n"
"Example : decrypt dest 0x8000 - decrypt the image to address 0x8000\n";

int do_decrypt(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

        cmd_tbl_t *cp;

	printf("Hello! Decrypt kernel start!\n");

        if (argc < 3)
                return CMD_RET_USAGE;

        /* drop initial "env" arg */
        argc--;
        argv++;

        cp = find_cmd_tbl(argv[0], cmd_decrypt_sub, ARRAY_SIZE(cmd_decrypt_sub));

        if (cp)
                return cp->cmd(cmdtp, flag, argc, argv);

        return CMD_RET_USAGE;
}

U_BOOT_CMD(decrypt, do_decrypt, 1, do_decrypt,
	"Decrypt image(kernel)",
	decrypt_help_text);
