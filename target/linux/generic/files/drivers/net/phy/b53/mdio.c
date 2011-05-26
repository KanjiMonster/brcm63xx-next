/*
 * mdio.c: B53 register access through MII registers
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

#include <linux/phy.h>

#define B53_PSEUDO_PHY	0x1e /* Register Access Pseudo PHY */

/* MII registers */
#define REG_MII_PAGE    0x10    /* MII Page register */
#define REG_MII_ADDR    0x11    /* MII Address register */
#define REG_MII_DATA0   0x18    /* MII Data register 0 */
#define REG_MII_DATA1   0x19    /* MII Data register 1 */
#define REG_MII_DATA2   0x1a    /* MII Data register 2 */
#define REG_MII_DATA3   0x1b    /* MII Data register 3 */

#define REG_MII_PAGE_ENABLE     BIT(0)
#define REG_MII_ADDR_WRITE      BIT(0)
#define REG_MII_ADDR_READ       BIT(1)



static void b53_mdio_op(struct mii_bus *bus, u8 page, u8 reg, u16 op)
{
	int i;
	u16 v;

	/* set page number */
	v = (page << 8) | REG_MII_PAGE_ENABLE;
	mdiobus_write(bus, B53_PSEUDO_PHY, REG_MII_PAGE, v);

	/* set register address */
	v = (reg << 8) | op;
	mdiobus_write(bus, B53_PSEUDO_PHY, REG_MII_ADDR, v);

	/* check if operation completed */
	for (i = 0; i < 5; ++i) {
		v = mdiobus_read(bus, B53_PSEUDO_PHY, REG_MII_ADDR);
		if (!(v & (REG_MII_ADDR_WRITE | REG_MII_ADDR_READ)))
			break;
		udelay(10);
	}
}

static inline u8 b53_mdio_read8(struct b53_device *dev, u8 page, u8 reg)
{
	struct mii_bus *bus = dev->priv;

	b53_mdio_op(bus, page, reg, REG_MII_ADDR_READ);

	return (u8)mdiobus_read(bus, B53_PSEUDO_PHY, REG_MII_DATA0);
}

static inline u16 b53_mdio_read16(struct b53_device *dev, u8 page, u8 reg)
{
	struct mii_bus *bus = dev->priv;

	b53_mdio_op(bus, page, reg, REG_MII_ADDR_READ);

	return mdiobus_read(bus, B53_PSEUDO_PHY, REG_MII_DATA0);
}

static inline u32 b53_mdio_read32(struct b53_device *dev, u8 page, u8 reg)
{
	struct mii_bus *bus = dev->priv;
	u32 val;

	b53_mdio_op(bus, page, reg, REG_MII_ADDR_READ);
	val = mdiobus_read(bus, B53_PSEUDO_PHY, REG_MII_DATA0);
	val |= mdiobus_read(bus, B53_PSEUDO_PHY, REG_MII_DATA1) << 16;

	return val;
}

static inline u64 b53_mdio_read48(struct b53_device *dev, u8 page, u8 reg)
{
	struct mii_bus *bus = dev->priv;
	u64 val;

	b53_mdio_op(bus, page, reg, REG_MII_ADDR_READ);

	val = mdiobus_read(bus, B53_PSEUDO_PHY, REG_MII_DATA0);
	val |= mdiobus_read(bus, B53_PSEUDO_PHY, REG_MII_DATA1) << 16;
	val |= (u64)mdiobus_read(bus, B53_PSEUDO_PHY, REG_MII_DATA2) << 32;

	return val;
}

static inline u64 b53_mdio_read64(struct b53_device *dev, u8 page, u8 reg)
{
	struct mii_bus *bus = dev->priv;
	u64 val;

	b53_mdio_op(bus, page, reg, REG_MII_ADDR_READ);

	val = mdiobus_read(bus, B53_PSEUDO_PHY, REG_MII_DATA0);
	val |= mdiobus_read(bus, B53_PSEUDO_PHY, REG_MII_DATA1) << 16;
	val |= (u64)mdiobus_read(bus, B53_PSEUDO_PHY, REG_MII_DATA2) << 32;
	val |= (u64)mdiobus_read(bus, B53_PSEUDO_PHY, REG_MII_DATA3) << 48;

	return val;
}

static void b53_mdio_write8(struct b53_device *dev, u8 page, u8 reg, u8 value)
{
	struct mii_bus *bus = dev->priv;

	mdiobus_write(bus, B53_PSEUDO_PHY, REG_MII_DATA0, value);
	b53_mdio_op(bus, page, reg, REG_MII_ADDR_WRITE);
}

static void b53_mdio_write16(struct b53_device *dev, u8 page, u8 reg,
			     u16 value)
{
	struct mii_bus *bus = dev->priv;

	mdiobus_write(bus, B53_PSEUDO_PHY, REG_MII_DATA0, value);
	b53_mdio_op(bus, page, reg, REG_MII_ADDR_WRITE);
}

static void b53_mdio_write32(struct b53_device *dev, u8 page, u8 reg,
				    u32 value)
{
	struct mii_bus *bus = dev->priv;

	mdiobus_write(bus, B53_PSEUDO_PHY, REG_MII_DATA0, value);
	mdiobus_write(bus, B53_PSEUDO_PHY, REG_MII_DATA1, value >> 16);
	b53_mdio_op(bus, page, reg, REG_MII_ADDR_WRITE);

}

static void b53_mdio_write48(struct b53_device *dev, u8 page, u8 reg,
				    u64 value)
{
	struct mii_bus *bus = dev->priv;

	mdiobus_write(bus, B53_PSEUDO_PHY, REG_MII_DATA0, value);
	mdiobus_write(bus, B53_PSEUDO_PHY, REG_MII_DATA1, value >> 16);
	mdiobus_write(bus, B53_PSEUDO_PHY, REG_MII_DATA2, value >> 32);
	b53_mdio_op(bus, page, reg, REG_MII_ADDR_WRITE);

}

static void b53_mdio_write64(struct b53_device *dev, u8 page, u8 reg,
			     u64 value)
{
	struct mii_bus *bus = dev->priv;

	mdiobus_write(bus, B53_PSEUDO_PHY, REG_MII_DATA0, value);
	mdiobus_write(bus, B53_PSEUDO_PHY, REG_MII_DATA1, value >> 16);
	mdiobus_write(bus, B53_PSEUDO_PHY, REG_MII_DATA2, value >> 32);
	mdiobus_write(bus, B53_PSEUDO_PHY, REG_MII_DATA3, value >> 48);
	b53_mdio_op(bus, page, reg, REG_MII_ADDR_WRITE);
}

static struct b53_io_ops b53_mdio_ops = {
		.read8 = b53_mdio_read8,
		.read16 = b53_mdio_read16,
		.read32 = b53_mdio_read32,
		.read48 = b53_mdio_read48,
		.read64 = b53_mdio_read64,
		.write8 = b53_mdio_write8,
		.write16 = b53_mdio_write16,
		.write32 = b53_mdio_write32,
		.write48 = b53_mdio_write48,
		.write64 = b53_mdio_write64,
};


static int b53_phy_probe(struct phy_device *phydev)
{
	return b53_switch_probe(&b53_mdio_ops, phydev->bus);
}

static int b53_phy_config_init(struct phy_device *phydev)
{
	struct b53_device *dev;
	int ret;

	/* Don't do anything if using the second MII */
	if (phydev->addr == 4)
		return 0;

	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev) {
		dev_err(&phydev->dev, "no memory for private data\n");
		return -ENOMEM;
	}

	dev->priv = phydev->bus;
	dev->ops = &b53_mdio_ops;

	if ((ret = b53_switch_register(dev, &b53_mdio_ops, phydev->bus))) {
		dev_err(&phydev->dev, "switch registration failed\n");
		kfree(dev);
		return ret;
	}

	phydev->priv = dev;

	switch (dev->chip_id) {
	case BCM5325E_DEVICE_ID:
	case BCM5325F_DEVICE_ID:
	case BCM5325M_DEVICE_ID:
		phydev->supported = phydev->advertising =
			SUPPORTED_100baseT_Full;
		break;
	case BCM5395_DEVICE_ID:
	case BCM5397_DEVICE_ID:
	case BCM5398_DEVICE_ID:
	case BCM53115_DEVICE_ID:
	case BCM53125_DEVICE_ID:
		phydev->supported = phydev->advertising =
			SUPPORTED_1000baseT_Full;
		break;
	default:
		BUG();
	}

	return 0;
}

static void b53_phy_remove(struct phy_device *phydev)
{
	struct b53_device *priv = phydev->priv;

	if (!priv)
		return;

	b53_switch_remove(priv);
	phydev->priv = NULL;

	kfree(priv);
}


static int b53_phy_config_aneg(struct phy_device *phydev)
{
	if (phydev->addr == 4)
		return genphy_config_aneg(phydev);

	return 0;
}

static int b53_phy_read_status(struct phy_device *phydev)
{
	struct b53_device *priv = phydev->priv;

	if (phydev->addr == 4)
		return genphy_read_status(phydev);

	switch (priv->chip_id) {
	case BCM5325E_DEVICE_ID:
	case BCM5325F_DEVICE_ID:
	case BCM5325M_DEVICE_ID:
		phydev->speed = 100;
		break;
	case BCM5395_DEVICE_ID:
	case BCM5397_DEVICE_ID:
	case BCM5398_DEVICE_ID:
	case BCM53115_DEVICE_ID:
	case BCM53125_DEVICE_ID:
		phydev->speed = 1000;
		break;
	default:
		BUG();
	}

	phydev->duplex = DUPLEX_FULL;
	phydev->link = 1;
	phydev->state = PHY_RUNNING;

	netif_carrier_on(phydev->attached_dev);
	phydev->adjust_link(phydev->attached_dev);

	return 0;
}



static struct phy_driver b53_phy_driver = {
	.phy_id		= 0x0143bc30,
	.name		= "Broadcom B53",
	.phy_id_mask	= 0x1ffffff0,
	.features	= PHY_BASIC_FEATURES,
	.probe		= b53_phy_probe,
	.remove		= b53_phy_remove,
	.config_aneg	= b53_phy_config_aneg,
	.config_init	= b53_phy_config_init,
	.read_status	= b53_phy_read_status,
	.driver = {
		.owner = THIS_MODULE,
	},
};

int __init b53_phy_driver_register(void)
{
	return phy_driver_register(&b53_phy_driver);
}

void __exit b53_phy_driver_unregister(void)
{
	phy_driver_unregister(&b53_phy_driver);
}

