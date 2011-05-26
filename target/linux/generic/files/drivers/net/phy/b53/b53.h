/*
 * Broadcom Home Gateway Reference Design
 * BCM53xx Register definitions
 *
 * Copyright 2004, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 */

#ifndef __BCM535M_H_
#define __BCM535M_H_

/* ROBO embedded device type */
#define ROBO_DEV_5380 1
#define ROBO_DEV_5365 2
#define ROBO_DEV_5350 3

/* Robo device ID register (in ROBO_MGMT_PAGE) */
#define B53_DEVICE_ID		0x30
#define  BCM53XX_DEVICE_ID_5325		0x25 /* Faked */
#define  BCM53XX_DEVICE_ID_5395		0x95
#define  BCM53XX_DEVICE_ID_5397		0x97
#define  BCM53XX_DEVICE_ID_5398		0x98
#define  BCM53XX_DEVICE_ID_53115	0x3115

/* Register Access Pseudo PHY */
#define BCM53XX_PSEUDO_PHY	0x1e

/* MII registers */
#define REG_MII_PAGE    0x10    /* MII Page register */
#define REG_MII_ADDR    0x11    /* MII Address register */
#define REG_MII_DATA0   0x18    /* MII Data register 0 */

#define REG_MII_PAGE_ENABLE     1
#define REG_MII_ADDR_WRITE      1
#define REG_MII_ADDR_READ       2

/* BCM5325m Serial Management Port (SMP) Page offsets */
#define B53_CTRL_PAGE			0x00  /* Control registers */
#define B53_STAT_PAGE			0x01  /* Status register */
#define B53_MGMT_PAGE			0x02  /* Management Mode registers */
#define B53_MIB_AC_PAGE			0x03  /* MIB Autocast registers */
#define B53_ARLCTRL_PAGE		0x04  /* ARL Control Registers */
#define B53_ARLIO_PAGE			0x05  /* ARL Access Registers */
#define B53_FRAMEBUF_PAGE		0x06  /* Management frame access registers */
#define B53_MEM_ACCESS_PAGE		0x08  /* Memory access registers */

#define BCM53XX_NUM_PORTS	6
#define BCM5380_NUM_PORTS	9

/* PHY Registers */
#define BCM53XX_PORT_MII_PAGE(i)	(0x10 + i) /* Port i MII Registers */
#define BCM53XX_IM_PORT_PAGE		0x18 /* Inverse MII Port (to EMAC) */
#define BCM53XX_ALL_PORT_PAGE		0x19 /* All ports MII Registers (broadcast)*/

/* MAC Statistics registers */
#define BCM53XX_PORT_MIB_PAGE(i)	(0x20 + i) /* Port i 10/100 MIB Statistics */
#define BCM53XX_IM_PORT_MIB_PAGE	0x28 /* Inverse MII Port MIB Statistics */

/* Quality of Service (QoS) Registers */
#define BCM53XX_QOS_PAGE		0x30 /* QoS Registers */

/* VLAN Registers */
#define B53_VLAN_PAGE		0x34 /* VLAN Registers */

/* Note SPI Data/IO Registers not used */
#define BCM53XX_SPI_DATA_IO_0_PAGE	0xf0 /* SPI Data I/O 0 */
#define BCM53XX_SPI_DATA_IO_1_PAGE	0xf1 /* SPI Data I/O 1 */
#define BCM53XX_SPI_DATA_IO_2_PAGE	0xf2 /* SPI Data I/O 2 */
#define BCM53XX_SPI_DATA_IO_3_PAGE	0xf3 /* SPI Data I/O 3 */
#define BCM53XX_SPI_DATA_IO_4_PAGE	0xf4 /* SPI Data I/O 4 */
#define BCM53XX_SPI_DATA_IO_5_PAGE	0xf5 /* SPI Data I/O 5 */
#define BCM53XX_SPI_DATA_IO_6_PAGE	0xf6 /* SPI Data I/O 6 */
#define BCM53XX_SPI_DATA_IO_7_PAGE	0xf7 /* SPI Data I/O 7 */

#define ROBO_SPI_STATUS_PAGE		0xfe /* SPI Status Registers */
#define ROBO_PAGE_PAGE			0xff /* Page Registers */

#define BCM53XX_PORT_CTRL(i)		(0x00 + i)	/* 10/100 Port 0 Control */
#define BCM53XX_IM_PORT_CTRL		0x08		/* 10/100 MII Port Control */
#define   PORT_CTRL_RX_DISABLE		BIT(0)
#define   PORT_CTRL_TX_DISABLE		BIT(1)
#define   PORT_CTRL_RX_BCST_EN		BIT(2)		/* Enable Broadcast RX (MII Port only) */
#define   PORT_CTRL_RX_MCST_EN		BIT(3)		/* Enable Multicast RX (MII Port only) */
#define   PORT_CTRL_RX_UCST_EN		BIT(4)		/* Enable Unicast RX (MII Port only) */
#define	  PORT_CTRL_STP_STATE_S		5
#define   PORT_CTRL_STP_STATE_MASK	(0x3 << PORT_CTRL_STP_STATE_S)


#define BCM53XX_SMP_CTRL		0x0a /* SMP Control register */
#define B53_SWITCH_MODE			0x0b	/* Switch Mode Control */
#define   B53_SM_SW_FWD_MODE	BIT(0)	/* Managed Mode Enable */
#define   B53_SM_SW_FWD_EN		BIT(1)	/* Forwarding Enable */
#define BCM53XX_PORT_OVERRIDE_CTRL	0x0e	/* Port state override */
#define BCM53XX_PORT_OVERRIDE_RVMII	BIT(4)	/* Bit 4 enables RvMII */
#define BCM53XX_PD_MODE_CTRL		0x0f /* Power-down mode control */
#define BCM53XX_IP_MULTICAST_CTRL	0x21 /* IP Multicast control */

/* BCM5325m STATUS PAGE (0x01) REGISTER MAP : 16bit/48bit registers */
#define ROBO_HALF_DUPLEX 0
#define ROBO_FULL_DUPLEX 1

#define B53_LINK_STAT_SUMMARY    0x00 /* Link Status Summary: 16bit */
#define ROBO_LINK_STAT_CHANGE     0x02 /* Link Status Change: 16bit */
#define B53_SPEED_STAT_SUMMARY   0x04 /* Port Speed Summary: 16bit*/
#define B53_DUPLEX_STAT_SUMMARY  0x06 /* Duplex Status Summary: 16bit */
#define ROBO_PAUSE_STAT_SUMMARY   0x08 /* PAUSE Status Summary: 16bit */
#define ROBO_SOURCE_ADDR_CHANGE   0x0C /* Source Address Change: 16bit	*/
#define ROBO_LSA_PORT0            0x10 /* Last Source Addr, Port 0: 48bits*/
#define ROBO_LSA_PORT1            0x16 /* Last Source Addr, Port 1: 48bits*/
#define ROBO_LSA_PORT2            0x1c /* Last Source Addr, Port 2: 48bits*/
#define ROBO_LSA_PORT3            0x22 /* Last Source Addr, Port 3: 48bits*/
#define ROBO_LSA_PORT4            0x28 /* Last Source Addr, Port 4: 48bits*/
#define ROBO_LSA_IM_PORT          0x40 /* Last Source Addr, IM Port: 48bits*/


#define BCM53XX_GLOBAL_CONFIG		0x00 /* Global Management Config: 8bit*/
#define   BCM53XX_GC_RESET_MIB		0x01
#define   BCM53XX_GC_RX_BPDU_EN		0x02
#define   BCM53XX_GC_MIB_AC_HDR_EN	0x10
#define   BCM53XX_GC_MIB_AC_EN		0x20
#define   BCM53XX_GC_FRM_MGMT_PORT_M	0xC0
#define   BCM53XX_GC_FRM_MGMT_PORT_04	0x00
#define   BCM53XX_GC_FRM_MGMT_PORT_MII	0x80

#define BCM53XX_MGMT_PORT_ID		0x02 /* Management Port ID: 8bit*/
#define   BCM53XX_MGMT_PORT_ID_M	0x0C

#define BCM53XX_RMON_MIB_STEER       0x04 /* RMON Mib Steering: 16bit */
#define BCM53XX_MIB_MODE_SELECT      0x04 /* MIB Mode select: 16bit (BCM5350) */
#define BCM53XX_AGE_TIMER_CTRL       0x06 /* Age time control: 32bit */
#define BCM53XX_MIRROR_CAP_CTRL      0x10 /* Mirror Capture : 16bit */
#define BCM53XX_MIRROR_ING_CTRL      0x12 /* Mirror Ingress Control: 16bit */
#define BCM53XX_MIRROR_ING_DIV_CTRL  0x14 /* Mirror Ingress Divider: 16bit */
#define BCM53XX_MIRROR_ING_MAC_ADDR  0x16 /* Ingress Mirror MAC Addr: 48bit*/
#define BCM53XX_MIRROR_EGR_CTRL      0x1c /* Mirror Egress Control: 16bit */
#define ROBO_MIRROR_EGR_DIV_CTRL  0x1e /* Mirror Egress Divider: 16bit */
#define ROBO_MIRROR_EGR_MAC_ADDR  0x20 /* Egress Mirror MAC Addr: 48bit*/

/* BCM5325m MIB AUTOCAST REGISTERS (0x03) REGISTER MAP: 8/16/48 bit regs */
#define ROBO_MIB_AC_PORT          0x00 /* MIB Autocast Port: 16bit */
#define ROBO_MIB_AC_HDR_PTR       0x02 /* MIB Autocast Header pointer:16bit*/ 
#define ROBO_MIB_AC_HDR_LEN       0x04 /* MIB Autocast Header Len: 16bit */
#define ROBO_MIB_AC_DA            0x06 /* MIB Autocast DA: 48bit */
#define ROBO_MIB_AC_SA            0x0c /* MIB Autocast SA: 48bit */
#define ROBO_MIB_AC_TYPE          0x12 /* MIB Autocast Type: 16bit */
#define ROBO_MIB_AC_RATE          0x14 /* MIB Autocast Rate: 8bit */
#define ROBO_GET_AC_RATE(secs) ((secs)*10)
#define ROBO_AC_RATE_MAX          0xff
#define ROBO_AC_RATE_DEFAULT      0x64  /* 10 secs */
typedef struct _ROBO_MIB_AC_STRUCT
{
    unsigned char   opcode:4;       /* Tx MIB Autocast opcode */
    unsigned char   portno:4;       /* zero-based port no. */
    unsigned char   portstate:8;    /* port state */
    unsigned long long TxOctets;   
    unsigned int    TxDropPkts;
    unsigned int    rsvd;
    unsigned int    TxBroadcastPkts;
    unsigned int    TxMulticastPkts;
    unsigned int    TxUnicastPkts;
    unsigned int    TxCollisions;
    unsigned int    TxSingleCollision;
    unsigned int    TxMultiCollision;
    unsigned int    TxDeferredTransmit;
    unsigned int    TxLateCollision;
    unsigned int    TxExcessiveCollision;
    unsigned int    TxFrameInDiscards;
    unsigned int    TxPausePkts;
    unsigned int    rsvd1[2];
    unsigned long long RxOctets;
    unsigned int    RxUndersizePkts;
    unsigned int    RxPausePkts;
    unsigned int    RxPkts64Octets;
    unsigned int    RxPkts64to127Octets;
    unsigned int    RxPkts128to255Octets;
    unsigned int    RxPkts256to511Octets;
    unsigned int    RxPkts512to1023Octets;
    unsigned int    RxPkts1024to1522Octets;
    unsigned int    RxOversizePkts;
    unsigned int    RxJabbers;
    unsigned int    RxAlignmentErrors;
    unsigned int    RxFCSErrors;
    unsigned long long RxGoodOctets;
    unsigned int    RxDropPkts;
    unsigned int    RxUnicastPkts;
    unsigned int    RxMulticastPkts;
    unsigned int    RxBroadcastPkts;
    unsigned int    RxSAChanges;
    unsigned int    RxFragments;
    unsigned int    RxExcessSizeDisc;
    unsigned int    RxSymbolError;
} ROBO_MIB_AC_STRUCT;

/* BCM5325m ARL CONTROL REGISTERS (0x04) REGISTER MAP: 8/16/48/64 bit regs */
#define ROBO_ARL_CONFIG           0x00 /* ARL Global Configuration: 8bit*/
#define ROBO_BPDU_MC_ADDR_REG     0x04 /* BPDU Multicast Address Reg:64bit*/
#define ROBO_MULTIPORT_ADDR_1     0x10 /* Multiport Address 1: 48 bits*/
#define ROBO_MULTIPORT_VECTOR_1   0x16 /* Multiport Vector 1: 16 bits */
#define ROBO_MULTIPORT_ADDR_2     0x20 /* Multiport Address 2: 48 bits*/
#define ROBO_MULTIPORT_VECTOR_2   0x26 /* Multiport Vector 2: 16 bits */
#define ROBO_SECURE_SRC_PORT_MASK 0x30 /* Secure Source Port Mask: 16 bits*/
#define ROBO_SECURE_DST_PORT_MASK 0x32 /* Secure Dest Port Mask: 16 bits */


/* BCM5325m ARL IO REGISTERS (0x05) REGISTER MAP: 8/16/48/64 bit regs */
#define ARL_TABLE_WRITE 0              /* for read/write state in control reg */
#define ARL_TABLE_READ  1              /* for read/write state in control reg */
#ifdef BCM5380
#define ARL_VID_BYTES   2              /* number of bytes for VID */
#else
#define ARL_VID_BYTES   1              /* number of bytes for VID */
#endif
typedef struct _ROBO_ARL_RW_CTRL_STRUC
{
    unsigned char   ARLrw:1;    /* ARL read/write (1=read) */
    unsigned char   rsvd:6;     /* reserved */
    unsigned char   ARLStart:1; /* ARL start/done (1=start) */
} ROBO_ARL_RW_CTRL_STRUC;
typedef struct _ROBO_ARL_SEARCH_CTRL_STRUC
{
    unsigned char   valid:1;    /* ARL search result valid */
    unsigned char   rsvd:6;     /* reserved */
    unsigned char   ARLStart:1; /* ARL start/done (1=start) */
} ROBO_ARL_SEARCH_CTRL_STRUC;
typedef struct _ROBO_ARL_ENTRY_CTRL_STRUC
{
    unsigned char   portID:4;   /* port id */
    unsigned char   chipID:2;   /* chip id */
    unsigned char   rsvd:5;     /* reserved */
    unsigned char   prio:2;     /* priority */
    unsigned char   age:1;      /* age */
    unsigned char   staticEn:1; /* static */
    unsigned char   valid:1;    /* valid */
} ROBO_ARL_ENTRY_CTRL_STRUC;
typedef struct _ROBO_ARL_SEARCH_RESULT_CTRL_STRUC
{
    unsigned char   portID:4;   /* port id */
    unsigned char   rsvd:1;     /* reserved */
    unsigned char   vid:8;   	/* vlan id */
    unsigned char   age:1;      /* age */
    unsigned char   staticEn:1; /* static */
    unsigned char   valid:1;    /* valid */
} ROBO_ARL_SEARCH_RESULT_CTRL_STRUC;
typedef struct _ROBO_ARL_ENTRY_MAC_STRUC
{
    unsigned char   macBytes[6];    /* MAC address */
} ROBO_ARL_ENTRY_MAC_STRUC;

typedef struct _ROBO_ARL_ENTRY_STRUC
{
    ROBO_ARL_ENTRY_MAC_STRUC    mac;    /* MAC address */
    ROBO_ARL_ENTRY_CTRL_STRUC   ctrl;   /* control bits */
} ROBO_ARL_ENTRY_STRUC;

typedef struct _ROBO_ARL_SEARCH_RESULT_STRUC
{
    ROBO_ARL_ENTRY_MAC_STRUC    mac;    		/* MAC address */
    ROBO_ARL_SEARCH_RESULT_CTRL_STRUC   ctrl;   /* control bits */
} ROBO_ARL_SEARCH_RESULT_STRUC;

/* multicast versions of ARL entry structs */
typedef struct _ROBO_ARL_ENTRY_MCAST_CTRL_STRUC
{
    unsigned int    portMask:12;/* multicast port mask */
    unsigned char   prio:1;     /* priority */
    unsigned char   gigPort:1;  /* gigabit port 1 mask */
    unsigned char   staticEn:1; /* static */
    unsigned char   valid:1;    /* valid */
} ROBO_ARL_ENTRY_MCAST_CTRL_STRUC;
typedef struct _ROBO_ARL_SEARCH_RESULT_MCAST_CTRL_STRUC
{
    unsigned int    portMask:13;   	/* multicast port mask */
    unsigned char   age:1;      	/* age */
    unsigned char   staticEn:1; 	/* static */
    unsigned char   valid:1;    	/* valid */
} ROBO_ARL_SEARCH_RESULT_MCAST_CTRL_STRUC;
/* BCM5350 extension register */
typedef struct _ROBO_ARL_SEARCH_RESULT_EXTENSION
{
    unsigned int    prio:2;         /* priority */
    unsigned int    portMask:1;     /* MSB (MII) of port mask for multicast */
    unsigned int    reserved:5;
} ROBO_ARL_SEARCH_RESULT_EXTENSION;

typedef struct _ROBO_ARL_ENTRY_MCAST_STRUC
{
    ROBO_ARL_ENTRY_MAC_STRUC        mac;    /* MAC address */
    ROBO_ARL_ENTRY_MCAST_CTRL_STRUC ctrl;   /* control bits */
} ROBO_ARL_ENTRY_MCAST_STRUC;
typedef struct _ROBO_ARL_SEARCH_RESULT_MCAST_STRUC
{
    ROBO_ARL_ENTRY_MAC_STRUC    mac;    				/* MAC address */
    ROBO_ARL_SEARCH_RESULT_MCAST_CTRL_STRUC   ctrl;   	/* control bits */
} ROBO_ARL_SEARCH_RESULT_MCAST_STRUC;

#define ROBO_ARL_RW_CTRL          0x00 /* ARL Read/Write Control :  8bit */
#define ROBO_ARL_MAC_ADDR_IDX     0x02 /* MAC Address Index: 48bit */
#define ROBO_ARL_VID_TABLE_IDX    0x08 /* VID Table Address Index: 8bit */
#define ROBO_ARL_ENTRY0           0x10 /* ARL Entry 0 : 64 bit */
#define ROBO_ARL_ENTRY1           0x18 /* ARL Entry 1 : 64 bit */
#define ROBO_ARL_SEARCH_CTRL      0x20 /* ARL Search Control: 8bit */
#define ROBO_ARL_SEARCH_ADDR      0x22 /* ARL Search Address: 16bit */
#define ROBO_ARL_SEARCH_RESULT    0x24 /* ARL Search Result: 64bit */
#define ROBO_ARL_SEARCH_RESULT_EXT 0x2c /* ARL Search Result Extension (5350): 8bit */
#define ROBO_ARL_VID_ENTRY0       0x30 /* ARL VID Entry 0: 64bit */
#define ROBO_ARL_VID_ENTRY1       0x32 /* ARL VID Entry 1: 64bit */

/* BCM5325m MANAGEMENT FRAME REGISTERS (0x6) REGISTER MAP: 8/16 bit regs */
#define ROBO_MGMT_FRAME_RD_DATA   0x00 /* Management Frame Read Data :8bit*/
#define ROBO_MGMT_FRAME_WR_DATA   0x01 /* Management Frame Write Data:8bit*/
#define ROBO_MGMT_FRAME_WR_CTRL   0x02 /* Write Control: 16bit */
#define ROBO_MGMT_FRAME_RD_STAT   0x04 /* Read Status: 16bit */

/* BCM5325m MEMORY ACCESS REGISTERS (Page 0x08) REGISTER MAP: 32 bit regs */
#define MEM_TABLE_READ  1               /* for read/write state in mem access reg */
#define MEM_TABLE_WRITE 0               /* for read/write state in mem access reg */
#define MEM_TABLE_ACCESS_START 1        /* for mem access read/write start */
#define MEM_TABLE_ACCESS_DONE  0        /* for mem access read/write done */
#define VLAN_TABLE_ADDR 0x3800          /* BCM5380 only */
#ifdef BCM5380
#define NUM_ARL_TABLE_ENTRIES 4096      /* number of entries in ARL table */
#define NUM_VLAN_TABLE_ENTRIES 2048     /* number of entries in VLAN table */
#define ARL_TABLE_ADDR 0                /* offset of ARL table start */
#else
#define NUM_ARL_TABLE_ENTRIES 2048      /* number of entries in ARL table */
#define NUM_VLAN_TABLE_ENTRIES 256      /* number of entries in VLAN table */
#define ARL_TABLE_ADDR 0x3800           /* offset of ARL table start */
/* corresponding values for 5350 */
#define NUM_ARL_TABLE_ENTRIES_5350 1024 /* number of entries in ARL table (5350) */
#define NUM_VLAN_TABLE_ENTRIES_5350 16  /* number of entries in VLAN table */
#define ARL_TABLE_ADDR_5350 0x1c00      /* offset of ARL table start (5350) */
#endif
typedef struct _ROBO_MEM_ACCESS_CTRL_STRUC
{
    unsigned int    memAddr:14; /* 64-bit memory address */
    unsigned char   rsvd:4;     /* reserved */
    unsigned char   readEn:1;   /* read enable (0 == write) */
    unsigned char   startDone:1;/* memory access start/done */
    unsigned int    rsvd1:12;   /* reserved */
} ROBO_MEM_ACCESS_CTRL_STRUC;
typedef struct _ROBO_MEM_ACCESS_DATA_STRUC
{
    unsigned int    memData[2]; /* 64-bit data */
    unsigned short  rsvd;       /* reserved */
} ROBO_MEM_ACCESS_DATA_STRUC;

#ifdef BCM5380
typedef struct _ROBO_ARL_TABLE_DATA_STRUC
{
    unsigned char   MACaddr[6]; /* MAC addr */
    unsigned int    portID:4;   /* port ID */
    unsigned int    chipID:2;   /* chip ID */
    unsigned int    rsvd:6;     /* reserved */
    unsigned int    highPrio:1; /* high priority address */
    unsigned int    age:1;      /* entry accessed/learned since ageing process */
    unsigned int    staticAddr:1;/* entry is static */
    unsigned int    valid:1;    /* entry is valid */
    unsigned int    vid:12;     /* vlan id */
    unsigned int    rsvd2:4;    /* reserved */
} ROBO_ARL_TABLE_DATA_STRUC;
#else
typedef struct _ROBO_ARL_TABLE_DATA_STRUC
{
    unsigned char   MACaddr[6]; /* MAC addr */
    unsigned int    portID:4;   /* port ID */
    unsigned int    chipID:2;   /* chip ID */
    unsigned int    rsvd:7;     /* reserved */
    unsigned int    age:1;      /* entry accessed/learned since ageing process */
    unsigned int    staticAddr:1;/* entry is static */
    unsigned int    valid:1;    /* entry is valid */
} ROBO_ARL_TABLE_DATA_STRUC;
#endif

/* multicast format*/
typedef struct _ROBO_ARL_TABLE_MCAST_DATA_STRUC
{
    unsigned char   MACaddr[6]; /* MAC addr */
    unsigned int    portMask:12;/* multicast port mask */
    unsigned char   prio:1;     /* priority */
    unsigned char   gigPort:1;  /* gigabit port 1 mask */
    unsigned char   staticEn:1; /* static */
    unsigned char   valid:1;    /* valid */
    unsigned int    vid:12;     /* vlan id */
    unsigned int    rsvd2:4;    /* reserved */
} ROBO_ARL_TABLE_MCAST_DATA_STRUC;
#define ROBO_MEM_ACCESS_CTRL      0x00 /* Memory Read/Write Control :32bit*/
#define ROBO_MEM_ACCESS_DATA      0x04 /* Memory Read/Write Data:64bit*/

/* BCM5325m SWITCH PORT (0x10-18) REGISTER MAP: 8/16 bit regs */
typedef struct _ROBO_MII_CTRL_STRUC
{
    unsigned char   rsvd:8;     /* reserved */
    unsigned char   duplex:1;   /* duplex mode */
    unsigned char   restartAN:1;/* restart auto-negotiation */
    unsigned char   rsvd1:1;    /* reserved */
    unsigned char   powerDown:1;/* power down */
    unsigned char   ANenable:1; /* auto-negotiation enable */
    unsigned char   speed:1;    /* forced speed selection */
    unsigned char   loopback:1; /* loopback */
    unsigned char   reset:1;    /* reset */
} ROBO_MII_CTRL_STRUC;
typedef struct _ROBO_MII_AN_ADVERT_STRUC
{
    unsigned char   selector:5;     /* advertise selector field */
    unsigned char   T10BaseT:1;     /* advertise 10BaseT */
    unsigned char   T10BaseTFull:1; /* advertise 10BaseT, full duplex */
    unsigned char   T100BaseX:1;    /* advertise 100BaseX */
    unsigned char   T100BaseXFull:1;/* advertise 100BaseX full duplex */
    unsigned char   noT4:1;         /* do not advertise T4 */
    unsigned char   pause:1;        /* advertise pause for full duplex */
    unsigned char   rsvd:2;         /* reserved */
    unsigned char   remoteFault:1;  /* transmit remote fault */
    unsigned char   rsvd1:1;        /* reserved */
    unsigned char   nextPage:1;     /* nex page operation supported */
} ROBO_MII_AN_ADVERT_STRUC;
#define ROBO_MII_CTRL                 0x00 /* Port MII Control */
#define ROBO_MII_STAT                 0x02 /* Port MII Status  */
/* Fields of link status register */
#define ROBO_MII_STAT_JABBER          (1<<1) /* Jabber detected */
#define ROBO_MII_STAT_LINK            (1<<2) /* Link status */

#define ROBO_MII_PHYID_HI             0x04 /* Port PHY ID High */
#define ROBO_MII_PHYID_LO             0x06 /* Port PHY ID Low */
#define ROBO_MII_ANA_REG              0x08 /* MII Auto-Neg Advertisement */
#define ROBO_MII_ANP_REG              0x0a /* MII Auto-Neg Partner Ability */
#define ROBO_MII_AN_EXP_REG           0x0c /* MII Auto-Neg Expansion */
#define ROBO_MII_AN_NP_REG            0x0e /* MII next page */
#define ROBO_MII_ANP_NP_REG           0x10 /* MII Partner next page */
#define ROBO_MII_100BX_AUX_CTRL       0x20 /* 100BASE-X Auxiliary Control */
#define ROBO_MII_100BX_AUX_STAT       0x22 /* 100BASE-X Auxiliary Status  */
#define ROBO_MII_100BX_RCV_ERR_CTR    0x24 /* 100BASE-X Receive Error Ctr */
#define ROBO_MII_100BX_RCV_FS_ERR     0x26 /* 100BASE-X Rcv False Sense Ctr */
#define ROBO_MII_AUX_CTRL             0x30 /* Auxiliary Control/Status */
/* Fields of Auxiliary control register */
#define ROBO_MII_AUX_CTRL_FD         (1<<0) /* Full duplex link detected*/
#define ROBO_MII_AUX_CTRL_SP100      (1<<1) /* Speed 100 indication */
#define ROBO_MII_AUX_STATUS           0x32 /* Aux Status Summary */
#define ROBO_MII_CONN_STATUS          0x34 /* Aux Connection Status */
#define ROBO_MII_AUX_MODE2            0x36 /* Aux Mode 2 */
#define ROBO_MII_AUX_ERR_STATUS       0x38 /* Aux Error and General Status */
#define ROBO_MII_AUX_MULTI_PHY        0x3c /* Aux Multiple PHY Register*/
#define ROBO_MII_BROADCOM_TEST        0x3e /* Broadcom Test Register */


/* BCM5325m PORT MIB REGISTERS (Pages 0x20-0x24,0x28) REGISTER MAP: 64/32 */
/* Tranmit Statistics */
#define ROBO_MIB_TX_OCTETS            0x00 /* 64b: TxOctets */
#define ROBO_MIB_TX_DROP_PKTS         0x08 /* 32b: TxDropPkts */
#define ROBO_MIB_TX_BC_PKTS           0x10 /* 32b: TxBroadcastPkts */
#define ROBO_MIB_TX_MC_PKTS           0x14 /* 32b: TxMulticastPkts */
#define ROBO_MIB_TX_UC_PKTS           0x18 /* 32b: TxUnicastPkts */
#define ROBO_MIB_TX_COLLISIONS        0x1c /* 32b: TxCollisions */
#define ROBO_MIB_TX_SINGLE_COLLISIONS 0x20 /* 32b: TxSingleCollision */
#define ROBO_MIB_TX_MULTI_COLLISIONS  0x24 /* 32b: TxMultiCollision */
#define ROBO_MIB_TX_DEFER_TX          0x28 /* 32b: TxDeferred Transmit */
#define ROBO_MIB_TX_LATE_COLLISIONS   0x2c /* 32b: TxLateCollision */
#define ROBO_MIB_EXCESS_COLLISIONS    0x30 /* 32b: TxExcessiveCollision*/
#define ROBO_MIB_FRAME_IN_DISCARDS    0x34 /* 32b: TxFrameInDiscards */
#define ROBO_MIB_TX_PAUSE_PKTS        0x38 /* 32b: TxPausePkts */

/* Receive Statistics */
#define ROBO_MIB_RX_OCTETS            0x44 /* 64b: RxOctets */
#define ROBO_MIB_RX_UNDER_SIZE_PKTS   0x4c /* 32b: RxUndersizePkts(runts)*/
#define ROBO_MIB_RX_PAUSE_PKTS        0x50 /* 32b: RxPausePkts */
#define ROBO_MIB_RX_PKTS_64           0x54 /* 32b: RxPkts64Octets */
#define ROBO_MIB_RX_PKTS_65_TO_127    0x58 /* 32b: RxPkts64to127Octets*/
#define ROBO_MIB_RX_PKTS_128_TO_255   0x5c /* 32b: RxPkts128to255Octets*/
#define ROBO_MIB_RX_PKTS_256_TO_511   0x60 /* 32b: RxPkts256to511Octets*/
#define ROBO_MIB_RX_PKTS_512_TO_1023  0x64 /* 32b: RxPkts512to1023Octets*/
#define ROBO_MIB_RX_PKTS_1024_TO_1522 0x68 /* 32b: RxPkts1024to1522Octets*/
#define ROBO_MIB_RX_OVER_SIZE_PKTS    0x6c /* 32b: RxOversizePkts*/
#define ROBO_MIB_RX_JABBERS           0x70 /* 32b: RxJabbers*/
#define ROBO_MIB_RX_ALIGNMENT_ERRORS  0x74 /* 32b: RxAlignmentErrors*/
#define ROBO_MIB_RX_FCS_ERRORS        0x78 /* 32b: RxFCSErrors */
#define ROBO_MIB_RX_GOOD_OCTETS       0x7c /* 32b: RxGoodOctets */
#define ROBO_MIB_RX_DROP_PKTS         0x84 /* 32b: RxDropPkts */
#define ROBO_MIB_RX_UC_PKTS           0x88 /* 32b: RxUnicastPkts */
#define ROBO_MIB_RX_MC_PKTS           0x8c /* 32b: RxMulticastPkts */
#define ROBO_MIB_RX_BC_PKTS           0x90 /* 32b: RxBroadcastPkts */
#define ROBO_MIB_RX_SA_CHANGES        0x94 /* 32b: RxSAChanges */
#define ROBO_MIB_RX_FRAGMENTS         0x98 /* 32b: RxFragments */
#define ROBO_MIB_RX_EXCESS_SZ_DISC    0x9c /* 32b: RxExcessSizeDisc*/
#define ROBO_MIB_RX_SYMBOL_ERROR      0xa0 /* 32b: RxSymbolError */

/* BCM5350 MIB Statistics */
/* Group 0 */
#define ROBO_MIB_TX_GOOD_PKTS         0x00 /* 16b: TxGoodPkts */
#define ROBO_MIB_TX_UNICAST_PKTS      0x02 /* 16b: TxUnicastPkts */
#define ROBO_MIB_RX_GOOD_PKTS         0x04 /* 16b: RxGoodPkts */
#define ROBO_MIB_RX_GOOD_UNICAST_PKTS 0x06 /* 16b: RxGoodUnicastPkts */
/* Group 1 */
#define ROBO_MIB_TX_COLLISION         0x00 /* 16b: TxCollision */
#define ROBO_MIB_TX_OCTETS_5350       0x02 /* 16b: TxOctets */
#define ROBO_MIB_RX_FCS_ERRORS_5350   0x04 /* 16b: RxFCSErrors */
#define ROBO_MIB_RX_GOOD_OCTETS_5350  0x06 /* 16b: RxGoodOctets */

/* BCM5325m QoS REGISTERS (Page 0x30) REGISTER MAP: 8/16 */
#define ROBO_QOS_CTRL                 0x00 /* 16b: QoS Control Register */
#define ROBO_QOS_LOCAL_WEIGHT_CTRL    0x10 /* 8b: Local HQ/LQ Weight Register*/
#define ROBO_QOS_CPU_WEIGHT_CTRL      0x12 /* 8b: CPU HQ/LQ Weight Register*/
#define ROBO_QOS_PAUSE_ENA            0x13 /* 16b: Qos Pause Enable Register*/
#define ROBO_QOS_PRIO_THRESHOLD       0x15 /* 8b: Priority Threshold Register*/
#define ROBO_QOS_RESERVED             0x16 /* 8b: Qos Reserved Register */

/* BCM5325m VLAN REGISTERS (Page 0x34) REGISTER MAP: 8/16bit */
typedef struct _ROBO_VLAN_CTRL0_STRUC
{
    unsigned char   frameControlP:2;    /* 802.1P frame control */
    unsigned char   frameControlQ:2;    /* 802.1Q frame control */
    unsigned char   dropMissedVID:1;    /* enable drop missed VID packet */
    unsigned char   vidMacHash:1;       /* VID_MAC hash enable */
    unsigned char   vidMacCheck:1;      /* VID_MAC check enable */
    unsigned char   VLANen:1;           /* 802.1Q VLAN enable */
} ROBO_VLAN_CTRL0_STRUC;
#define VLAN_TABLE_WRITE 1              /* for read/write state in table access reg */
#define VLAN_TABLE_READ 0               /* for read/write state in table access reg */
#define VLAN_ID_HIGH_BITS 0             /* static high bits in table access reg */
#define VLAN_ID_MAX 255                 /* max VLAN id */
#define VLAN_ID_MAX5350 15              /* max VLAN id (5325) */
#define VLAN_ID_MASK VLAN_ID_MAX        /* VLAN id mask */

#define B53_VLAN_CTRL0				0x00	/* 8b: VLAN Control 0 Register */
#define   B53_VC0_DROP_VID_MISS		BIT(4)
#define   B53_VC0_VID_HASH_VID		BIT(5)
#define   B53_VC0_VID_CHK_EN		BIT(6)	/* Use VID,DA or VID,SA  */
#define   B53_VC0_VLAN_EN			BIT(7)	/* 802.1Q VLAN Enabled */
#define B53_VLAN_CTRL1				0x01 /* 8b: VLAN Control 1 Register */
#define   B53_VC1_RX_MCST_TAG_EN	BIT(1)
#define   B53_VC1_RX_MCST_FWD_EN	BIT(2)
#define   B53_VC1_RX_MCST_UNTAG_EN	BIT(3)
#define B53_VLAN_CTRL2				0x02 /* 8b: VLAN Control 2 Register */
#define B53_VLAN_CTRL3				0x03 /* 8b: VLAN Control 3 Register */
#define B53_VLAN_CTRL4				0x04 /* 8b: VLAN Control 4 Register */
#define  B53_VC4_ING_VID_CHECK_S	6
#define  B53_VC4_ING_VID_CHECK_MASK	(0x3 << B53_VC4_ING_VID_CHECK_S)
#define  B53_VC4_ING_VID_VIO_FWD	0	/* forward, but do not learn */
#define  B53_VC4_ING_VID_VIO_DROP	1	/* drop VID violations */
#define  B53_VC4_NO_ING_VID_CHK		2	/* do not check */
#define B53_VLAN_CTRL5				0x05 /* 8b: VLAN Control 5 Register */
#define  B53_VC5_DROP_VTABLE_MISS	BIT(3)
#define BCM53XX_VLAN_TABLE_ACCESS	0x08 /* 14b: VLAN Table Access Register */
#define BCM5325_VLAN_TABLE_ACCESS	0x06 /* 14b: VLAN Table Access Register (5325) */
#define   BCM53XX_VTA_VID_LOW_MASK	0x00ff
#define   BCM53XX_VTA_VID_HIGH_S	8
#define   BCM53XX_VTA_VID_HIGH_MASK	(0xf << BCM53XX_VTA_VID_HIGH_S)
#define   BCM53XX_VTA_RW_STATE		0x1000
#define   BCM53XX_VTA_RW_STATE_RD	0x0000
#define   BCM53XX_VTA_RW_STATE_WR	0x1000
#define   BCM53XX_VTA_RW_OP_EN		0x2000
#define BCM53XX_VLAN_WRITE		0x0a /* 15b: VLAN Write Register */
#define BCM5325_VLAN_WRITE		0x08 /* 15b: VLAN Write Register (5325) */
#define BCM53XX_VLAN_READ		0x0c /* 15b: VLAN Read Register */
#define   BCM53XX_VA_MEMBER_MASK	0x7f
#define   BCM53xx_VA_UNTAG_SHIFT	7
#define   BCM53XX_VA_UNTAG_MASK		(0x7f << BCM5325_VR_UNTAG_SHIFT)
#define   BCM53XX_VA_VALID		BIT(14)
#define   BCM5325_VA_MEMBER_MASK	0x3f
#define   BCM5325_VA_UNTAG_S		6
#define   BCM5325_VA_UNTAG_MASK		(0x3f << BCM5325_VR_UNTAG_SHIFT)
#define   BCM5325_VA_VID_HIGH_S		12
#define   BCM5325_VA_VID_HIGH_MASK	(0xffff << BCM5325_VR_VID_HIGH_SHIFT)
#define   BCM5325_VA_VALID			BIT(20)
#define   BCM5380_VA_MEMBER_MASK	0x1fff
#define   BCM5380_VA_UNTAG_SHIFT	13
#define   BCM5380_VA_UNTAG_MASK		(0x1fff << BCM5380_VA_UNTAG_SHIFT)
#define   BCM5380_VA_VALID		BIT(26)

#define BCM53XX_VLAN_PORT_DEF_TAG(i)	(0x10 + 2 *(i)) /* 16b: VLAN Port Default Tag Register */

#define BCM53XX_VLAN_PORTMII_DEF_TAG   0x1a /* 16b: VLAN Port MII Default Tag Register */
/* 5380 only */
#define ROBO_VLAN_PORT5_DEF_TAG     0x1a /* 16b: VLAN Port 5 Default Tag Register */
#define ROBO_VLAN_PORT6_DEF_TAG     0x1c /* 16b: VLAN Port 6 Default Tag Register */
#define ROBO_VLAN_PORT7_DEF_TAG     0x1e /* 16b: VLAN Port 7 Default Tag Register */

/* obsolete */
#define ROBO_VLAN_PORT0_CTRL       0x00 /* 16b: Port 0 VLAN  Register */
#define ROBO_VLAN_PORT1_CTRL       0x02 /* 16b: Port 1 VLAN  Register */
#define ROBO_VLAN_PORT2_CTRL       0x04 /* 16b: Port 2 VLAN  Register */
#define ROBO_VLAN_PORT3_CTRL       0x06 /* 16b: Port 3 VLAN  Register */
#define ROBO_VLAN_PORT4_CTRL       0x08 /* 16b: Port 4 VLAN  Register */
#define ROBO_VLAN_IM_PORT_CTRL     0x10 /* 16b: Inverse MII Port VLAN Reg */
#define ROBO_VLAN_SMP_PORT_CTRL    0x12 /* 16b: Serial Port VLAN  Register */
#define ROBO_VLAN_PORTSPI_DEF_TAG  0x1c /* 16b: VLAN Port SPI Default Tag Register */
#define ROBO_VLAN_PRIORITY_REMAP   0x20 /* 24b: VLAN Priority Re-Map Register */

#endif /* !__BCM535M_H_ */





