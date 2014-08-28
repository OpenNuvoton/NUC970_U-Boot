/*
 *  Copyright (c) 2012 Nuvoton Technology Corp.
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

#define REG_PID         0xB0000000

int print_cpuinfo (void)
{
        unsigned int id;

        id = readl(REG_PID) & 0xFFFF;

        printf("CPU: ");

        switch(id) {
                case 0x910:
                        printf("NUC910\n");
                        break;
                case 0x920:
                        printf("NUC910\n");
                        break;
                case 0x950:
                        printf("NUC950\n");
                        break;
                case 0x960:
                        printf("NUC960\n");
                        break;
                case 0xd008:
                        printf("NUC970\n");
                        break;
                default:
                        printf("Unknown\n");
                        break;

        }

        return(0);
}

int arch_cpu_init(void)
{
        // do nothing...

        return 0;
}

