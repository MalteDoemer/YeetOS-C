
all: YeetOS

.EXPORT_ALL_VARIABLES: .PHONY

ARCH=x86

TOPDIR	:= $(abspath .)
MAKE=make --no-print-directory

ARCHIVES=\
kernel/kernel.a \
libc/libc.a \
drivers/drivers.a \
arch/$(ARCH)/arch.a \

LD_SCRIPT = arch/$(ARCH)/link.ld

SUBDIRS= kernel libc drivers arch/$(ARCH)

# should define $(CC) $(LD) $(AS) $(AR) 
# and respectively $(C_FLAGS) $(LD_FLAGS) $(AS_FLAGS)
include arch/$(ARCH)/make.config


DEFINES := __KERNEL__ $(DEFINES)

ifdef DEBUG
DEFINES := _DEBUG __DEBUG $(DEFINES)
else
DEFINES := NDEBUG $(DEFINES)
endif

DEFINES := $(patsubst %,-D%,$(DEFINES))

C_FLAGS := $(C_FLAGS) $(DEFINES)


YeetOS: subdirs
	$(LD) -T $(LD_SCRIPT) $(LD_FLAGS) --whole-archive $(ARCHIVES) -o $@

subdirs: symlinks
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done

clean:
	rm -f include/arch
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done

symlinks:
	rm -f include/arch
	( cd include ; ln -s arch-$(ARCH) arch )

format:
	scripts/format.sh

install: YeetOS
	scripts/install.sh

run: install
	scripts/run.sh