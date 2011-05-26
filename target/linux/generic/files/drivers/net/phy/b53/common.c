#include "b53_priv.h"
#include "b53.h"

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
	case 0x00:
	case 0x01:
		/* find out the specific version */
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
		pr_err("BCM53%x not supported", id);
		return -ENODEV;
	default:
		/* check if we have a 16 bit device id */
		id = b53_read16(dev, B53_MGMT_PAGE, B53_DEVICE_ID);
		
		if (id == BCM53115_DEVICE_ID)
			pr_err("BCM53115 not supported");
		else
			pr_err("unsupported switch id=%x\n", id);
		
		return -ENODEV;
	}
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

static int b53_get_vlan_base(struct switch_dev *dev,
				      const struct switch_attr *attr,
				      struct switch_val *val)
{
	struct b53_device *priv = sw_to_b53(dev);

	val->value.i = priv->vlan_base;

	return 0;
}

int b53_set_vlan_base(struct switch_dev *dev,
				      const struct switch_attr *attr,
				      struct switch_val *val)
{
	struct b53_device *priv = sw_to_b53(dev);

	priv->vlan_base = (val->value.i & ~(priv->sw_dev.vlans - 1));

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
		
		if (priv->ports[i].eg_tagged)
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

	port = &val->value.ports[0];
	for (i = 0; i < val->len; i++, port++) {
		priv->vlans[val->port_vlan].members |= BIT(port->id);		
		priv->ports[port->id].in_tagged = 1;
		
		if (!(port->flags & BIT(SWITCH_PORT_FLAG_TAGGED))) {
			priv->ports[port->id].eg_tagged = 0;
			priv->ports[port->id].pvid = val->port_vlan;
		} else { 
			priv->ports[port->id].eg_tagged = 1;
		}
	}
	
	priv->vlans[val->port_vlan].valid = (val->len > 0);

	return 0;
}

static int b53_get_port_link(struct switch_dev *dev,
				    const struct switch_attr *attr,
				    struct switch_val *val)
{
	struct b53_device *priv = sw_to_b53(dev);
	
	val->len = priv->get_port_link(priv, val->port_vlan);
	val->value.s = priv->buf;	

	return 0;
}

static int b53_reset_switch(struct switch_dev *dev)
{
	struct b53_device *priv = sw_to_b53(dev);
	int i;

	/* reset vlans */
	priv->enable_vlan = 0;
	priv->vlan_base = 0;
	
	for (i = 0; i <= dev->vlans; i++) {
		priv->vlans[i].members = 0;
		priv->vlans[i].valid = 0;
	}
	
	priv->vlans[0].valid = 1;
	for (i = 0; i < dev->ports; i++) {
		priv->ports[i].eg_tagged = 0;
		priv->ports[i].pvid = 0;
	}

	return priv->apply(priv);
}

static int b53_apply_config(struct switch_dev *dev)
{
	struct b53_device *priv = sw_to_b53(dev);
	return priv->apply(priv);
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
	{
		.type = SWITCH_TYPE_INT,
		.name = "vlan_base",
		.description = "VLAN base (added to very VLAN's VID)",
		.set = b53_set_vlan_base,
		.get = b53_get_vlan_base,
		.max = 4095,
	}
};

static struct switch_attr b53_port_ops[] = {
	{
		.type = SWITCH_TYPE_STRING,
		.name = "link",
		.description = "Get port link information",
		.max = 1,
		.set = NULL,
		.get = b53_get_port_link,
	},
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

int b53_switch_register(struct b53_device *dev, const struct b53_io_ops *ops, void *priv)
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
		ret = b53_bcm5325_init(dev);
		break;
	case BCM5395_DEVICE_ID:
		ret = b53_bcm5395_init(dev);
		break;
	case BCM5397_DEVICE_ID:
		ret = b53_bcm5397_init(dev);
		break;
	case BCM5398_DEVICE_ID:
		ret = b53_bcm5398_init(dev);
		break;
	case BCM53115_DEVICE_ID:
		ret = b53_bcm53115_init(dev);
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
	
} module_exit(b53_exit);

MODULE_DESCRIPTION("B53 switch driver");
MODULE_VERSION("0.0.1");
MODULE_LICENSE("GPL");
