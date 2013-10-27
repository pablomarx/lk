# top level project rules for the bcm2835-test project
#
LOCAL_DIR := $(GET_LOCAL_DIR)

TARGET := bcm2835
MODULES += \
	lib/evlog \
	lib/debugcommands \
	app/tests \
	app/shell

GLOBAL_DEFINES += \
	WITH_KERNEL_EVLOG=1

# extra rules to copy the bcm2835.conf file to the build dir
#$(BUILDDIR)/bcm2835.conf: $(LOCAL_DIR)/bcm2835.conf
#	@echo copy $< to $@
#	$(NOECHO)cp $< $@

#EXTRA_BUILDDEPS += $(BUILDDIR)/bcm2835.conf
#GENERATED += $(BUILDDIR)/bcm2835.conf
