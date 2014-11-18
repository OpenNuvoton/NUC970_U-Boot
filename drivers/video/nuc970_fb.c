/*
 * Copyright (c) 2014 Nuvoton Technology Corp.
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
 * Description:   NUC970 Video driver source file
 */
#include <common.h>
#include <asm/io.h>
#include <lcd.h>
#include "nuc970_fb.h" 

#define REG_HCLKEN      0xB0000210
#define REG_CLKDIVCTL0  0xB0000220
#define REG_CLKDIVCTL1  0xB0000224
#define REG_CLKUPLLCON  0xB0000264
#define REG_MFP_GPA_L   0xB0000070
#define REG_MFP_GPA_H   0xB0000074
#define REG_MFP_GPD_H   0xB000008C
#define REG_MFP_GPG_L   0xB00000A0
#define REG_MFP_GPG_H   0xB00000A4

vpost_cfg_t vpost_cfg = {
        .clk            = 5000000,
        .hight          = 240,
        .width          = 320,
        .left_margin    = 4,
        .right_margin   = 12,
        .hsync_len      = 2,
        .upper_margin   = 2,
        .lower_margin   = 2,
        .vsync_len      = 1,
        .dccs           = 0x0e00040a,//0x0e00041a,
        .devctl         = 0x060800c0,
        .fbctrl         = 0x00a000a0,
        .scale          = 0x04000400,                
};



vidinfo_t panel_info = {
        .vl_col         = 320,
        .vl_row         = 240,
        .vl_bpix        = 4,  // 2^4 = 16bpp
};

//int lcd_line_length;  
int lcd_color_fg;
int lcd_color_bg;

void *lcd_base;                 /* Start of framebuffer memory  */
void *lcd_console_address;              /* Start of console buffer      */

short console_col;
short console_row;


void lcd_enable(void)
{
        // Turn on back light
        writel(readl(REG_GPIOH_DOUT) | (1 << 2), REG_GPIOH_DOUT);
        return;          
}

void lcd_disable(void)
{
        // Turn off back light
        writel(readl(REG_GPIOH_DOUT) & ~(1 << 2), REG_GPIOH_DOUT);
        return;         
}

void lcd_ctrl_init(void *lcdbase)
{
        int volatile i;
        int div;

        // VPOST clk
        writel(readl(REG_HCLKEN) | 0x02000000, REG_HCLKEN); // LCD 
        div = (CONFIG_EXT_CLK / vpost_cfg.clk) - 1;
        if(div < 0)
                div = 0;
        if(div > 0xff)
                div = 0xff;

	div = 9; //CWWeng test
	//printf("[%s] div = 0x%x\n",__FUNCTION__,div);

        //writel((readl(REG_CLKDIVCTL1) & ~0x18), REG_CLKDIVCTL1); // Set VPOST clock source from XIN
        //writel((readl(REG_CLKDIVCTL1) & ~0x18) | (2 << 3), REG_CLKDIVCTL1); // Set VPOST clock source from ACLKOUT
        writel((readl(REG_CLKDIVCTL1) & ~0x18) | (3 << 3), REG_CLKDIVCTL1); // Set VPOST clock source from UCLKOUT
        writel((readl(REG_CLKDIVCTL1) & ~0xFF00) | (div << 8), REG_CLKDIVCTL1);

        // GPIO
        writel(0x22222222, REG_MFP_GPA_L); // LCD_DATA0~7
        writel(0x22222222, REG_MFP_GPA_H); // LCD_DATA8~15
        writel(0x22222222, REG_MFP_GPD_H); // LCD_DATA16~23
        writel((readl(REG_MFP_GPG_L) & ~0xFF000000) | 0x22000000, REG_MFP_GPG_L); // LCD_CLK LCD_HSYNC
        writel((readl(REG_MFP_GPG_H) & ~0xFF) | 0x22, REG_MFP_GPG_H); // LCD_VSYNC LCD_DEN

        writel(readl(REG_GPIOH_DIR) | (1 << 2), REG_GPIOH_DIR);
        writel(readl(REG_GPIOH_DOUT) | (1 << 2), REG_GPIOH_DOUT);
        
        // Configure VPOST registers.               
        writel(0, REG_LCM_DCCS);  // Stop LCD
        
        writel(readl(REG_LCM_DCCS) | LCM_DCCS_ENG_RST, REG_LCM_DCCS);  // Reset LCD
        for(i = 0; i < 0x1000; i++);
        writel(readl(REG_LCM_DCCS) & (~LCM_DCCS_ENG_RST),REG_LCM_DCCS);
        for(i = 0; i < 0x1000; i++);

        writel(0, REG_LCM_DEV_CTRL);
        
        /* set frambuffer start phy addr*/
        writel((unsigned int)lcdbase, REG_LCM_VA_BADDR0);
        //writel((unsigned int)lcdbase, REG_LCM_VA_BADDR1);

        writel(vpost_cfg.fbctrl, REG_LCM_VA_FBCTRL);
        writel(vpost_cfg.scale, REG_LCM_VA_SCALE);        
        
        writel(vpost_cfg.devctl, REG_LCM_DEV_CTRL);
        writel(LCM_CRTC_SIZE_VTTVAL(vpost_cfg.hight + vpost_cfg.upper_margin + vpost_cfg.lower_margin) | 
               LCM_CRTC_SIZE_HTTVAL(vpost_cfg.width + vpost_cfg.left_margin + vpost_cfg.right_margin), REG_LCM_CRTC_SIZE);
        writel(LCM_CRTC_DEND_VDENDVAL(vpost_cfg.hight) | LCM_CRTC_DEND_HDENDVAL(vpost_cfg.width), REG_LCM_CRTC_DEND);
        writel(LCM_CRTC_HR_EVAL(vpost_cfg.width + 5) | LCM_CRTC_HR_SVAL(vpost_cfg.width + 1), REG_LCM_CRTC_HR);
        writel(LCM_CRTC_HSYNC_EVAL(vpost_cfg.width + vpost_cfg.right_margin + vpost_cfg.hsync_len) |
               LCM_CRTC_HSYNC_SVAL(vpost_cfg.width + vpost_cfg.right_margin), REG_LCM_CRTC_HSYNC);
        writel(LCM_CRTC_VR_EVAL(vpost_cfg.hight + vpost_cfg.lower_margin + vpost_cfg.vsync_len) |
               LCM_CRTC_VR_SVAL(vpost_cfg.hight + vpost_cfg.lower_margin), REG_LCM_CRTC_VR);


        writel(vpost_cfg.dccs, REG_LCM_DCCS);       
        

	//printf("REG_CLKDIVCTL0 = 0x%x\n",readl(REG_CLKDIVCTL0));
	//printf("REG_CLKDIVCTL1 = 0x%x\n",readl(REG_CLKDIVCTL1));
	//printf("REG_CLKUPLLCON = 0x%x\n",readl(REG_CLKUPLLCON));
        

        return;
}


void lcd_getcolreg (ushort regno, ushort *red, ushort *green, ushort *blue)
{
        return;
}
#ifdef CONFIG_LCD_INFO
#include <version.h>
void lcd_show_board_info(void)
{
        lcd_printf ("%s\n", U_BOOT_VERSION);
        lcd_printf ("(C) 2014 Nuvoton Technology Corp.\n");
        lcd_printf ("NUC970 Evaluation Board\n");

}
#endif /* CONFIG_LCD_INFO */

