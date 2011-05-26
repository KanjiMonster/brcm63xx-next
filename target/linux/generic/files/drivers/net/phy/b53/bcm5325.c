#include "b53.h"
#include "b53_priv.h"

#include <linux/kernel.h>

#define b5325_port(X)	(x == 5 ? 8 : x)


static int b53_bcm5325_get_port_link(struct b53_device *dev, int port)
{
	u32 len;
	u16 link, speed, duplex;

	link = b53_read16(dev, B53_STAT_PAGE, B53_LINK_STAT_SUMMARY);
	speed = b53_read16(dev, B53_STAT_PAGE, B53_SPEED_STAT_SUMMARY);
	duplex = b53_read16(dev, B53_STAT_PAGE, B53_DUPLEX_STAT_SUMMARY);

	if (link & (1 << port)) {
		len = snprintf(dev->buf, sizeof(dev->buf),
			       "port:%d link:up speed:%s %s-duplex",
			       port,
			       (speed & (1 << port)) ?
			       "100baseT" : "10baseT",
			       (duplex & (1 << port)) ?
			       "full" : "half");
	} else {
		len =
		    snprintf(dev->buf, sizeof(dev->buf), "port:%d link: down",
			     port);
	}
	return len;
}

static int b53_bcm5325_apply(struct b53_device *dev)
{
	int i;

	/* disable switching */
	b53_write16(dev, B53_CTRL_PAGE, B53_SWITCH_MODE,
			b53_read16(dev, B53_CTRL_PAGE, B53_SWITCH_MODE) &
		 ~(B53_SM_SW_FWD_EN));

	if (dev->enable_vlan) {
		u16 untag = 0;
		
		b53_write8(dev, B53_VLAN_PAGE, B53_VLAN_CTRL0,
				B53_VC0_VLAN_EN | B53_VC0_VID_CHK_EN | B53_VC0_VID_HASH_VID | B53_VC0_DROP_VID_MISS);

		b53_write8(dev, B53_VLAN_PAGE, B53_VLAN_CTRL1,
				B53_VC1_RX_MCST_TAG_EN | B53_VC1_RX_MCST_FWD_EN | B53_VC1_RX_MCST_UNTAG_EN);

		b53_write8(dev, B53_VLAN_PAGE, B53_VLAN_CTRL4,
				B53_VC4_ING_VID_VIO_DROP << B53_VC4_ING_VID_CHECK_S);

		b53_write8(dev, B53_VLAN_PAGE, B53_VLAN_CTRL5,
				B53_VC5_DROP_VTABLE_MISS);

		for (i = 0; i < 6; i++) {
			if (!dev->ports[i].eg_tagged)
				untag |= BIT(i);

			b53_write16(dev, B53_VLAN_PAGE,
				BCM53XX_VLAN_PORT_DEF_TAG(i), dev->ports[i].pvid + dev->vlan_base);
		}

		for (i = 0; i < 16; i++) {
			u16 vlan_untag = (untag & dev->vlans[i].members);			
			u16 vlan_base = dev->vlan_base >> 4;

			/* write config now */
			b53_write32(dev, B53_VLAN_PAGE, BCM5325_VLAN_WRITE,
					(dev->vlans[i].valid ? BCM5325_VA_VALID : 0) |
					(vlan_base << BCM5325_VA_VID_HIGH_S) |
					(vlan_untag << BCM5325_VA_UNTAG_S) |
					dev->vlans[i].members);

			b53_write16(dev, B53_VLAN_PAGE,
					BCM5325_VLAN_TABLE_ACCESS, i | 
					BCM53XX_VTA_RW_STATE_WR | 
					BCM53XX_VTA_RW_OP_EN);
		}
		/* set the vlan base */
		
	} else {
		/* clear all vlan entries */
		for (i = 0; i < 16; i++) {
			b53_write32(dev, B53_VLAN_PAGE,
					BCM5325_VLAN_WRITE, 0);
			b53_write16(dev, B53_VLAN_PAGE,
					BCM5325_VLAN_TABLE_ACCESS, i | BCM53XX_VTA_RW_STATE_WR | BCM53XX_VTA_RW_OP_EN);
		}
		
		b53_write32(dev, B53_VLAN_PAGE,
				BCM5325_VLAN_WRITE, BCM5325_VA_VALID | (0x3f << BCM5325_VA_UNTAG_S) | 0x3f);
		b53_write16(dev, B53_VLAN_PAGE,
				BCM5325_VLAN_TABLE_ACCESS, 0 | BCM53XX_VTA_RW_STATE_WR | BCM53XX_VTA_RW_OP_EN);
		
		/* reset ports to a known good state */
		for (i = 0; i < 6; i++) {
			b53_write16(dev, BCM53XX_CTRL_PAGE,
					BCM53XX_PORT_CTRL(b5325_port(i)), 0x0000);
			b53_write16(dev, B53_VLAN_PAGE,
					BCM53XX_VLAN_PORT_DEF_TAG(i), 1);
		}

		b53_write8(dev, B53_VLAN_PAGE, B53_VLAN_CTRL0, 0);
		b53_write8(dev, B53_VLAN_PAGE, B53_VLAN_CTRL1, 0);
		b53_write8(dev, B53_VLAN_PAGE, B53_VLAN_CTRL4, 0);
		b53_write8(dev, B53_VLAN_PAGE, B53_VLAN_CTRL5, 0);

	}

	/* enable switching */
	b53_write16(dev, BCM53XX_CTRL_PAGE, BCM53XX_SWITCH_MODE,
		b53_read16(dev, BCM53XX_CTRL_PAGE, BCM53XX_SWITCH_MODE) |
			BCM53XX_SM_SW_FWD_EN);

	return 0;
}


int b53_bcm5325_init(struct b53_device *dev)
{
	struct switch_dev *sw_dev = &dev->sw_dev;
	
	sw_dev->name = "BCM5325";
	sw_dev->devname = "bcm5325";
	sw_dev->cpu_port = 5;
	sw_dev->ports = 6;
	sw_dev->vlans = 16;

	dev->get_port_link = b53_bcm5325_get_port_link;
	dev->apply = b53_bcm5325_apply;	
	
	return 0;
}
