/*
 * spi.c: B53 register access through SPI
 *
 * Copyright (C) 2011 Jonas Gorski <jonas.gorski@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "b53.h"
#include "b53_regs.h"

#include <linux/kernel.h>
#include <linux/spi/spi.h>

#define B53_SPI_STATUS		0xfe
#define B53_SPI_CMD_RACK	BIT(5)

#define B53_SPI_CMD_READ	0x00
#define B53_SPI_CMD_WRITE	0x01
#define B53_SPI_CMD_NORMAL	0x60
#define B53_SPI_CMD_FAST	0x10

#define B53_SPI_PAGE_SELECT	0xff


static inline int b53_spi_set_page(struct spi_device *spi, u8 page)
{
	u8 txbuf[3];

	txbuf[0] = B53_SPI_CMD_NORMAL | B53_SPI_CMD_WRITE;
	txbuf[1] = B53_SPI_PAGE_SELECT;
	txbuf[2] = page;

	return spi_write(spi, txbuf, sizeof(txbuf));
}

static inline int b53_spi_read(struct spi_device *spi, u8 *val)
{
	u8 txbuf[2];

	txbuf[0] = B53_SPI_CMD_NORMAL | B53_SPI_CMD_READ;
	txbuf[1] = 0xf0;

	return spi_write_then_read(spi, txbuf, sizeof(txbuf), val, 1);
}

static int b53_spi_prepare_reg_read(struct spi_device *spi, u8 reg)
{
	u8 txbuf[2];
	u8 rxbuf;
	int retry_count;

	txbuf[0] = B53_SPI_CMD_NORMAL | B53_SPI_CMD_READ;
	txbuf[1] = reg;

	spi_write_then_read(spi, txbuf, sizeof(txbuf), &rxbuf, 1);

	for (retry_count = 0; retry_count < 10; retry_count++) {
		txbuf[0] = B53_SPI_CMD_NORMAL | B53_SPI_CMD_READ;
		txbuf[1] = B53_SPI_STATUS;

		spi_write_then_read(spi, txbuf, sizeof(txbuf), &rxbuf, 1);

		if (rxbuf & B53_SPI_CMD_RACK)
			break;

		mdelay(1);
	}

	if (retry_count == 10)
		return -1;

	return 0;
}


static u8 b53_spi_read8(struct b53_device *dev, u8 page, u8 reg)
{
	struct spi_device *spi = dev->priv;
	u8 ret;

	b53_spi_set_page(spi, page);
	b53_spi_prepare_reg_read(spi, reg);
	b53_spi_read(spi, &ret);

	return ret;
}

static u16 b53_spi_read16(struct b53_device *dev, u8 page, u8 reg)
{
	struct spi_device *spi = dev->priv;
	u8 val;
	u16 ret;
	int i;

	b53_spi_set_page(spi, page);
	b53_spi_prepare_reg_read(spi, reg);

	b53_spi_read(spi, &val);
	ret = val;
	for (i = 1; i < 2; i++) {
		b53_spi_read(spi, &val);
		ret <<= 8;
		ret |= val;
	}

	return ret;
}

static u32 b53_spi_read32(struct b53_device *dev, u8 page, u8 reg)
{
	struct spi_device *spi = dev->priv;
	u8 val;
	u32 ret;
	int i;

	b53_spi_set_page(spi, page);
	b53_spi_prepare_reg_read(spi, reg);

	b53_spi_read(spi, &val);
	ret = val;
	for (i = 1; i < 4; i++) {
		b53_spi_read(spi, &val);
		ret <<= 8;
		ret |= val;
	}

	return ret;
}

static u64 b53_spi_read48(struct b53_device *dev, u8 page, u8 reg)
{
	struct spi_device *spi = dev->priv;
	u8 val;
	u64 ret;
	int i;

	b53_spi_set_page(spi, page);
	b53_spi_prepare_reg_read(spi, reg);

	b53_spi_read(spi, &val);
	ret = val;
	for (i = 1; i < 6; i++) {
		b53_spi_read(spi, &val);
		ret <<= 8;
		ret |= val;
	}

	return ret;
}

static u64 b53_spi_read64(struct b53_device *dev, u8 page, u8 reg)
{
	struct spi_device *spi = dev->priv;
	u8 val;
	u64 ret;
	int i;

	b53_spi_set_page(spi, page);
	b53_spi_prepare_reg_read(spi, reg);

	b53_spi_read(spi, &val);
	ret = val;
	for (i = 1; i < 8; i++) {
		b53_spi_read(spi, &val);
		ret <<= 8;
		ret |= val;
	}

	return ret;
}

static void b53_spi_write8(struct b53_device *dev, u8 page, u8 reg, u8 value)
{
	struct spi_device *spi = dev->priv;
	u8 txbuf[3];

	b53_spi_set_page(spi, page);

	txbuf[0] = B53_SPI_CMD_NORMAL | B53_SPI_CMD_WRITE;
	txbuf[1] = reg;
	txbuf[2] = value;

	spi_write(spi, txbuf, sizeof(txbuf));
}

static void b53_spi_write16(struct b53_device *dev, u8 page, u8 reg, u16 value)
{
	struct spi_device *spi = dev->priv;
	u8 txbuf[4];
	int i;

	b53_spi_set_page(spi, page);

	txbuf[0] = B53_SPI_CMD_NORMAL | B53_SPI_CMD_WRITE;
	txbuf[1] = reg;

	/* switch expects little endian */
	for (i = (sizeof(txbuf) - 1); i >= 2; i++) {
		txbuf[i] = value & 0xff;
		value >>= 8;
	}

	spi_write(spi, txbuf, sizeof(txbuf));
}

static void b53_spi_write32(struct b53_device *dev, u8 page, u8 reg, u32 value)
{
	struct spi_device *spi = dev->priv;
	u8 txbuf[6];
	int i;

	b53_spi_set_page(spi, page);

	txbuf[0] = B53_SPI_CMD_NORMAL | B53_SPI_CMD_WRITE;
	txbuf[1] = reg;

	/* switch expects little endian */
	for (i = (sizeof(txbuf) - 1); i >= 2; i++) {
		txbuf[i] = value & 0xff;
		value >>= 8;
	}

	spi_write(spi, txbuf, sizeof(txbuf));
}

static void b53_spi_write48(struct b53_device *dev, u8 page, u8 reg, u64 value)
{
	struct spi_device *spi = dev->priv;
	u8 txbuf[8];
	int i;

	b53_spi_set_page(spi, page);

	txbuf[0] = B53_SPI_CMD_NORMAL | B53_SPI_CMD_WRITE;
	txbuf[1] = reg;

	/* switch expects little endian */
	for (i = (sizeof(txbuf) - 1); i >= 2; i++) {
		txbuf[i] = value & 0xff;
		value >>= 8;
	}

	spi_write(spi, txbuf, sizeof(txbuf));
}

static void b53_spi_write64(struct b53_device *dev, u8 page, u8 reg, u64 value)
{
	struct spi_device *spi = dev->priv;
	u8 txbuf[10];
	int i;

	b53_spi_set_page(spi, page);

	txbuf[0] = B53_SPI_CMD_NORMAL | B53_SPI_CMD_WRITE;
	txbuf[1] = reg;

	/* switch expects little endian */
	for (i = (sizeof(txbuf) - 1); i >= 2; i++) {
		txbuf[i] = value & 0xff;
		value >>= 8;
	}

	spi_write(spi, txbuf, sizeof(txbuf));
}

static struct b53_io_ops b53_spi_ops = {
		.read8 = b53_spi_read8,
		.read16 = b53_spi_read16,
		.read32 = b53_spi_read32,
		.read48 = b53_spi_read48,
		.read64 = b53_spi_read64,
		.write8 = b53_spi_write8,
		.write16 = b53_spi_write16,
		.write32 = b53_spi_write32,
		.write48 = b53_spi_write48,
		.write64 = b53_spi_write64,
};

static int __devinit b53_spi_probe(struct spi_device *spi)
{
	struct b53_device *dev;
	int ret;

	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev)
		return -ENOMEM;

	if ((ret = b53_switch_register(dev, &b53_spi_ops, spi))) {
		kfree(dev);
		return ret;
	}

	spi->dev.platform_data = dev;

	return 0;
}

static int __devexit b53_spi_remove(struct spi_device *spi)
{
	struct b53_device *dev = spi->dev.platform_data;

	if (dev) {
		b53_switch_remove(dev);
		kfree(dev);
		spi->dev.platform_data = NULL;
	}

	return 0;
}

static struct spi_driver b53_spi_driver = {
	.driver = {
		.name	= "b53-switch",
		.bus	= &spi_bus_type,
		.owner	= THIS_MODULE,
	},
	.probe	= b53_spi_probe,
	.remove	= __devexit_p(b53_spi_remove),
};

int __init b53_spi_driver_register(void)
{
	return spi_register_driver(&b53_spi_driver);
}

void __exit b53_spi_driver_unregister(void)
{
	return spi_unregister_driver(&b53_spi_driver);
}
