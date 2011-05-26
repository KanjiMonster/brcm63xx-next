#ifndef __B53_PRIV_H
#define __B53_PRIV_H

#include <linux/kernel.h>
#include <linux/switch.h>

struct b53_device;

struct b53_io_ops {
	u8 (*read8)(struct b53_device *dev, u8 page, u8 reg);
	u16 (*read16)(struct b53_device *dev, u8 page, u8 reg);
	u32 (*read32)(struct b53_device *dev, u8 page, u8 reg);
	u64 (*read48)(struct b53_device *dev, u8 page, u8 reg);
	u64 (*read64)(struct b53_device *dev, u8 page, u8 reg);
	void (*write8)(struct b53_device *dev, u8 page, u8 reg, u8 value);
	void (*write16)(struct b53_device *dev, u8 page, u8 reg, u16 value);
	void (*write32)(struct b53_device *dev, u8 page, u8 reg, u32 value);
	void (*write48)(struct b53_device *dev, u8 page, u8 reg, u64 value);
	void (*write64)(struct b53_device *dev, u8 page, u8 reg, u64 value);
};

enum {
	BCM5325E_DEVICE_ID = 0x25,
	BCM5325F_DEVICE_ID = 0x26,
	BCM5325M_DEVICE_ID = 0x27,
	BCM5395_DEVICE_ID = 0x95,
	BCM5397_DEVICE_ID = 0x97,
	BCM5398_DEVICE_ID = 0x98,
	BCM53115_DEVICE_ID = 0x3115,
};


struct b53_vlan {
	unsigned int	members:9;
	unsigned int	valid:1;
};

struct b53_port {
	unsigned int	pvid:12;
	unsigned int	eg_tagged:1;
	unsigned int	in_tagged:1;
};

struct b53_device {
	const struct b53_io_ops *ops;
	void *priv;

	int (*apply)(struct b53_device *dev);
	int (*get_port_link)(struct b53_device *dev, int port);
	
	struct switch_dev sw_dev;

	u16 chip_id;

	int enable_vlan;
	u16 vlan_base;
	
	struct b53_port ports[9];
	struct b53_vlan vlans[256];

	char buf[512];
};


static inline struct b53_device *sw_to_b53(struct switch_dev *sw)
{
	return container_of(sw, struct b53_device, sw_dev);
}

int b53_switch_probe(const struct b53_io_ops *ops, void *priv);
int b53_switch_register(struct b53_device *dev, const struct b53_io_ops *ops, void *priv);

static inline void b53_switch_remove(struct b53_device *dev)
{
	unregister_switch(&dev->sw_dev);
}

static inline u8 b53_read8(struct b53_device *dev, u8 page, u8 reg)
{
	pr_info("b53_read8(%02x, %02x)\n", page, reg);
	return dev->ops->read8(dev, page, reg);
}

static inline u16 b53_read16(struct b53_device *dev, u8 page, u8 reg)
{
	pr_info("b53_read16(%02x, %02x)\n", page, reg);
	return dev->ops->read16(dev, page, reg);
}

static inline u32 b53_read32(struct b53_device *dev, u8 page, u8 reg)
{
	pr_info("b53_read32(%02x, %02x)\n", page, reg);
	return dev->ops->read32(dev, page, reg);
}

static inline u64 b53_read48(struct b53_device *dev, u8 page, u8 reg)
{
	pr_info("b53_read48(%02x, %02x)\n", page, reg);
	return dev->ops->read48(dev, page, reg);
}

static inline u64 b53_read64(struct b53_device *dev, u8 page, u8 reg)
{
	pr_info("b53_read64(%02x, %02x)\n", page, reg);
	return dev->ops->read64(dev, page, reg);
}

static inline void b53_write8(struct b53_device *dev, u8 page, u8 reg, u8 value)
{
	pr_info("b53_write8(%02x, %02x) -> %02x\n", page, reg, value);
	dev->ops->write8(dev, page, reg, value);
}

static inline void b53_write16(struct b53_device *dev, u8 page, u8 reg, u16 value)
{
	pr_info("b53_write16(%02x, %02x) -> %04x\n", page, reg, value);
	dev->ops->write16(dev, page, reg, value);
}

static inline void b53_write32(struct b53_device *dev, u8 page, u8 reg, u32 value)
{
	pr_info("b53_write32(%02x, %02x) -> %08x\n", page, reg, value);
	dev->ops->write32(dev, page, reg, value);
}

static inline void b53_write48(struct b53_device *dev, u8 page, u8 reg, u64 value)
{
	pr_info("b53_write48(%02x, %02x) -> %12x\n", page, reg, value);
	dev->ops->write48(dev, page, reg, value);
}

static inline void b53_write64(struct b53_device *dev, u8 page, u8 reg, u64 value)
{
	pr_info("b53_write64(%02x, %02x) -> %16x\n", page, reg, value);
	dev->ops->write64(dev, page, reg, value);
}

#ifdef CONFIG_B53_BCM5325
int b53_bcm5325_init(struct b53_device *dev);
#else
static inline int b53_bcm5325_init(struct b53_device *dev) { return -EINVAL; }
#endif

#ifdef CONFIG_B53_BCM5395_SUPPORT
int b53_bcm5395_init(struct b53_device *dev);
#else
static inline int b53_bcm5395_init(struct b53_device *dev) { return -EINVAL; }
#endif

#ifdef CONFIG_B53_BCM5397_SUPPORT
int b53_bcm5397_init(struct b53_device *dev);
#else
static inline int b53_bcm5397_init(struct b53_device *dev) { return -EINVAL; }
#endif

#ifdef CONFIG_B53_BCM5398_SUPPORT
int b53_bcm5398_init(struct b53_device *dev);
#else
static inline int b53_bcm5398_init(struct b53_device *dev) { return -EINVAL; }
#endif

#ifdef CONFIG_B53_BCM53115_SUPPORT
int b53_bcm53115_init(struct b53_device *dev);
#else
static inline int b53_bcm53115_init(struct b53_device *dev) { return -EINVAL; }
#endif


#ifdef CONFIG_B53_PHY_DRIVER
int __init b53_phy_driver_register(void);
void __exit b53_phy_driver_unregister(void);
#else
static inline int b53_phy_driver_register() { return 0; }
static inline void b53_phy_driver_unregister() {}
#endif

#ifdef CONFIG_B53_SPI_DRIVER
int __init b53_spi_driver_register(void);
void __exit b53_spi_driver_unregister(void);
#else
static inline int b53_phy_driver_register() { return 0; }
}
static inline void b53_phy_driver_unregister() {}
#endif


#endif
