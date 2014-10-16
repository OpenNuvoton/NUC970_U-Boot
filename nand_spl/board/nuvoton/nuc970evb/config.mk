#
# (C) Copyright 2006
# Stefan Roese, DENX Software Engineering, sr@denx.de.
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
#
# Nuvoton nuc9xx Reference Platform (nuc900) board

# CONFIG_SYS_TEXT_BASE for SPL:
#
# On nuc900 platforms the SPL is located in SRAM at 0x200.
#
# CONFIG_SYS_TEXT_BASE = 0x200

include $(TOPDIR)/board/$(BOARDDIR)/config.mk

# PAD_TO used to generate a 8kByte binary needed for the combined image
# -> PAD_TO = CONFIG_SYS_NAND_U_BOOT_OFFS
#PAD_TO	:= $(shell expr $$[$(CONFIG_SYS_TEXT_BASE) + 4096])
PAD_TO	:= $(shell expr $$[$(CONFIG_SYS_NAND_U_BOOT_OFFS) + $(CONFIG_SYS_TEXT_BASE) - 16 ])

ifeq ($(debug),1)
PLATFORM_CPPFLAGS += -DDEBUG
endif
