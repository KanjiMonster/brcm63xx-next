
b53-y				+= common.o
b53-$(CONFIG_B53_FE)		+= fe_sw.o
b53-$(CONFIG_B53_GE)		+= ge_sw.o
b53-$(CONFIG_B53_PHY_DRIVER)	+= mdio.o
b53-$(CONFIG_B53_SPI_DRIVER)	+= spi.o

obj-$(CONFIG_B53)		+= b53.o

ccflags-y			+= -Werror
