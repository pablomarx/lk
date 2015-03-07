LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

ARCH := arm

ifeq ($(BCM_SOC),)
$(error need to define BCM_SOC)
endif

MEMBASE := 0x8000

ifeq ($(BCM_SOC),bcm2835)
ARM_CPU := arm1176jzf-s
MEMSIZE := 0x10000000	# 256MB
GLOBAL_DEFINES += \
	BCM2835=1
endif

ifeq ($(BCM_SOC),bcm2836)
ARM_CPU := cortex-a7
MEMSIZE := 0x40000000	# 1024MB
GLOBAL_DEFINES += \
	BCM2836=1
endif

GLOBAL_INCLUDES += \
	$(LOCAL_DIR)/include

MODULE_SRCS += \
	$(LOCAL_DIR)/platform_reset.S \
	$(LOCAL_DIR)/debug.c \
	$(LOCAL_DIR)/interrupts.c \
	$(LOCAL_DIR)/platform.c \
	$(LOCAL_DIR)/uart.c \
	$(LOCAL_DIR)/timer.c

GLOBAL_DEFINES += \
	WITH_PLATFORM_RESET=1 \
	MEMBASE=$(MEMBASE) \
	MEMSIZE=$(MEMSIZE)

LINKER_SCRIPT += \
	$(BUILDDIR)/system-onesegment.ld

include make/module.mk
