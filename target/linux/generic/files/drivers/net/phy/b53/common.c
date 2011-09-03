/*
 * common.c: B53 common functions
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
#include <linux/module.h>

int b53_detect_chip(struct b53_device *dev)
{
	u16 id;
	u8 vc4;
	int i;

	id = b53_read8(dev, B53_MGMT_PAGE, B53_DEVICE_ID);
	for (i = 0; i < 10 && id == 0; i++)
		id = b53_read8(dev, B53_MGMT_PAGE, B53_DEVICE_ID);

	switch (id) {
	case 0:
		/* BCM5325 does not have a this register so it returns
		 * 0, so assume it's one. */

		/* find out the specific version: */
		vc4 = b53_read8(dev, B53_VLAN_PAGE, B53_VLAN_CTRL4);
		switch (vc4) {
		case 1:
			dev->chip_id = BCM5325E_DEVICE_ID;
			break;
		case 3:
			dev->chip_id = BCM5325F_DEVICE_ID;
			break;
		default:
			dev->chip_id = BCM5325M_DEVICE_ID;
			break;
		}
		break;
	case BCM5395_DEVICE_ID:
	case BCM5397_DEVICE_ID:
	case BCM5398_DEVICE_ID:
		dev->chip_id = id;
		break;
	default:
		/* check if we have a 16 bit device id */
		id = b53_read16(dev, B53_MGMT_PAGE, B53_DEVICE_ID);

		switch (id) {
		case BCM53115_DEVICE_ID:
		case BCM53125_DEVICE_ID:
			dev->chip_id = id;
			break;
		default:
			pr_err("unsupported switch id=%x\n", id);
			return -ENODEV;
		}
	}

	dev->core_rev = b53_read8(dev, B53_STAT_PAGE, B53_CORE_REV);

	return 0;
}

static int b53_get_vlan_enable(struct switch_dev *dev,
				      const struct switch_attr *attr,
				      struct switch_val *val)
{
	struct b53_device *priv = sw_to_b53(dev);

	val->value.i = priv->enable_vlan;

	return 0;
}

int b53_set_vlan_enable(struct switch_dev *dev,
				      const struct switch_attr *attr,
				      struct switch_val *val)
{
	struct b53_device *priv = sw_to_b53(dev);

	priv->enable_vlan = val->value.i;

	return 0;
}

static int b53_get_port_pvid(struct switch_dev *dev, int port, int *val)
{
	struct b53_device *priv = sw_to_b53(dev);

	*val = priv->ports[port].pvid;

	return 0;
}

static int b53_set_port_pvid(struct switch_dev *dev, int port, int val)
{
	struct b53_device *priv = sw_to_b53(dev);

	if (val > 15)
		return -EINVAL;

	priv->ports[port].pvid = val;

	return 0;
}

static int b53_get_vlan_ports(struct switch_dev *dev,
				     struct switch_val *val)
{
	struct b53_device *priv = sw_to_b53(dev);
	struct switch_port *port;
	struct b53_vlan *vlan;
	int i;

	vlan = &priv->vlans[val->port_vlan];

	if (!vlan->valid)
		return 0;

	port = &val->value.ports[0];
	val->len = 0;
	for (i = 0; i < dev->ports; i++) {
		if (!(vlan->members & BIT(i)))
			continue;

		port->id = i;

		if (priv->ports[i].tagged)
			port->flags = BIT(SWITCH_PORT_FLAG_TAGGED);
		else
			port->flags = 0;

		val->len++;
		port++;
	}

	return 0;
}

static int b53_set_vlan_ports(struct switch_dev *dev,
			      struct switch_val *val)
{
	struct b53_device *priv = sw_to_b53(dev);
	struct switch_port *port;
	int i;

	/* only BCM5325x supports VID 0 */
	if (val->port_vlan == 0 && !is5325(priv))
		return -EINVAL;

	port = &val->value.ports[0];
	for (i = 0; i < val->len; i++, port++) {
		priv->vlans[val->port_vlan].members |= BIT(port->id);

		if (!(port->flags & BIT(SWITCH_PORT_FLAG_TAGGED))) {
			priv->ports[port->id].tagged = 0;
			priv->ports[port->id].pvid = val->port_vlan;
		} else {
			priv->ports[port->id].tagged = 1;
		}
	}

	priv->vlans[val->port_vlan].valid = (val->len > 0);

	return 0;
}

static int b53_get_port_link(struct switch_dev *dev,
			     int port,
			     struct switch_port_state *state)
{
	struct b53_device *priv = sw_to_b53(dev);
	u8 link, speed, duplex;
	int ret;

	ret = priv->get_port_link(priv, port, &link, &speed, &duplex);
	if (ret)
		return ret;

	if (link) {
		state->link = 1;
		state->speed = speed;
		state->duplex = duplex;
	} else {
		state->link = 0;
	}

	return 0;
}

static void b53_set_forwarding(struct b53_device *dev, int enable)
{
	u8 mgmt = b53_read8(dev, B53_CTRL_PAGE, B53_SWITCH_MODE);

	if (enable)
		mgmt |= SM_SW_FWD_MODE;
	else
		mgmt &= ~SM_SW_FWD_MODE;

	b53_write8(dev, B53_CTRL_PAGE, B53_SWITCH_MODE, mgmt);
}

static int b53_reset_switch(struct switch_dev *dev)
{
	struct b53_device *priv = sw_to_b53(dev);
	int i;

	/* reset vlans */
	priv->enable_vlan = 0;

	for (i = 0; i <= dev->vlans; i++) {
		priv->vlans[i].members = 0;
		priv->vlans[i].valid = 0;
	}

	for (i = 0; i < dev->ports; i++) {
		priv->ports[i].tagged = 0;
		priv->ports[i].pvid = 0;
	}

	return priv->reset(priv);
}

static int b53_apply_config(struct switch_dev *dev)
{
	struct b53_device *priv = sw_to_b53(dev);

	/* disable switching */
	b53_set_forwarding(priv, 0);

	priv->set_managed(priv, priv->enable_vlan);
	priv->apply(priv);

	/* enable switching */
	b53_set_forwarding(priv, 1);

	return 0;
}

static struct switch_attr b53_global_ops[] = {
	{
		.type = SWITCH_TYPE_INT,
		.name = "enable_vlan",
		.description = "Enable VLAN mode",
		.set = b53_set_vlan_enable,
		.get = b53_get_vlan_enable,
		.max = 1,
	},
};

static struct switch_attr b53_port_ops[] = {
};

static struct switch_attr b53_vlan_ops[] = {
};

static const struct switch_dev_ops b53_switch_ops = {
	.attr_global = {
		.attr = b53_global_ops,
		.n_attr = ARRAY_SIZE(b53_global_ops),
	},
	.attr_port = {
		.attr = b53_port_ops,
		.n_attr = ARRAY_SIZE(b53_port_ops),
	},
	.attr_vlan = {
		.attr = b53_vlan_ops,
		.n_attr = ARRAY_SIZE(b53_vlan_ops),
	},

	.get_port_pvid = b53_get_port_pvid,
	.set_port_pvid = b53_set_port_pvid,
	.get_port_link = b53_get_port_link,
	.get_vlan_ports = b53_get_vlan_ports,
	.set_vlan_ports = b53_set_vlan_ports,
	.apply_config = b53_apply_config,
	.reset_switch = b53_reset_switch,
};

int b53_switch_probe(const struct b53_io_ops *ops, void *priv)
{
	struct b53_device dev;
	dev.ops = ops;
	dev.priv = priv;
	return b53_detect_chip(&dev);
}

int b53_switch_register(struct b53_device *dev, const struct b53_io_ops *ops,
			void *priv)
{
	int ret;

	dev->priv = priv;
	dev->ops = ops;
	dev->sw_dev.ops = &b53_switch_ops;

	if (b53_detect_chip(dev))
		return -EINVAL;

	switch (dev->chip_id) {
	case BCM5325E_DEVICE_ID:
	case BCM5325F_DEVICE_ID:
	case BCM5325M_DEVICE_ID:
		ret = b53_fe_init(dev);
		break;
	case BCM5395_DEVICE_ID:
	case BCM5397_DEVICE_ID:
	case BCM5398_DEVICE_ID:
	case BCM53115_DEVICE_ID:
	case BCM53125_DEVICE_ID:
		ret = b53_ge_init(dev);
		break;
	default:
		pr_err("unsupported switch id=%x\n", dev->chip_id);
		return -EINVAL;
	}

	if (ret) {
		pr_err("switch initialisation failed: %d\n", ret);
		return ret;
	}

	return register_switch(&dev->sw_dev, NULL);
}


static int __init b53_init(void)
{
	int ret = b53_phy_driver_register();

	if (!ret)
		ret = b53_spi_driver_register();

	return ret;
}

module_init(b53_init);

static void __exit b53_exit(void)
{
	b53_spi_driver_unregister();
	b53_phy_driver_unregister();
}

module_exit(b53_exit);

MODULE_DESCRIPTION("B53 switch driver");
MODULE_LICENSE("GPL");
