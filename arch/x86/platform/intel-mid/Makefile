CFLAGS_intel_soc_pm_debug.o		:= -Werror
CFLAGS_intel_soc_pmu.o			:= -Werror
CFLAGS_intel_soc_mdfld.o		:= -Werror

# platform configuration for board devices
obj-y					+= device_libs/

# INTEL-MID specific files
obj-$(CONFIG_X86_INTEL_MID)		+= intel_mid_vrtc.o
obj-$(CONFIG_X86_INTEL_MID)		+= intel-mid.o
# MFLD specific files
obj-$(CONFIG_SERIAL_MFD_HSU)		+= mfld-hsu.o
obj-$(CONFIG_X86_MDFLD)			+= mfld.o
obj-$(CONFIG_INTEL_MID_RAM_CONSOLE)     += intel_mid_ram_console.o

obj-$(CONFIG_ATOM_SOC_POWER)		+= intel_soc_pm_debug.o intel_soc_pmu.o
obj-$(CONFIG_INTEL_ATOM_MDFLD_POWER)	+= intel_soc_mdfld.o

# BOARD files
obj-$(CONFIG_BOARD_MFLD_MOTO)		+= board-mmi.o
