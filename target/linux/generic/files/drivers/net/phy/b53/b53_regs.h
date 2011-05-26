/*
 * b53_regs.h: B53 register defintions
 *
 * Copyright (C) 2004 Broadcom Corporation
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

#ifndef __B53_REGS_H
#define __B53_REGS_H



/* Management Port (SMP) Page offsets */
#define B53_CTRL_PAGE			0x00 /* Control */
#define B53_STAT_PAGE			0x01 /* Status */
#define B53_MGMT_PAGE			0x02 /* Management Mode */
#define B53_MIB_AC_PAGE			0x03 /* MIB Autocast */
#define B53_ARLCTRL_PAGE		0x04 /* ARL Control */
#define B53_ARLIO_PAGE			0x05 /* ARL Access */
#define B53_FRAMEBUF_PAGE		0x06 /* Management frame access */
#define B53_MEM_ACCESS_PAGE		0x08 /* Memory access */

/* PHY Registers */
#define B53_PORT_MII_PAGE(i)		(0x10 + i) /* Port i MII Registers */
#define B53_IM_PORT_PAGE		0x18 /* Inverse MII Port (to EMAC) */
#define B53_ALL_PORT_PAGE		0x19 /* All ports MII (broadcast) */

/* Quality of Service (QoS) Registers */
#define B53_QOS_PAGE			0x30 /* QoS Registers */

/* VLAN Registers */
#define B53_VLAN_PAGE			0x34 /* VLAN Registers */

/* Note SPI Data/IO Registers not used */
#define B53_SPI_DATA_IO_0_PAGE		0xf0 /* SPI Data I/O 0 */
#define B53_SPI_DATA_IO_1_PAGE		0xf1 /* SPI Data I/O 1 */
#define B53_SPI_DATA_IO_2_PAGE		0xf2 /* SPI Data I/O 2 */
#define B53_SPI_DATA_IO_3_PAGE		0xf3 /* SPI Data I/O 3 */
#define B53_SPI_DATA_IO_4_PAGE		0xf4 /* SPI Data I/O 4 */
#define B53_SPI_DATA_IO_5_PAGE		0xf5 /* SPI Data I/O 5 */
#define B53_SPI_DATA_IO_6_PAGE		0xf6 /* SPI Data I/O 6 */
#define B53_SPI_DATA_IO_7_PAGE		0xf7 /* SPI Data I/O 7 */

#define B53_SPI_STATUS_PAGE		0xfe /* SPI Status Registers */
#define B53_PAGE_PAGE			0xff /* Page Registers */

/*************************************************************************
 * Control Page registers
 *************************************************************************/

/* Port Control Register (8 bit) */
#define B53_PORT_CTRL(i)		(0x00 + i)
#define B53_IM_PORT_CTRL		0x08	/* Inverse MII port */
#define   PORT_CTRL_RX_DISABLE		BIT(0)
#define   PORT_CTRL_TX_DISABLE		BIT(1)
#define   PORT_CTRL_RX_BCST_EN		BIT(2) /* Broadcast RX (IM only) */
#define   PORT_CTRL_RX_MCST_EN		BIT(3) /* Multicast RX (IM only) */
#define   PORT_CTRL_RX_UCST_EN		BIT(4) /* Unicast RX (IM only) */
#define	  PORT_CTRL_STP_STATE_S		5
#define   PORT_CTRL_STP_STATE_MASK	(0x3 << PORT_CTRL_STP_STATE_S)

/* SMP Control Register (8 bit) */
#define B53_SMP_CTRL			0x0a

/* Switch Mode Control Register (8 bit) */
#define B53_SWITCH_MODE			0x0b
#define   SM_SW_FWD_MODE		BIT(0)	/* 1 = Managed Mode */
#define   SM_SW_FWD_EN			BIT(1)	/* Forwarding Enable */

/* Port state override register (8 bit) */
#define B53_PORT_OVERRIDE_CTRL		0x0e
#define   PORT_OVERRIDE_RVMII		BIT(4)	/* Enable Reverse MII */

/* Power-down mode control */
#define B53_PD_MODE_CTRL_REG		0x0f

/* IP Multicast control */
#define B53_IP_MULTICAST_CTRL		0x21

/* Software reset register*/
#define B53_SOFTRESET			0x79

/*************************************************************************
 * Status Page registers
 *************************************************************************/

/* Link Status Summary Register (16bit) */
#define B53_LINK_STAT			0x00

/* Link Status Change Register (16 bit) */
#define B53_LINK_STAT_CHANGE		0x02

/* Port Speed Summary Register (16 bit for FE, 32 bit for GE) */
#define B53_SPEED_STAT			0x04
#define  SPEED_PORT_FE(reg, port)	(((reg) >> (port)) & 1)
#define  SPEED_PORT_GE(reg, port)	(((reg) >> 2 * (port)) & 3)
#define  SPEED_STAT_10M			0
#define  SPEED_STAT_100M		1
#define  SPEED_STAT_1000M		2

/* Duplex Status Summary (16 bit) */
#define B53_DUPLEX_STAT_FE		0x06
#define B53_DUPLEX_STAT_GE		0x08

/* Core Revision Register (8 bit) */
#define B53_CORE_REV			0x50

/*************************************************************************
 * Management Mode Page Registers
 *************************************************************************/

/* Global Management Config Register (8 bit) */
#define B53_GLOBAL_CONFIG		0x00
#define   GC_RESET_MIB			0x01
#define   GC_RX_BPDU_EN			0x02
#define   GC_MIB_AC_HDR_EN		0x10
#define   GC_MIB_AC_EN			0x20
#define   GC_FRM_MGMT_PORT_M		0xC0
#define   GC_FRM_MGMT_PORT_04		0x00
#define   GC_FRM_MGMT_PORT_MII		0x80

/* Device ID register (16 bit) */
#define B53_DEVICE_ID			0x30

/*************************************************************************
 * ARL Access Page Registers
 *************************************************************************/

/* VLAN Table Access Register (8 bit) */
#define B53_VT_ACCESS			0x80
#define B53_VT_ACCESS_9798		0x60 /* for BCM5397/BCM5398 */
#define   VTA_START_CMD			BIT(7)

/* VLAN Table Index Register (16 bit) */
#define B53_VT_INDEX			0x81
#define B53_VT_INDEX_9798		0x61

/* VLAN Table Entry Register (32 bit) */
#define B53_VT_ENTRY			0x83
#define B53_VT_ENTRY_9798		0x63
#define   VTE_MEMBERS			0x1ff
#define   VTE_UNTAG_S			9
#define   VTE_UNTAG			(0x1ff << 9)

/*************************************************************************
 * VLAN Page Registers
 *************************************************************************/

/* VLAN Control 0 (8 bit) */
#define B53_VLAN_CTRL0			0x00
#define   VC0_DROP_VID_MISS		BIT(4)
#define   VC0_VID_HASH_VID		BIT(5)
#define   VC0_VID_CHK_EN		BIT(6)	/* Use VID,DA or VID,SA */
#define   VC0_VLAN_EN			BIT(7)	/* 802.1Q VLAN Enabled */

/* VLAN Control 1 (8 bit) */
#define B53_VLAN_CTRL1			0x01
#define   VC1_RX_MCST_TAG_EN		BIT(1)
#define   VC1_RX_MCST_FWD_EN		BIT(2)
#define   VC1_RX_MCST_UNTAG_EN		BIT(3)

/* VLAN Control 2 (8 bit) */
#define B53_VLAN_CTRL2			0x02

/* VLAN Control 3 (8 bit) */
#define B53_VLAN_CTRL3			0x03

/* VLAN Control 4 (8 bit) */
#define B53_VLAN_CTRL4			0x04
#define   VC4_ING_VID_CHECK_S		6
#define   VC4_ING_VID_CHECK_MASK	(0x3 << VC4_ING_VID_CHECK_S)
#define   VC4_ING_VID_VIO_FWD		0 /* forward, but do not learn */
#define   VC4_ING_VID_VIO_DROP		1 /* drop VID violations */
#define   VC4_NO_ING_VID_CHK		2 /* do not check */

/* VLAN Control 5 (8 bit) */
#define B53_VLAN_CTRL5			0x05
#define   VC5_DROP_VTABLE_MISS		BIT(3)

/* VLAN Table Access Register (16 bit) */
#define B53_VLAN_TABLE_ACCESS		0x06
#define   VTA_VID_LOW_MASK		0x00ff
#define   VTA_VID_HIGH_S		8
#define   VTA_VID_HIGH_MASK		(0xf << BCM53XX_VTA_VID_HIGH_S)
#define   VTA_RW_STATE			BIT(12)
#define   VTA_RW_STATE_RD		0
#define   VTA_RW_STATE_WR		BIT(12)
#define   VTA_RW_OP_EN			BIT(13)

/* VLAN Read/Write Registers (16 bit) */
#define B53_VLAN_WRITE			0x08
#define B53_VLAN_READ			0x0c
#define   VA_MEMBER_MASK		0x3f
#define   VA_UNTAG_S			6
#define   VA_UNTAG_MASK			(0x3f << BCM5325_VR_UNTAG_SHIFT)
#define   VA_VID_HIGH_S			12
#define   VA_VID_HIGH_MASK		(0xffff << BCM5325_VR_VID_HIGH_SHIFT)
#define   VA_VALID			BIT(20)

/* VLAN Port Default Tag (16 bit) */
#define B53_VLAN_PORT_DEF_TAG(i)	(0x10 + 2 * (i))

#endif /* !__B53_REGS_H */





