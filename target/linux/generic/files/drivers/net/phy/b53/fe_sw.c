/*
 * fe_sw.c: Fast Ethernet switches (BCM5325(E/F) specific implementation.
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

static int b53_fe_get_port_link(struct b53_device *dev, int port, u8 *link,
		u8 *speed, u8 *duplex)
{
	u16 link_reg, speed_reg, duplex_reg;

	if (port == 5)
		return -EINVAL;

	link_reg = b53_read16(dev, B53_STAT_PAGE, B53_LINK_STAT);
	speed_reg = b53_read16(dev, B53_STAT_PAGE, B53_SPEED_STAT);
	duplex_reg = b53_read16(dev, B53_STAT_PAGE, B53_DUPLEX_STAT_FE);

	*link = (link_reg >> port) & 1;
	*speed = SPEED_PORT_FE(speed_reg, port);
	*duplex = (duplex_reg >> port) & 1;

	return 0;
}

static void b53_fe_set_vlan_entry(struct b53_device *dev, u8 index, u16 members,
				  u16 untag)
{
	if (members)
		b53_write32(dev, B53_VLAN_PAGE, B53_VLAN_WRITE, VA_VALID |
			    (untag << VA_UNTAG_S) | members);
	else
		b53_write32(dev, B53_VLAN_PAGE, B53_VLAN_WRITE, 0);

	b53_write16(dev, B53_VLAN_PAGE, B53_VLAN_TABLE_ACCESS, index |
			VTA_RW_STATE_WR | VTA_RW_OP_EN);
}

static void b53_fe_set_managed(struct b53_device *dev, int enable)
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

		b53_write8(dev, B53_VLAN_PAGE, B53_VLAN_CTRL4,
			   VC4_ING_VID_VIO_DROP << VC4_ING_VID_CHECK_S);

		b53_write8(dev, B53_VLAN_PAGE, B53_VLAN_CTRL5,
			   VC5_DROP_VTABLE_MISS);
	} else {
		mgmt &= ~SM_SW_FWD_MODE;

		b53_write8(dev, B53_VLAN_PAGE, B53_VLAN_CTRL0, 0);
		b53_write8(dev, B53_VLAN_PAGE, B53_VLAN_CTRL1, 0);
		b53_write8(dev, B53_VLAN_PAGE, B53_VLAN_CTRL4, 0);
		b53_write8(dev, B53_VLAN_PAGE, B53_VLAN_CTRL5, 0);
	}

	b53_write8(dev, B53_CTRL_PAGE, B53_SWITCH_MODE, mgmt);
}

static int b53_fe_apply(struct b53_device *dev)
{
	int i;

	if (dev->enable_vlan) {
		u16 untag = 0;

		for (i = 0; i < 6; i++) {
			if (!dev->ports[i].tagged) {
				untag |= BIT(i);

				b53_write16(dev, B53_VLAN_PAGE,
					    B53_VLAN_PORT_DEF_TAG(i),
					    dev->ports[i].pvid);
			}
		}

		for (i = 0; i < 16; i++) {
			u16 vlan_untag = (untag & dev->vlans[i].members);

			/* write config now */
			dev->set_vlan_entry(dev, i, dev->vlans[i].members,
					    vlan_untag);
		}
	} else {
		/* clear all vlan entries */
		for (i = 0; i < 16; i++)
			dev->set_vlan_entry(dev, i, 0, 0);

		dev->set_vlan_entry(dev, 1, 0x3f, 0x3f);

		/* reset ports to a known good state */
		for (i = 0; i < 6; i++) {
			b53_write16(dev, B53_CTRL_PAGE,
				    B53_PORT_CTRL(i), 0x0000);
			b53_write16(dev, B53_VLAN_PAGE,
				    B53_VLAN_PORT_DEF_TAG(i), 0);
		}
	}

	return 0;
}

static int b53_fe_reset(struct b53_device *dev)
{
	int i;
	u16 mgmt = b53_read16(dev, B53_CTRL_PAGE, B53_SWITCH_MODE);

	if (!(mgmt & SM_SW_FWD_EN)) {
		mgmt &= ~SM_SW_FWD_MODE;
		mgmt |= SM_SW_FWD_EN;

		b53_write16(dev, B53_CTRL_PAGE, B53_SWITCH_MODE, mgmt);
		mgmt = b53_read16(dev, B53_CTRL_PAGE, B53_SWITCH_MODE);

		if (!(mgmt & SM_SW_FWD_EN)) {
			pr_err("Failed to enable switch!\n");
			return -EINVAL;
		}

		for (i = 0; i < (dev->sw_dev.ports - 1); i++)
			b53_write8(dev, B53_CTRL_PAGE, B53_PORT_CTRL(i), 0);

		b53_write8(dev, B53_CTRL_PAGE, B53_IM_PORT_CTRL, 0);
	}
	return 0;
}

int b53_fe_init(struct b53_device *dev)
{
	struct switch_dev *sw_dev = &dev->sw_dev;

	sw_dev->name = "BCM5325";
	sw_dev->alias = "bcm5325";
	sw_dev->cpu_port = 5;
	sw_dev->ports = 6;
	sw_dev->vlans = 16;

	dev->get_port_link = b53_fe_get_port_link;
	dev->apply = b53_fe_apply;
	dev->reset = b53_fe_reset;
	dev->set_managed = b53_fe_set_managed;
	dev->set_vlan_entry = b53_fe_set_vlan_entry;

	return b53_fe_reset(dev);
}
