#
# Copyright 2019 Kopera
#

obj-y += ../common/variscite/imx8m_eeprom.o

ifdef CONFIG_SPL_BUILD
obj-y += spl.o
obj-$(CONFIG_IMX8M_LPDDR4) += lpddr4_timing_b0.o lpddr4_timing.o
obj-$(CONFIG_POWER_PFUZE100) += ../../freescale/common/pfuze.o
else
obj-y += imx8mq_kopera_polar0.o ../common/mmc.o
endif
