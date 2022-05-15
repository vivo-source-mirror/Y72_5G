ifeq ($(CONFIG_VIVO_TS),)
  CONFIG_VIVO_TS=y
endif

obj-$(CONFIG_VIVO_TS) += vivo_ts.o

vivo_ts-y += vts_core.o
vivo_ts-y += vts_node.o
vivo_ts-y += vts_incell.o
vivo_ts-y += vts_class.o
vivo_ts-y += vts_report.o
vivo_ts-y += vts_op.o
vivo_ts-y += vts_spi_drv.o




vivo_ts-y += $(addprefix nt36670/, \
				nt36xxx.o \
				nt36xxx_ext_proc.o \
				nt36xxx_fw_update.o \
				nt36xxx_mp_ctrlram.o \
			)

vivo_ts-y += $(addprefix nt-no-flash/, \
				nt36xxx.o \
				nt36xxx_ext_proc.o \
				nt36xxx_fw_update.o \
				nt36xxx_mp_ctrlram.o \
			)



vivo_ts-y += $(addprefix ilitek/, \
				ilitek_core.o \
				ilitek_fw.o \
				ilitek_ic.o \
				ilitek_main.o \
				ilitek_mp.o \
				ilitek_node.o \
				ilitek_plat.o \
				ilitek_spi.o \
				ilitek_touch.o \
			)

vivo_ts-y += $(addprefix ili_9882n/, \
				ili9881x.o \
				ili9881x_hostdl.o \
				ili9881x_ic.o \
				ili9881x_mp.o \
				ili9881x_node.o \
				ili9881x_plat.o \
				ili9881x_spi.o \
				ili9881x_touch.o \
				vivo_v2.o \
			)

vivo_ts-y += vts_init.o
