/*
 * SPI flash interface
 *
 * Copyright (C) 2008 Atmel Corporation
 * Copyright (C) 2010 Reinhard Meyer, EMK Elektronik
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <fdtdec.h>
#include <malloc.h>
#include <spi.h>
#include <spi_flash.h>
#include <watchdog.h>

#include "../../spi/nuc970_spi.h"

#include "spi_flash_internal.h"

DECLARE_GLOBAL_DATA_PTR;

int spi_flash_en_quad_mode(struct spi_flash *flash);
int spi_flash_disable_quad_mode(void);
int spi_flash_reset(void);
static int _spi_flash_disable_quad_mode(struct spi_flash *flash);
#ifdef CONFIG_SPI_FLASH_EON
int EON_en_quad_mode(struct spi_flash *flash);
int EON_disable_quad_mode(struct spi_flash *flash);
#endif

static void spi_flash_addr(struct spi_flash *flash, u32 addr, u8 *cmd, u8 *cmd_len)
{
	/* cmd[0] is actual command */
	if (spi_flash_use_4byte_mode(flash)) { 
		cmd[1] = addr >> 24; 
		cmd[2] = addr >> 16; 
		cmd[3] = addr >> 8; 
		cmd[4] = addr >> 0; 
		*cmd_len = 5; 
	} else { 
		cmd[1] = addr >> 16; 
		cmd[2] = addr >> 8; 
		cmd[3] = addr >> 0; 
		*cmd_len = 4; 
	}
}

static void spi_flash_page_addr(struct spi_flash *flash, u32 page_addr, u32 byte_addr, u8 *cmd, u8 *cmd_len) 
{ 
	/* cmd[0] is actual command */ 
	if (spi_flash_use_4byte_mode(flash)) { 
		cmd[1] = page_addr >> 16; 
		cmd[2] = page_addr >> 8; 
		cmd[3] = page_addr >> 0; 
		cmd[4] = byte_addr; 
		*cmd_len = 5; 
	} else { 
		cmd[1] = page_addr >> 8; 
		cmd[2] = page_addr >> 0; 
		cmd[3] = byte_addr; 
		*cmd_len = 4; 
	} 
} 

#ifdef CONFIG_SPI_FLASH_EON
static int spi_flash_read_write(struct spi_slave *spi,
				const u8 *cmd, size_t cmd_len,
				const u8 *data_out, u8 *data_in,
				size_t data_len)
{
	unsigned long flags = SPI_XFER_BEGIN;
	int ret;
	//int i;

	if (data_len == 0)
		flags |= SPI_XFER_END;
/*	
	printf("\n[cmd]>>");
	for(i=0;i<cmd_len;i++)
		printf("0x%02x ", *(cmd+i));
	printf("\n");
*/		
	if (spi->quad_enable)
    		flags |= SPI_6WIRE;
			
	ret = spi_xfer(spi, cmd_len * 8, cmd, NULL, flags);
	if (ret) {
		debug("SF: Failed to send command (%zu bytes): %d\n",
				cmd_len, ret);
	} else if (data_len != 0) {
		if(data_out)
		{	
/*
			printf("data=>>");
			for(i=0;i<0x50;i++)
				printf("0x%02x ", *(data_out+i));	
*/
		}	
//		printf("\n");
		
		if (spi->quad_enable)
	    		flags |= SPI_6WIRE;
		else
			flags = 0;
			
		ret = spi_xfer(spi, data_len * 8, data_out, data_in, flags | SPI_XFER_END);
		if (ret)
			debug("SF: Failed to transfer %zu bytes of data: %d\n",
					data_len, ret);
	}
	
	if(data_in)
	{	
/*
		printf("data<<=");
		for(i=0;i<0x50;i++)
			printf("0x%02x ", *(data_in+i));	
*/
	}
//	printf("\n");
	
	return ret;
}
#endif

#if defined(CONFIG_SPI_FLASH_WINBOND) || defined(CONFIG_SPI_FLASH_MACRONIX) || defined(CONFIG_SPI_FLASH_SPANSION)
static int spi_flash_read_write(struct spi_slave *spi,
				const u8 *cmd, size_t cmd_len,
				const u8 *data_out, u8 *data_in,
				size_t data_len)
{
	unsigned long flags = SPI_XFER_BEGIN;
	int ret;
	int i;

	if (data_len == 0)
		flags |= SPI_XFER_END;
/*	
	printf("\n[cmd]>>");
	for(i=0;i<cmd_len;i++)
		printf("0x%02x ", *(cmd+i));
	printf("\n");
*/		
	ret = spi_xfer(spi, cmd_len * 8, cmd, NULL, flags);
	if (ret) {
		debug("SF: Failed to send command (%zu bytes): %d\n",
				cmd_len, ret);
	} else if (data_len != 0) {
/*
		if(data_out)
		{	

			printf("data=>>");
			for(i=0;i<0x50;i++)
				printf("0x%02x ", *(data_out+i));	

		}	
		printf("\n");
*/		
		if (spi->quad_enable)
	    		flags |= SPI_6WIRE;
		else
			flags = 0;
			
		ret = spi_xfer(spi, data_len * 8, data_out, data_in, flags | SPI_XFER_END);
		if (ret)
			debug("SF: Failed to transfer %zu bytes of data: %d\n",
					data_len, ret);
	}
/*	
	if(data_in)
	{	

		printf("data<<=");
		for(i=0;i<0x50;i++)
			printf("0x%02x ", *(data_in+i));	

	}
	printf("\n");
*/	
	return ret;
}
#endif

int spi_flash_cmd(struct spi_slave *spi, u8 cmd, void *response, size_t len)
{
	return spi_flash_cmd_read(spi, &cmd, 1, response, len);
}

int spi_flash_cmd_read(struct spi_slave *spi, const u8 *cmd,
		size_t cmd_len, void *data, size_t data_len)
{
	return spi_flash_read_write(spi, cmd, cmd_len, NULL, data, data_len);
}

int spi_flash_cmd_write(struct spi_slave *spi, const u8 *cmd, size_t cmd_len,
		const void *data, size_t data_len)
{
	return spi_flash_read_write(spi, cmd, cmd_len, data, NULL, data_len);
}

int spi_flash_cmd_write_multi(struct spi_flash *flash, u32 offset,
		size_t len, const void *buf)
{
	unsigned long page_addr, byte_addr, page_size;
	size_t chunk_len, actual;
	int ret;
	u8 cmd[5], cmd_len;

	page_size = flash->page_size;
	page_addr = offset / page_size;
	byte_addr = offset % page_size;

	ret = spi_claim_bus(flash->spi);
	if (ret) {
		debug("SF: unable to claim SPI bus\n");
		return ret;
	}

	cmd[0] = CMD_PAGE_PROGRAM;
	for (actual = 0; actual < len; actual += chunk_len) {
		chunk_len = min(len - actual, page_size - byte_addr);

		if (flash->spi->max_write_size)
			chunk_len = min(chunk_len, flash->spi->max_write_size);

		spi_flash_page_addr(flash, page_addr, byte_addr, cmd, &cmd_len);

		debug("PP: 0x%p => cmd = { 0x%02x 0x%02x%02x%02x%02x } chunk_len = %zu\n", 
			buf + actual, cmd[0], cmd[1], cmd[2], cmd[3], cmd[4], chunk_len);

		ret = spi_flash_cmd_write_enable(flash);
		if (ret < 0) {
			debug("SF: enabling write failed\n");
			break;
		}

		ret = spi_flash_cmd_write(flash->spi, cmd, cmd_len,
					  buf + actual, chunk_len);
		if (ret < 0) {
			debug("SF: write failed\n");
			break;
		}

		ret = spi_flash_cmd_wait_ready(flash, SPI_FLASH_PROG_TIMEOUT);
		if (ret)
			break;

		byte_addr += chunk_len;
		if (byte_addr == page_size) {
			page_addr++;
			byte_addr = 0;
		}
	}

	debug("SF: program %s %zu bytes @ %#x\n",
	      ret ? "failure" : "success", len, offset);

	spi_release_bus(flash->spi);
	return ret;
}

int spi_flash_cmd_write_quad(struct spi_flash *flash, u32 offset,
		size_t len, const void *buf)
{
	unsigned long page_addr, byte_addr, page_size;
	size_t chunk_len, actual;
	int ret;
	u8 cmd[4];
	struct spi_slave *spi = flash->spi;
	
	spi->quad_enable = 1;
		
	page_size = flash->page_size;
	page_addr = offset / page_size;
	byte_addr = offset % page_size;
	
	ret = spi_claim_bus(flash->spi);
	if (ret) {
		debug("SF: unable to claim SPI bus\n");
		return ret;
	}

	cmd[0] = CMD_PAGE_PROGRAM_QUAD;
	for (actual = 0; actual < len; actual += chunk_len) {
		chunk_len = min(len - actual, page_size - byte_addr);

		if (flash->spi->max_write_size)
			chunk_len = min(chunk_len, flash->spi->max_write_size);

		cmd[1] = page_addr >> 8;
		cmd[2] = page_addr;
		cmd[3] = byte_addr;

		debug("PP: 0x%p => cmd = { 0x%02x 0x%02x%02x%02x } chunk_len = %zu\n",
		      buf + actual, cmd[0], cmd[1], cmd[2], cmd[3], chunk_len);

		ret = spi_flash_cmd_write_enable(flash);
		if (ret < 0) {
			debug("SF: enabling write failed\n");
			break;
		}

		ret = spi_flash_cmd_write(flash->spi, cmd, 4,
					  buf + actual, chunk_len);
		if (ret < 0) {
			debug("SF: write failed\n");
			break;
		}

		ret = spi_flash_cmd_wait_ready(flash, SPI_FLASH_PROG_TIMEOUT);
		if (ret)
			break;

		byte_addr += chunk_len;
		if (byte_addr == page_size) {
			page_addr++;
			byte_addr = 0;
		}
	}

	debug("SF: program %s %zu bytes @ %#x\n",
	      ret ? "failure" : "success", len, offset);

	spi->quad_enable = 0;

	spi_release_bus(flash->spi);
	return ret;
}

int spi_flash_read_common(struct spi_flash *flash, const u8 *cmd,
		size_t cmd_len, void *data, size_t data_len)
{
	struct spi_slave *spi = flash->spi;
	int ret;

	spi_claim_bus(spi);
	ret = spi_flash_cmd_read(spi, cmd, cmd_len, data, data_len);
	spi_release_bus(spi);

	return ret;
}

int spi_flash_cmd_read_fast(struct spi_flash *flash, u32 offset,
		size_t len, void *data)
{
	u8 cmd[6], cmd_len;

	/* Handle memory-mapped SPI */
	if (flash->memory_map)
		memcpy(data, flash->memory_map + offset, len);

	cmd[0] = CMD_READ_ARRAY_FAST;
	spi_flash_addr(flash, offset, cmd, &cmd_len);  
        cmd[cmd_len] = 0x00; 

	return spi_flash_read_common(flash, cmd, cmd_len + 1, data, len);
}

#ifdef CONFIG_SPI_FLASH_EON
int spi_flash_cmd_read_quad(struct spi_flash *flash, u32 offset,
               size_t len, void *data)
{
	struct spi_slave *spi = flash->spi;
	
	unsigned long page_addr, byte_addr, page_size;
	size_t chunk_len, actual;
	int ret = 0;
	u8 cmd[8], cmd_len;
	
	EON_en_quad_mode(flash);

	spi->quad_enable = 1;

	/* Handle memory-mapped SPI */
	if (flash->memory_map)
		memcpy(data, flash->memory_map + offset, len);
	
	page_size = flash->page_size;
	page_addr = offset / page_size;
	byte_addr = offset % page_size;

	if (spi_flash_use_4byte_mode(flash)) 
		cmd_len = 8;
	else
		cmd_len = 7;
	
	cmd[0] = CMD_READ_QUAD_EON;

	for (actual = 0; actual < len; actual += chunk_len) {
		//chunk_len = min(len - actual, page_size - byte_addr);
		chunk_len = len; //CWWeng 2015.10.21

		if (cmd_len == 7) { //3-byte address
			cmd[1] = page_addr >> 8;
			cmd[2] = page_addr;
			cmd[3] = byte_addr;
			cmd[4] = 0x0;
			cmd[5] = 0x0;
			cmd[6] = 0x0;
		} else { //4-byte address
			cmd[1] = page_addr >> 16;
			cmd[2] = page_addr >> 8;
			cmd[3] = page_addr;
			cmd[4] = byte_addr;
			cmd[5] = 0x0;
			cmd[6] = 0x0;
			cmd[7] = 0x0;
		}
		
		//ret = spi_flash_read_common(flash, cmd, sizeof(cmd),
		ret = spi_flash_read_common(flash, cmd, cmd_len,
		       data + actual, chunk_len);
		if (ret < 0) {
			debug("SF: read failed");
			break;
		}
		
		byte_addr += chunk_len;
		if (byte_addr == page_size) {
			page_addr++;
			byte_addr = 0;
		}
	}
	
	EON_disable_quad_mode(flash);

	spi->quad_enable = 0;

	return ret;
}
#endif

#if defined(CONFIG_SPI_FLASH_WINBOND) || defined(CONFIG_SPI_FLASH_MACRONIX) || defined(CONFIG_SPI_FLASH_SPANSION)
int spi_flash_cmd_read_quad(struct spi_flash *flash, u32 offset,
               size_t len, void *data)
{
	struct spi_slave *spi = flash->spi;
	
	unsigned long page_addr, byte_addr, page_size;
	size_t chunk_len, actual;
	int ret = 0;
	u8 cmd[6], cmd_len;
	
	spi->quad_enable = 1;
	
	/* Handle memory-mapped SPI */
	if (flash->memory_map)
		memcpy(data, flash->memory_map + offset, len);
	
	page_size = flash->page_size;
	page_addr = offset / page_size;
	byte_addr = offset % page_size;

	if (spi_flash_use_4byte_mode(flash)) 
		cmd_len = 6;
	else
		cmd_len = 5;
	
	cmd[0] = CMD_READ_ARRAY_QUAD;
	for (actual = 0; actual < len; actual += chunk_len) {
		//chunk_len = min(len - actual, page_size - byte_addr);
		chunk_len = len; //CWWeng 2015.10.21

		if (cmd_len == 5) { //3-byte address
			cmd[1] = page_addr >> 8;
			cmd[2] = page_addr;
			cmd[3] = byte_addr;
			cmd[4] = 0x0;
		} else { //4-byte address
			cmd[1] = page_addr >> 16;
			cmd[2] = page_addr >> 8;
			cmd[3] = page_addr;
			cmd[4] = byte_addr;
			cmd[5] = 0x0;
		}
		
		//ret = spi_flash_read_common(flash, cmd, sizeof(cmd),
		ret = spi_flash_read_common(flash, cmd, cmd_len,
		       data + actual, chunk_len);
		if (ret < 0) {
			debug("SF: read failed");
			break;
		}
		
		byte_addr += chunk_len;
		if (byte_addr == page_size) {
			page_addr++;
			byte_addr = 0;
		}
	}
	
	spi->quad_enable = 0;

	return ret;
}
#endif

int spi_flash_cmd_poll_bit(struct spi_flash *flash, unsigned long timeout,
			   u8 cmd, u8 poll_bit)
{
	struct spi_slave *spi = flash->spi;
	unsigned long timebase;
	int ret;
	u8 status;
/*
	printf("\n[cmd]>>");
        printf("0x%02x ", cmd);
        printf("\n");
*/
	ret = spi_xfer(spi, 8, &cmd, NULL, SPI_XFER_BEGIN);
	if (ret) {
		debug("SF: Failed to send command %02x: %d\n", cmd, ret);
		return ret;
	}

	timebase = get_timer(0);
	do {
		WATCHDOG_RESET();

		//printf("spi_xfer\n");
		ret = spi_xfer(spi, 8, NULL, &status, 0);
		if (ret)
			return -1;

		if ((status & poll_bit) == 0)
			break;

	} while (get_timer(timebase) < timeout);

	spi_xfer(spi, 0, NULL, NULL, SPI_XFER_END);

	if ((status & poll_bit) == 0)
		return 0;

	/* Timed out */
	debug("SF: time out!\n");
	return -1;
}

int spi_flash_cmd_wait_ready(struct spi_flash *flash, unsigned long timeout)
{
	return spi_flash_cmd_poll_bit(flash, timeout,
		CMD_READ_STATUS, STATUS_WIP);
}

int spi_nand_flash_cmd_poll_bit(struct spi_flash *flash, unsigned long timeout,
                           u8 cmd, u8 poll_bit, u8 *status)
{
	struct spi_slave *spi = flash->spi;
	unsigned long timebase;
	u8 cmd_buf[2];

	cmd_buf[0] = 0x0F;
	cmd_buf[1] = cmd;

	timebase = get_timer(0);
	do {
		WATCHDOG_RESET();

		spi_flash_cmd_read(spi, cmd_buf, 2, status, 1);
		if ((*status & poll_bit) == 0)
			break;

	} while (get_timer(timebase) < timeout);

	if ((*status & poll_bit) == 0)
		return 0;

	/* Timed out */
	debug("SF: time out!\n");
	return -1;
}

int spi_nand_flash_cmd_wait_ready(struct spi_flash *flash, u8 status_bit, u8 *status,
                                  unsigned long timeout)
{
	return spi_nand_flash_cmd_poll_bit(flash, timeout,
					   0xC0, status_bit, status);
}

int spi_flash_cmd_erase(struct spi_flash *flash, u32 offset, size_t len)
{
	u32 start, end, erase_size;
	int ret;
	u8 cmd[5], cmd_len;

	erase_size = flash->sector_size;
	if (offset % erase_size || len % erase_size) {
		debug("SF: Erase offset/length not multiple of erase size\n");
		return -1;
	}

	ret = spi_claim_bus(flash->spi);
	if (ret) {
		debug("SF: Unable to claim SPI bus\n");
		return ret;
	}

	if (erase_size == 4096)
		cmd[0] = CMD_ERASE_4K;
	else
		cmd[0] = CMD_ERASE_64K;

	start = offset;
	end = start + len;

	while (offset < end) {
		spi_flash_addr(flash, offset, cmd, &cmd_len);
		offset += erase_size;

                debug("SF: erase %2x %2x %2x %2x %2x (%x)\n", cmd[0], cmd[1], 
			cmd[2], cmd[3], cmd[4], offset);

		ret = spi_flash_cmd_write_enable(flash);
		if (ret)
			goto out;

		ret = spi_flash_cmd_write(flash->spi, cmd, cmd_len, NULL, 0);
		if (ret)
			goto out;

		ret = spi_flash_cmd_wait_ready(flash, SPI_FLASH_PAGE_ERASE_TIMEOUT);
		if (ret)
			goto out;
	}

	debug("SF: Successfully erased %zu bytes @ %#x\n", len, start);

 out:
	spi_release_bus(flash->spi);
	return ret;
}

int spi_flash_cmd_write_status(struct spi_flash *flash, u8 sr)
{
	u8 cmd;
	int ret;

	ret = spi_flash_cmd_write_enable(flash);
	if (ret < 0) {
		debug("SF: enabling write failed\n");
		return ret;
	}

	cmd = CMD_WRITE_STATUS;
	ret = spi_flash_cmd_write(flash->spi, &cmd, 1, &sr, 1);
	if (ret) {
		debug("SF: fail to write status register\n");
		return ret;
	}

	ret = spi_flash_cmd_wait_ready(flash, SPI_FLASH_PROG_TIMEOUT);
	if (ret < 0) {
		debug("SF: write status register timed out\n");
		return ret;
	}

	return 0;
}

#ifdef CONFIG_SPI_FLASH_MACRONIX
static int read_sr(struct spi_flash *flash, u8 *rs)
{
	int ret;
	u8 cmd;

	cmd = CMD_READ_STATUS;
	ret = spi_flash_read_common(flash, &cmd, 1, rs, 1);
	if (ret < 0) {
		debug("SF: fail to read status register\n");
		return ret;
	}

	return 0;
}

static int spi_flash_write_common(struct spi_flash *flash, const u8 *cmd,
		size_t cmd_len, const void *buf, size_t buf_len)
{
	struct spi_slave *spi = flash->spi;
	unsigned long timeout = SPI_FLASH_PROG_TIMEOUT;
	int ret;

	if (buf == NULL)
		timeout = SPI_FLASH_PAGE_ERASE_TIMEOUT;

	ret = spi_claim_bus(spi);
	if (ret) {
		debug("SF: unable to claim SPI bus\n");
		return ret;
	}

	ret = spi_flash_cmd_write_enable(flash);
	if (ret < 0) {
		debug("SF: enabling write failed\n");
		return ret;
	}

	ret = spi_flash_cmd_write(spi, cmd, cmd_len, buf, buf_len);
	if (ret < 0) {
		debug("SF: write cmd failed\n");
		return ret;
	}

	ret = spi_flash_cmd_wait_ready(flash, timeout);
	if (ret < 0) {
		debug("SF: write %s timed out\n",
		      timeout == SPI_FLASH_PROG_TIMEOUT ?
			"program" : "page erase");
		return ret;
	}

	spi_release_bus(spi);

	return ret;
}

static int write_sr(struct spi_flash *flash, u8 ws)
{
	u8 cmd;
	int ret;

	cmd = CMD_WRITE_STATUS;
	ret = spi_flash_write_common(flash, &cmd, 1, &ws, 1);
	if (ret < 0) {
		debug("SF: fail to write status register\n");
		return ret;
	}

	return 0;
}
#endif

#ifdef CONFIG_SPI_FLASH_MACRONIX
int spi_flash_en_quad_mode(struct spi_flash *flash)
{
	u8 qeb_status;
	int ret;

	ret = read_sr(flash, &qeb_status);
	if (ret < 0)
		return ret;

	if (qeb_status & STATUS_QEB_MXIC)
		return 0;

	ret = write_sr(flash, qeb_status | STATUS_QEB_MXIC);
	if (ret < 0)
		return ret;

	/* read SR and check it */
	ret = read_sr(flash, &qeb_status);
	if (!(ret >= 0 && (qeb_status & STATUS_QEB_MXIC))) {
		printf("SF: Macronix SR Quad bit not clear\n");
		return -1; //-EINVAL;
	}

	return ret;
}

static int _spi_flash_disable_quad_mode(struct spi_flash *flash)
{
	u8 qeb_status;
	int ret;

	ret = read_sr(flash, &qeb_status);
	if (ret < 0)
		return ret;

	ret = write_sr(flash, qeb_status & ~(STATUS_QEB_MXIC));
	if (ret < 0)
		return ret;

	/* read SR and check it */
	ret = read_sr(flash, &qeb_status);
	if (!(ret >= 0 && (qeb_status & STATUS_QEB_MXIC))) {
		printf("SF: Macronix SR Quad bit not clear\n");
		return -22; //-EINVAL;
	}

	return ret;
}
#endif

#ifdef CONFIG_SPI_FLASH_EON
int EON_en_quad_mode(struct spi_flash *flash)
{
	int ret;

	spi_claim_bus(flash->spi);
	ret = spi_flash_cmd(flash->spi, CMD_QUAD_ENABLE, NULL, 0);
        if (ret) {
                printf("SF: Failed issue enable quad command (CMD_QUAD_ENABLE)\n");
        }
	spi_release_bus(flash->spi);

	return ret;
}

int EON_disable_quad_mode(struct spi_flash *flash)
{
	int ret;

	spi_claim_bus(flash->spi);
	ret = spi_flash_cmd(flash->spi, CMD_QUAD_RESET, NULL, 0);
        if (ret) {
                printf("SF: Failed issue reset quad command (CMD_QUAD_RESET)\n");
        }
	spi_release_bus(flash->spi);

	return ret;
}

int spi_flash_en_quad_mode(struct spi_flash *flash)
{
	return 0;
}

static int _spi_flash_disable_quad_mode(struct spi_flash *flash)
{
	return 0;
}
#endif

#if defined(CONFIG_SPI_FLASH_WINBOND) || defined(CONFIG_SPI_FLASH_SPANSION) 
int spi_flash_en_quad_mode(struct spi_flash *flash)
{
	u8 stat, con, cd;
	u16 cr;
	int ret;
	cd = CMD_WRITE_STATUS;
	
	ret = spi_flash_cmd_write_enable(flash);
	if (ret < 0) {
		debug("SF: enabling write failed\n");
		goto out;
	}
	
	ret = spi_flash_cmd(flash->spi, CMD_READ_STATUS, &stat, 1);
	ret = spi_flash_cmd(flash->spi, CMD_READ_CONFIG, &con, 1);
	if (ret < 0) {
		debug("%s: SF: read CR failed\n", __func__);
		goto out;
	}
	/* Byte 1 - status reg, Byte 2 - config reg */
	cr = ((con | 0x1 << 1) << 8) | (stat << 0);
	
	ret = spi_flash_cmd_write(flash->spi, &cd, 1, &cr, 2);
	if (ret) {
		debug("SF: fail to write conf register\n");
		goto out;
	}
	
	ret = spi_flash_cmd_wait_ready(flash, SPI_FLASH_PROG_TIMEOUT);
	if (ret < 0) {
		debug("SF: write conf register timed out\n");
		goto out;
	}
	
	ret = spi_flash_cmd(flash->spi, CMD_READ_STATUS, &stat, 1);
	ret = spi_flash_cmd(flash->spi, CMD_READ_CONFIG, &con, 1);
	if (ret < 0) {
		debug("%s: SF: read CR failed\n", __func__);
		goto out;
	}
	debug("%s: *** CR = %x\n", __func__, con);
	
	ret = spi_flash_cmd_write_disable(flash);
	if (ret < 0) {
		debug("SF: disabling write failed\n");
		goto out;
	}

out:
	return ret;
}

static int _spi_flash_disable_quad_mode(struct spi_flash *flash)
{
	u8 stat, con, cd;
	u16 cr;
	int ret;
	cd = CMD_WRITE_STATUS;
	
	ret = spi_flash_cmd_write_enable(flash);
	if (ret < 0) {
		debug("SF: enabling write failed\n");
		goto out;
	}
	
	ret = spi_flash_cmd(flash->spi, CMD_READ_STATUS, &stat, 1);
	ret = spi_flash_cmd(flash->spi, CMD_READ_CONFIG, &con, 1);
	if (ret < 0) {
		debug("%s: SF: read CR failed\n", __func__);
		goto out;
	}
	/* Byte 1 - status reg, Byte 2 - config reg */
	cr = ((con & ~0x1 << 1) << 8) | (stat << 0);
	
	ret = spi_flash_cmd_write(flash->spi, &cd, 1, &cr, 2);
	if (ret) {
		debug("SF: fail to write conf register\n");
		goto out;
	}
	
	ret = spi_flash_cmd_wait_ready(flash, SPI_FLASH_PROG_TIMEOUT);
	if (ret < 0) {
		debug("SF: write conf register timed out\n");
		goto out;
	}
	
	ret = spi_flash_cmd(flash->spi, CMD_READ_STATUS, &stat, 1);
	ret = spi_flash_cmd(flash->spi, CMD_READ_CONFIG, &con, 1);
	if (ret < 0) {
		debug("%s: SF: read CR failed\n", __func__);
		goto out;
	}
	debug("%s: *** CR = %x\n", __func__, con);
	
	ret = spi_flash_cmd_write_disable(flash);
	if (ret < 0) {
		debug("SF: disabling write failed\n");
		goto out;
	}

out:
	return ret;
}
#endif


int spi_flash_disable_quad_mode(void)
{
	struct spi_flash *flash;
	struct spi_slave *spi;

	/*
	 * Load U-Boot image from SPI flash into RAM
	 */

	flash = spi_flash_probe(0, 0, 1000000, 3); //SPI_MODE_3
	if (!flash) {
		puts("SPI probe failed.\n");
		hang();
	}

	spi = flash->spi;

	spi_claim_bus(spi);
	_spi_flash_disable_quad_mode(flash);
	spi_release_bus(spi);

	return 0;
}

int spi_flash_reset(void)
{
	struct spi_flash *flash;
	int ret;
	struct spi_slave *spi;

	/*
	 * Load U-Boot image from SPI flash into RAM
	 */

	flash = spi_flash_probe(0, 0, 1000000, 3); //SPI_MODE_3
	if (!flash) {
		puts("SPI probe failed.\n");
		hang();
	}

	spi = flash->spi;

	spi_claim_bus(spi);

#if defined(CONFIG_SPI_FLASH_WINBOND) || defined(CONFIG_SPI_FLASH_MACRONIX) || defined(CONFIG_SPI_FLASH_EON)
	ret = spi_flash_cmd(spi, CMD_RESET_ENABLE, NULL, 0);
	if (ret) {
		printf("SF: Failed issue reset command (CMD_RESET_ENABLE)\n");
	}

	ret = spi_flash_cmd(spi, CMD_RESET_MEMORY, NULL, 0);
	if (ret) {
		printf("SF: Failed issue reset command (CMD_RESET_MEMORY)\n");
	}
#endif
#ifdef CONFIG_SPI_FLASH_SPANSION
	ret = spi_flash_cmd(spi, CMD_RESET_SPAN, NULL, 0);
	if (ret) {
		printf("SF: Failed issue reset command (CMD_RESET_SPAN)\n");
	}

	ret = spi_flash_cmd(spi, CMD_RESET_MODE, NULL, 0);
	if (ret) {
		printf("SF: Failed issue reset command (CMD_RESET_SPAN)\n");
	}
#endif

	spi_release_bus(spi);

	printf("SF: Device software reset\n");
	
	return ret;
}

#ifdef CONFIG_OF_CONTROL
int spi_flash_decode_fdt(const void *blob, struct spi_flash *flash)
{
	fdt_addr_t addr;
	fdt_size_t size;
	int node;

	/* If there is no node, do nothing */
	node = fdtdec_next_compatible(blob, 0, COMPAT_GENERIC_SPI_FLASH);
	if (node < 0)
		return 0;

	addr = fdtdec_get_addr_size(blob, node, "memory-map", &size);
	if (addr == FDT_ADDR_T_NONE) {
		debug("%s: Cannot decode address\n", __func__);
		return 0;
	}

	if (flash->size != size) {
		debug("%s: Memory map must cover entire device\n", __func__);
		return -1;
	}
	flash->memory_map = (void *)addr;

	return 0;
}
#endif /* CONFIG_OF_CONTROL */

/*
 * The following table holds all device probe functions
 *
 * shift:  number of continuation bytes before the ID
 * idcode: the expected IDCODE or 0xff for non JEDEC devices
 * probe:  the function to call
 *
 * Non JEDEC devices should be ordered in the table such that
 * the probe functions with best detection algorithms come first.
 *
 * Several matching entries are permitted, they will be tried
 * in sequence until a probe function returns non NULL.
 *
 * IDCODE_CONT_LEN may be redefined if a device needs to declare a
 * larger "shift" value.  IDCODE_PART_LEN generally shouldn't be
 * changed.  This is the max number of bytes probe functions may
 * examine when looking up part-specific identification info.
 *
 * Probe functions will be given the idcode buffer starting at their
 * manu id byte (the "idcode" in the table below).  In other words,
 * all of the continuation bytes will be skipped (the "shift" below).
 */
#define IDCODE_CONT_LEN 0
#define IDCODE_PART_LEN 5
static const struct {
	const u8 shift;
	const u8 idcode;
	struct spi_flash *(*probe) (struct spi_slave *spi, u8 *idcode);
} flashes[] = {
	/* Keep it sorted by define name */
#ifdef CONFIG_SPI_FLASH_ATMEL
	{ 0, 0x1f, spi_flash_probe_atmel, },
#endif
#ifdef CONFIG_SPI_FLASH_EON
	{ 0, 0x1c, spi_flash_probe_eon, },
#endif
#ifdef CONFIG_SPI_FLASH_MACRONIX
	{ 0, 0xc2, spi_flash_probe_macronix, },
#endif
#ifdef CONFIG_SPI_FLASH_SPANSION
	{ 0, 0x01, spi_flash_probe_spansion, },
#endif
#ifdef CONFIG_SPI_FLASH_SST
	{ 0, 0xbf, spi_flash_probe_sst, },
#endif
#ifdef CONFIG_SPI_FLASH_STMICRO
	{ 0, 0x20, spi_flash_probe_stmicro, },
#endif
#ifdef CONFIG_SPI_FLASH_WINBOND
	{ 0, 0xef, spi_flash_probe_winbond, },
#endif
#ifdef CONFIG_SPI_FRAM_RAMTRON
	{ 6, 0xc2, spi_fram_probe_ramtron, },
# undef IDCODE_CONT_LEN
# define IDCODE_CONT_LEN 6
#endif
	/* Keep it sorted by best detection */
#ifdef CONFIG_SPI_FLASH_STMICRO
	{ 0, 0xff, spi_flash_probe_stmicro, },
#endif
#ifdef CONFIG_SPI_FRAM_RAMTRON_NON_JEDEC
	{ 0, 0xff, spi_fram_probe_ramtron, },
#endif
#ifdef CONFIG_SPI_NAND_WINBOND
	{ 0, 0xff, spi_nand_flash_probe, },
#endif
};
#define IDCODE_LEN (IDCODE_CONT_LEN + IDCODE_PART_LEN)

struct spi_flash *spi_flash_probe(unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int spi_mode)
{
	struct spi_slave *spi;
	struct spi_flash *flash = NULL;
	int ret, i, shift;
	u16 jedec, ext_jedec;
	u8 idcode[IDCODE_LEN], *idp;
	char *cp;

	spi = spi_setup_slave(bus, cs, max_hz, spi_mode);
	if (!spi) {
		printf("SF: Failed to set up slave\n");
		return NULL;
	}

	ret = spi_claim_bus(spi);
	if (ret) {
		debug("SF: Failed to claim SPI bus: %d\n", ret);
		goto err_claim_bus;
	}

	/* Read the ID codes */
	ret = spi_flash_cmd(spi, CMD_READ_ID, idcode, sizeof(idcode));
	if (ret)
		goto err_read_id;

#ifdef DEBUG
	printf("SF: Got idcodes\n");
	print_buffer(0, idcode, 1, sizeof(idcode), 0);
#endif

	/* count the number of continuation bytes */
	for (shift = 0, idp = idcode;
	     shift < IDCODE_CONT_LEN && *idp == 0x7f;
	     ++shift, ++idp)
		continue;

	/* search the table for matches in shift and id */
	for (i = 0; i < ARRAY_SIZE(flashes); ++i)
		if (flashes[i].shift == shift && flashes[i].idcode == *idp) {
			/* we have a match, call probe */
			flash = flashes[i].probe(spi, idp);
			if (flash)
				break;
		}

	jedec = idcode[1] << 8 | idcode[2];
        ext_jedec = idcode[3] << 8 | idcode[4];

	if (!flash) {
		printf("SF: Unsupported manufacturer %02x\n", *idp);
		printf("manuf %02x, jedec %04x, ext_jedec %04x\n",
                       idcode[0], jedec, ext_jedec);
		goto err_manufacturer_probe;
	}
	
	ret = spi_flash_set_4byte_mode(flash); 
	if (ret) { 
		debug("SF: Failed to enable 4 byte mode: %d\n", ret); 
		goto err_manufacturer_probe; 
	} 
#ifndef CONFIG_SPI_NAND_WINBOND  // no need to set this for spi nand
	cp = getenv("spimode");
	if (cp) 
                if(*cp == SPI_QUAD_MODE ) 
			spi_flash_en_quad_mode(flash);
#endif

#ifdef CONFIG_OF_CONTROL
	if (spi_flash_decode_fdt(gd->fdt_blob, flash)) {
		debug("SF: FDT decode error\n");
		goto err_manufacturer_probe;
	}
#endif
	printf("SF: Detected %s with page size ", flash->name);
	print_size(flash->sector_size, ", total ");
	print_size(flash->size, "");
	if (flash->memory_map)
		printf(", mapped at %p", flash->memory_map);
	puts("\n");

	spi_release_bus(spi);

	return flash;

err_manufacturer_probe:
err_read_id:
	spi_release_bus(spi);
err_claim_bus:
	spi_free_slave(spi);
	return NULL;
}

void *spi_flash_do_alloc(int offset, int size, struct spi_slave *spi,
			 const char *name)
{
	struct spi_flash *flash;
	void *ptr;
	char *cp;

	ptr = malloc(size);
	if (!ptr) {
		debug("SF: Failed to allocate memory\n");
		return NULL;
	}
	memset(ptr, '\0', size);
	flash = (struct spi_flash *)(ptr + offset);

	/* Set up some basic fields - caller will sort out sizes */
	flash->spi = spi;
	flash->name = name;

	cp = getenv("spimode");
	if (cp) {
                if(*cp == SPI_QUAD_MODE ) {
			flash->read = spi_flash_cmd_read_quad;
			flash->write = spi_flash_cmd_write_quad;
		}
		else {
			flash->read = spi_flash_cmd_read_fast;
			flash->write = spi_flash_cmd_write_multi;
		}
	}
	else {
		flash->read = spi_flash_cmd_read_fast;
		flash->write = spi_flash_cmd_write_multi;
	}

//	flash->read = spi_flash_cmd_read_fast;
//	flash->write = spi_flash_cmd_write_multi;
	flash->erase = spi_flash_cmd_erase;

	return flash;
}

void spi_flash_free(struct spi_flash *flash)
{
	spi_free_slave(flash->spi);
	free(flash);
}
