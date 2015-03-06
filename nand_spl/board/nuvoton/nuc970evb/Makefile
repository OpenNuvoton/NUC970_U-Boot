#
# (C) Copyright 2006-2007
# Stefan Roese, DENX Software Engineering, sr@denx.de.
#
# (C) Copyright 2008
# Guennadi Liakhovetki, DENX Software Engineering, <lg@denx.de>
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

CONFIG_NAND_SPL	= y
#CONFIG_SPL_BUILD = y


include $(TOPDIR)/config.mk
include $(TOPDIR)/nand_spl/board/$(BOARDDIR)/config.mk

nandobj	:= $(OBJTREE)/nand_spl/

LDSCRIPT= $(TOPDIR)/nand_spl/board/$(BOARDDIR)/u-boot.lds
LDFLAGS := -T $(nandobj)u-boot.lds -Ttext $(CONFIG_SYS_TEXT_BASE) $(LDFLAGS) \
		$(LDFLAGS_FINAL) -gc-sections 
AFLAGS	+= -DCONFIG_NAND_SPL
CFLAGS	+= -DCONFIG_NAND_SPL -ffunction-sections

SOBJS	= start.o cpu_init.o lowlevel_init.o divlib.o _lshrdi3.o _ashldi3.o _ashrdi3.o
#COBJS	= nand_boot.o nand_ecc.o  nuc970_nand.o nuc970_nand_spl.o nand.o nand_bbt.o nand_ids.o nand_util.o nand_base.o serial_nuc970.o nuc900_sysprintf.o dlmalloc.o timer.o time.o div64.o string.o vsprintf.o
COBJS	= nand_boot.o nand_ecc.o  nuc970_nand.o nuc970_nand_spl.o nand.o nand_bbt.o nand_ids.o nand_util.o nand_base.o serial_nuc970.o serial.o nuc970_sysprintf.o nuc970_wdt.o timer.o time.o div64.o string.o vsprintf.o stdio.o console.o reset.o interrupts.o nuc970_reset.o ctype.o mtdcore.o mtdpart.o
SRCS	:= $(addprefix $(obj),$(SOBJS:.o=.S) $(COBJS:.o=.c))
OBJS	:= $(addprefix $(obj),$(SOBJS) $(COBJS))
__OBJS	:= $(SOBJS) $(COBJS)
LNDIR	:= $(nandobj)board/$(BOARDDIR)

ALL	= $(nandobj)u-boot-spl $(nandobj)u-boot-spl.bin $(nandobj)u-boot-spl-16k.bin

all:	$(obj).depend $(ALL)

$(nandobj)u-boot-spl-16k.bin: $(nandobj)u-boot-spl
	$(OBJCOPY) ${OBJCFLAGS} --pad-to=$(PAD_TO) -O binary $< $@

$(nandobj)u-boot-spl.bin:	$(nandobj)u-boot-spl
	$(OBJCOPY) ${OBJCFLAGS} -O binary $< $@

$(nandobj)u-boot-spl:	$(OBJS) $(nandobj)u-boot.lds
	cd $(LNDIR) && $(LD) $(LDFLAGS) $(__OBJS) \
		-Map $(nandobj)u-boot-spl.map \
		-o $(nandobj)u-boot-spl

$(nandobj)u-boot.lds: $(LDSCRIPT)
	$(CPP) $(CPPFLAGS) $(LDPPFLAGS) -ansi -D__ASSEMBLY__ -P - <$^ >$@

# create symbolic links for common files

# from cpu directory
$(obj)start.S:
	@rm -f $@
	@ln -s $(TOPDIR)/nand_spl/board/nuvoton/nuc970evb/start_nand_spl.S $@
#	@ln -s $(TOPDIR)/arch/arm/cpu/arm926ejs/start.S $@


# from SoC directory


# from board directory
$(obj)lowlevel_init.S:
	@rm -f $@
	@ln -s $(TOPDIR)/arch/arm/cpu/arm926ejs/nuc970/lowlevel_init.S $@

# from nand_spl directory
$(obj)cpu_init.S:
	@rm -f $@
	@ln -s $(TOPDIR)/nand_spl/board/nuvoton/nuc970evb/cpu_init.S $@
	
$(obj)divlib.S:
	@rm -f $@
	@ln -s $(TOPDIR)/nand_spl/board/nuvoton/nuc970evb/divlib.S $@
	
$(obj)nand_boot.c:
	@rm -f $@
	@ln -s $(TOPDIR)/nand_spl/board/nuvoton/nuc970evb/nand_boot.c $@
	
$(obj)nuc970_nand_spl.c:
	@rm -f $@
	@ln -s $(TOPDIR)/nand_spl/board/nuvoton/nuc970evb/nuc970_nand_spl.c $@	
	
$(obj)nuc970_sysprintf.c:
	@rm -f $@
	@ln -s $(TOPDIR)/nand_spl/board/nuvoton/nuc970evb/nuc970_sysprintf.c $@		
	
# from drivers/serial directory	
$(obj)serial_nuc970.c:
	@rm -f $@
	@ln -s $(TOPDIR)/drivers/serial/serial_nuc970.c $@	
	
$(obj)serial.c:
	@rm -f $@
	@ln -s $(TOPDIR)/drivers/serial/serial.c $@	
	
# from drivers/mtd/nand directory
$(obj)nand_ecc.c:
	@rm -f $@
	@ln -s $(TOPDIR)/drivers/mtd/nand/nand_ecc.c $@

# CWWeng add begin
$(obj)nuc970_nand.c:
	@rm -f $@
	@ln -s $(TOPDIR)/nand_spl/board/nuvoton/nuc970evb/nuc970_nand.c $@

$(obj)nand.c:
	@rm -f $@
	@ln -s $(TOPDIR)/nand_spl/board/nuvoton/nuc970evb/nand.c $@

$(obj)nand_bbt.c:
	@rm -f $@
	@ln -s $(TOPDIR)/nand_spl/board/nuvoton/nuc970evb/nand_bbt.c $@

$(obj)nand_ids.c:
	@rm -f $@
	@ln -s $(TOPDIR)/nand_spl/board/nuvoton/nuc970evb/nand_ids.c $@

$(obj)nand_util.c:
	@rm -f $@
	@ln -s $(TOPDIR)/nand_spl/board/nuvoton/nuc970evb/nand_util.c $@
# CWWeng add end

$(obj)nand_base.c:
	@rm -f $@
	@ln -s $(TOPDIR)/nand_spl/board/nuvoton/nuc970evb/nand_base.c $@

$(obj)mtdcore.c:
	@rm -f $@
	@ln -s $(TOPDIR)/drivers/mtd/mtdcore.c $@
$(obj)mtdpart.c:
	@rm -f $@
	@ln -s $(TOPDIR)/drivers/mtd/mtdpart.c $@

# CWWeng add for malloc-related function
#$(obj)dlmalloc.c:
#	@rm -f $@
#	@ln -s $(TOPDIR)/common/dlmalloc.c $@

# CWWeng add for time-related function
$(obj)nuc970_wdt.c:
	@rm -f $@
	@ln -s $(TOPDIR)/drivers/watchdog/nuc970_wdt.c $@

$(obj)timer.c:
	@rm -f $@
	@ln -s $(TOPDIR)/board/nuvoton/nuc970evb/timer.c $@

$(obj)time.c:
	@rm -f $@
	@ln -s $(TOPDIR)/lib/time.c $@

$(obj)div64.c:
	@rm -f $@
	@ln -s $(TOPDIR)/lib/div64.c $@

# CWWeng add for memcpy and memset function
$(obj)string.c:
	@rm -f $@
	@ln -s $(TOPDIR)/lib/string.c $@

# CWWeng add for __aeabi_llsr and __aeabi_lasr function
$(obj)_lshrdi3.S:
	@rm -f $@
	@ln -s $(TOPDIR)/arch/arm/lib/_lshrdi3.S $@

$(obj)_ashldi3.S:
	@rm -f $@
	@ln -s $(TOPDIR)/arch/arm/lib/_ashldi3.S $@

$(obj)_ashrdi3.S:
	@rm -f $@
	@ln -s $(TOPDIR)/arch/arm/lib/_ashrdi3.S $@

# CWWeng add for panic function
$(obj)vsprintf.c:
	@rm -f $@
	@ln -s $(TOPDIR)/lib/vsprintf.c $@

$(obj)stdio.c:
	@rm -f $@
	@ln -s $(TOPDIR)/common/stdio.c $@

$(obj)console.c:
	@rm -f $@
	@ln -s $(TOPDIR)/common/console.c $@

$(obj)ctype.c:
	@rm -f $@
	@ln -s $(TOPDIR)/lib/ctype.c $@

# CWWeng add for do_reset function
$(obj)reset.c:
	@rm -f $@
	@ln -s $(TOPDIR)/arch/arm/lib/reset.c $@

$(obj)interrupts.c:
	@rm -f $@
	@ln -s $(TOPDIR)/arch/arm/lib/interrupts.c $@

$(obj)nuc970_reset.c:
	@rm -f $@
	@ln -s $(TOPDIR)/arch/arm/cpu/arm926ejs/nuc970/reset.c $@

#########################################################################

$(obj)%.o:	$(obj)%.S
	$(CC) $(AFLAGS) -c -o $@ $<

$(obj)%.o:	$(obj)%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
