/*
 * ge_sw.c: Gigabit capable switches specific implementation.
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

static int b53_ge_get_port_link(struct b53_device *dev, int port, u8 *link,
				u8 *speed, u8 *duplex)
{
	u32 speed_reg;
	u16 link_reg, duplex_reg;

	if (port == 8)
		return -EINVAL;

	link_reg = b53_read16(dev, B53_STAT_PAGE, B53_LINK_STAT);
	speed_reg = b53_read32(dev, B53_STAT_PAGE, B53_SPEED_STAT);
	duplex_reg = b53_read16(dev, B53_STAT_PAGE, B53_DUPLEX_STAT_GE);

	*link = link_reg & (1 << port);
	*speed = SPEED_PORT_GE(speed_reg, port);
	*duplex = duplex_reg & (1 << port);

	return 0;
}

static void b53_ge_set_vlan_entry_9798(struct b53_device *dev, u8 index,
				       u16 members, u16 untag)
{
	b53_write8(dev, B53_ARLIO_PAGE, B53_VT_INDEX_9798, index);
	b53_write32(dev, B53_ARLIO_PAGE, B53_VT_ENTRY_9798,
		    (untag << VTE_UNTAG_S) | members);
	b53_write16(dev, B53_ARLIO_PAGE, B53_VT_ACCESS_9798, VTA_START_CMD);
}

static void b53_ge_set_vlan_entry(struct b53_device *dev, u8 index, u16 members,
				  u16 untag)
{
	b53_write8(dev, B53_ARLIO_PAGE, B53_VT_INDEX, index);
	b53_write32(dev, B53_ARLIO_PAGE, B53_VT_ENTRY,
		    (untag << VTE_UNTAG_S) | members);
	b53_write16(dev, B53_ARLIO_PAGE, B53_VT_ACCESS, VTA_START_CMD);
}

static void b53_ge_set_managed(struct b53_device *dev, int enable)
{
	u8 mgmt = b53_read8(dev, B53_CTRL_PAGE, B53_SWITCH_MODE);

	if (enable) {
		mgmt |= SM_SW_FWD_MODE;

		b53_write8(dev, B53_VLAN_PAGE, B53_VLAN_CTRL0,
			   VC0_VLAN_EN | VC0_VID_CHK_EN |
			   VC0_VID_HASH_VID);

		b53_write8(dev, B53_VLAN_PAGE, B53_VLAN_CTRL1,
			   VC1_RX_MCST_TAG_EN | VC1_RX_MCST_FWD_EN |
			   VC1_RX_MCST_UNTAG_EN);
	} else {
		mgmt &= ~SM_SW_FWD_MODE;

		b53_write8(dev, B53_VLAN_PAGE, B53_VLAN_CTRL0, 0);
		b53_write8(dev, B53_VLAN_PAGE, B53_VLAN_CTRL1, 0);
	}

	b53_write8(dev, B53_CTRL_PAGE, B53_SWITCH_MODE, mgmt);
}

static int b53_ge_apply(struct b53_device *dev)
{
	int i;

	if (dev->enable_vlan) {
		u16 untag = 0;

		for (i = 0; i < 9; i++) {
			if (!dev->ports[i].tagged) {
				untag |= BIT(i);

				b53_write16(dev, B53_VLAN_PAGE,
					    B53_VLAN_PORT_DEF_TAG(i),
					    dev->ports[i].pvid);
			}
		}

		for (i = 1; i < 16; i++) {
			u16 vlan_untag = (untag & dev->vlans[i].members);

			dev->set_vlan_entry(dev, i, dev->vlans[i].members,
					    vlan_untag);
		}
	} else {
		int max_port;

		/* clear all vlan entries */
		for (i = 1; i < 16; i++) {
			dev->set_vlan_entry(dev, i, 0, 0);
		}

		/* reset ports to a known good state */
		if (dev->chip_id == BCM5398_DEVICE_ID)
			max_port = 7;
		else if (dev->chip_id == BCM53115_DEVICE_ID)
			max_port = 5;
		else
			max_port = 4;

		for (i = 0; i <= max_port; i++)
			b53_write8(dev, B53_CTRL_PAGE, B53_PORT_CTRL(i), 0);

		b53_write8(dev, B53_CTRL_PAGE, B53_IM_PORT_CTRL, 0);

		for (i = 0; i < 9; i++) {
			b53_write16(dev, B53_VLAN_PAGE,
					B53_VLAN_PORT_DEF_TAG(i), 0);
		}
	}

	return 0;
}

static int b53_ge_reset(struct b53_device *dev)
{
	int i;
	u8 mgmt = b53_read8(dev, B53_CTRL_PAGE, B53_SWITCH_MODE);

	if (!(mgmt & SM_SW_FWD_EN)) {
		int max_port;

		mgmt &= ~SM_SW_FWD_MODE;
		mgmt |= SM_SW_FWD_EN;

		b53_write8(dev, B53_CTRL_PAGE, B53_SWITCH_MODE, mgmt);
		mgmt = b53_read8(dev, B53_CTRL_PAGE, B53_SWITCH_MODE);

		if (!(mgmt & SM_SW_FWD_EN)) {
			pr_err("Failed to enable switch!\n");
			return -EINVAL;
		}

		if (dev->chip_id == BCM5398_DEVICE_ID)
			max_port = 7;
		else if (dev->chip_id == BCM53115_DEVICE_ID)
			max_port = 5;
		else
			max_port = 4;

		for (i = 0; i <= max_port; i++)
			b53_write8(dev, B53_CTRL_PAGE, B53_PORT_CTRL(i), 0);

		b53_write8(dev, B53_CTRL_PAGE, B53_IM_PORT_CTRL, 0);

	}

	if (dev->chip_id == BCM53115_DEVICE_ID ||
	    dev->chip_id == BCM53125_DEVICE_ID) {
		u8 mii_port_override = b53_read8(dev, B53_CTRL_PAGE,
					   B53_PORT_OVERRIDE_CTRL);
		b53_write8(dev, B53_CTRL_PAGE, B53_PORT_OVERRIDE_CTRL,
			   mii_port_override | 0x81);
	}

	return 0;
}

int b53_ge_init(struct b53_device *dev)
{
	struct switch_dev *sw_dev = &dev->sw_dev;

	switch (dev->chip_id) {
	case BCM5395_DEVICE_ID:
		sw_dev->name = "BCM5395";
		sw_dev->alias = "bcm5395";
		break;
	case BCM5397_DEVICE_ID:
		sw_dev->name = "BCM5397";
		sw_dev->alias = "bcm5397";
		break;
	case BCM5398_DEVICE_ID:
		sw_dev->name = "BCM5398";
		sw_dev->alias = "bcm5398";
		break;
	case BCM53115_DEVICE_ID:
		sw_dev->name = "BCM53115";
		sw_dev->alias = "bcm53115";
		break;
	case BCM53125_DEVICE_ID:
		sw_dev->name = "BCM53125";
		sw_dev->alias = "bcm53125";
		break;
	}
	sw_dev->cpu_port = 8;
	sw_dev->ports = 9;
	sw_dev->vlans = 16;

	dev->get_port_link = b53_ge_get_port_link;
	dev->apply = b53_ge_apply;
	dev->reset = b53_ge_reset;
	dev->set_managed = b53_ge_set_managed;
	if (is5397_98(dev))
		dev->set_vlan_entry = b53_ge_set_vlan_entry_9798;
	else
		dev->set_vlan_entry = b53_ge_set_vlan_entry;

	return b53_ge_reset(dev);
}
