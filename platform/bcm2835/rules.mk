LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

ARCH := arm
ARM_CPU := arm1176jzf-s

GLOBAL_INCLUDES += \
	$(LOCAL_DIR)/include

MODULE_SRCS += \
	$(LOCAL_DIR)/platform_reset.S \
	$(LOCAL_DIR)/debug.c \
	$(LOCAL_DIR)/interrupts.c \
	$(LOCAL_DIR)/platform.c \
	$(LOCAL_DIR)/uart.c \
	$(LOCAL_DIR)/timer.c

MEMBASE := 0x8000
MEMSIZE := 0x10000000	# 256MB

GLOBAL_DEFINES += \
	WITH_PLATFORM_RESET=1 \
	MEMBASE=$(MEMBASE) \
	MEMSIZE=$(MEMSIZE)

LINKER_SCRIPT += \
	$(BUILDDIR)/system-onesegment.ld

include make/module.mk
