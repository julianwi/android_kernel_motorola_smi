DRIVER_NAME := pnwdisp

MEDIADIR=../../..
GFXDIR=$(MEDIADIR)/graphics/sgx
DISPLAYDIR=$(MEDIADIR)/display/pnw/drv
OTM_HDMI_SRC = $(MEDIADIR)/otm_hdmi
FBDEVDIR=$(MEDIADIR)/graphics/sgx/services4/3rdparty/linux_framebuffer_drm
VIDEO_COMMON_DIR=$(MEDIADIR)/video/common
DECODE_DIR=$(MEDIADIR)/video/decode
ENCODE_DIR=$(MEDIADIR)/video/encode
MEDIA_COMMON_DIR=$(MEDIADIR)/common

MEDIAINC=drivers/staging/intel_media
GFXINC=$(MEDIAINC)/graphics/sgx
DISPLAYINC=$(MEDIAINC)/display/pnw/drv
OTM_HDMI_INC=$(MEDIAINC)/otm_hdmi

# Get definitions for PVRINCS-y and PVRDEFS-y
include drivers/staging/intel_media/bld/clovertrail/pvrdefs.mk

include_dirs := \
	$(PVRINCS-y) \
	-I$(MEDIAINC) \
	-I$(DISPLAYINC) \
	-I$(MEDIAINC)/otm_hdmi/os/android/include \
	-I$(MEDIAINC)/otm_hdmi/pil/include \
	-I$(MEDIAINC)/video/decode \
	-I$(MEDIAINC)/video/encode \
	-I$(MEDIAINC)/video/common \
	-I$(MEDIAINC)/common \
	-I$(MEDIAINC)/interface \
	-I$(OTM_HDMI_INC)/os/android/include \
	-I$(OTM_HDMI_INC)/pil/include \
	-I$(OTM_HDMI_INC)/pil/common \
	-I$(OTM_HDMI_INC)/pil/specific/include \
	-I$(OTM_HDMI_INC)/pil/specific/ctp \
	-I$(OTM_HDMI_INC)/ipil/include \
	-I$(OTM_HDMI_INC)/ipil/common \
	-I$(OTM_HDMI_INC)/ipil/specific/include \
	-I$(OTM_HDMI_INC)/ipil/specific/mfld \
	-Iinclude/linux \
	-Iinclude/drm

ccflags-y += $(include_dirs)

ccflags-y += $(PVRDEFS-y)

$(DRIVER_NAME)-objs += $(FBDEVDIR)/drmlfb_displayclass.o \
	$(FBDEVDIR)/drmlfb_linux.o \
	$(MEDIA_COMMON_DIR)/psb_drv.o \
	$(MEDIA_COMMON_DIR)/psb_powermgmt.o \
	$(MEDIA_COMMON_DIR)/psb_irq.o \
	$(MEDIA_COMMON_DIR)/psb_gtt.o \
	$(DISPLAYDIR)/psb_bl.o \
	$(DISPLAYDIR)/psb_fb.o \
	$(DISPLAYDIR)/psb_intel_display.o \
	$(DISPLAYDIR)/mdfld_csc.o \
	$(DISPLAYDIR)/psb_pvr_glue.o \
	$(DISPLAYDIR)/mdfld_dsi_dbi.o \
	$(DISPLAYDIR)/mdfld_dsi_dpi.o \
	$(DISPLAYDIR)/mdfld_dsi_output.o \
	$(DISPLAYDIR)/mdfld_output.o \
	$(DISPLAYDIR)/auo_sc1_vid.o \
	$(DISPLAYDIR)/auo_sc1_cmd.o \
	$(DISPLAYDIR)/gi_sony_vid.o \
	$(DISPLAYDIR)/gi_sony_cmd.o \
	$(DISPLAYDIR)/h8c7_vid.o \
	$(DISPLAYDIR)/mdfld_dsi_pkg_sender.o \
	$(DISPLAYDIR)/mdfld_dsi_esd.o \
	$(DISPLAYDIR)/mdfld_dsi_dbi_dsr.o \
	$(DISPLAYDIR)/tc35876x_vid.o \
	$(DISPLAYDIR)/gi_renesas_cmd.o \
	$(DISPLAYDIR)/mdfld_hdmi_audio.o \

ifeq ($(CONFIG_MDFD_VIDEO_DECODE), y)
$(DRIVER_NAME)-objs += $(ENCODE_DIR)/pnw_topaz.o \
	$(ENCODE_DIR)/pnw_topazinit.o \
	$(VIDEO_COMMON_DIR)/psb_cmdbuf.o \
	$(VIDEO_COMMON_DIR)/psb_buffer.o \
	$(VIDEO_COMMON_DIR)/psb_fence.o \
	$(VIDEO_COMMON_DIR)/psb_mmu.o \
	$(DECODE_DIR)/psb_msvdx.o \
	$(DECODE_DIR)/psb_msvdx_ec.o \
	$(DECODE_DIR)/psb_msvdxinit.o \
	$(DECODE_DIR)/psb_msvdx_fw.o \
	$(VIDEO_COMMON_DIR)/psb_ttm_glue.o \
	$(VIDEO_COMMON_DIR)/psb_ttm_fence.o \
	$(VIDEO_COMMON_DIR)/psb_ttm_fence_user.o \
	$(VIDEO_COMMON_DIR)/psb_ttm_placement_user.o
endif

ifeq ($(CONFIG_SUPPORT_SMD_QHD_AMOLED_COMMAND_MODE_DISPLAY), y)
$(DRIVER_NAME)-objs += $(DISPLAYDIR)/smd_qhd_amoled_430_cmd.o
$(DRIVER_NAME)-objs += $(DISPLAYDIR)/smd_dynamic_gamma.o
$(DRIVER_NAME)-objs += $(DISPLAYDIR)/mdfld_mot_cmn.o
endif

obj-y += $(DRIVER_NAME).o
