alias RUNQEMU_AUTOMOUNT="make -j8 run GDBSERVER=y  QEMU_VIRTFS_ENABLE=y QEMU_USERNET_ENABLE=y QEMU_VIRTFS_AUTOMOUNT=y"
alias BUILD_QEMU="make CFG_CORE_HEAP_SIZE=262144  GDBSERVER=y  QEMU_VIRTFS_ENABLE=y QEMU_USERNET_ENABLE=y QEMU_VIRTFS_AUTOMOUNT=y"
alias RUNQEMU="make -j8 run GDBSERVER=y  QEMU_VIRTFS_ENABLE=y QEMU_USERNET_ENABLE=y"


alias SWD="telnet localhost 54323"




build/common.mk
```
# The ports used for the consoles that are spawned when running QEMU.
QEMU_NW_PORT ?= 54322
QEMU_SW_PORT ?= 54323
```

build/qemu_v8.mk
```
QEMU_RUN_ARGS = $(QEMU_BASE_ARGS) $(QEMU_SCMI_ARGS)
QEMU_RUN_ARGS += $(QEMU_RUN_ARGS_COMMON)
#QEMU_RUN_ARGS += -s -S -serial tcp:127.0.0.1:$(QEMU_NW_PORT) -serial tcp:127.0.0.1:$(QEMU_SW_PORT) 
#QEMU_RUN_ARGS += -s -serial mon:stdio
#QEMU_RUN_ARGS += -s -serial tcp:127.0.0.1:64320,server
QEMU_RUN_ARGS += -s -serial mon:stdio -serial tcp:127.0.0.1:$(QEMU_SW_PORT),server 
.PHONY: run-only
run-only:
	ln -sf $(ROOT)/out-br/images/rootfs.cpio.gz $(BINARIES_PATH)/
	$(call check-terminal)
	$(call run-help)
#	$(call launch-terminal,$(QEMU_NW_PORT),"Normal World")
#	$(call launch-terminal,$(QEMU_SW_PORT),"Secure World")
#	$(call wait-for-ports,$(QEMU_NW_PORT),$(QEMU_SW_PORT))
	cd $(BINARIES_PATH) && $(QEMU_BUILD)/aarch64-softmmu/qemu-system-aarch64 \
		$(QEMU_RUN_ARGS)
```
