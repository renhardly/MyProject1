PROGS=db onoff modesetter testpat planescale capture producer consumer
OMAP_PROGS=omap-db
PKG_CONFIG=pkg-config
CROSS_COMPILE:=arm-linux-gnueabihf-
CC:=$(CROSS_COMPILE)gcc
LD:=$(CROSS_COMPILE)ld
CFLAGS:=--sysroot=/media/rootfs
ifdef CROSS_COMPILE
	CFLAGS += $(shell $(PKG_CONFIG) --cflags libdrm) $(shell $(PKG_CONFIG) --cflags libdrm_omap)
	LDLIBS += $(shell $(PKG_CONFIG) --libs libdrm) $(shell $(PKG_CONFIG) --libs libdrm_omap)
	PROGS += $(OMAP_PROGS)
else
	CFLAGS += $(shell $(PKG_CONFIG) --cflags libdrm)
	LDLIBS += $(shell $(PKG_CONFIG) --libs libdrm)
endif
CFLAGS:=$(subst -I,-I/media/rootfs,$(CFLAGS))

CFLAGS += -O2 -Wall -std=c99 -D_GNU_SOURCE -D_DEFAULT_SOURCE -D_XOPEN_SOURCE

LDLIBS += -lrt -pthread
#LDFLAGS += -static

all:$(PROGS)

$(PROGS): % : %.c common.o common-drm.o common-modeset.o common-drawing.o
	@echo "$(LINK.c) $^ $(LDLIBS) -o $@"
	@$(LINK.c) $^ $(LDLIBS) -o $@

%.o: %.c *.h
	@echo "$(COMPILE.c) -o $@ $<"
	@$(COMPILE.c) -o $@ $<

.PHONY: strip clean

strip: $(PROGS)
	$(STRIP) $(PROGS)

clean:
	rm -f $(PROGS) *.o
