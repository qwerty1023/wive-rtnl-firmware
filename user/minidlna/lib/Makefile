.EXPORT_ALL_VARIABLES:
.PHONY: all clean romfs

ifndef ROOTDIR
ROOTDIR=`pwd`
endif
ifndef ROMFSDIR
ROMFSDIR=$(ROOTDIR)/romfs
endif

CFLAGS  += -ffunction-sections -fdata-sections
LDFLAGS += -Wl,--gc-sections

DESTDIR=/opt/Wive-RTNL/user/minidlna/stage

dir_y =
dir_n =
dir_  =
CONFIG_USER_MINIDLNA=y
# minidlna shared libs
dir_$(CONFIG_USER_MINIDLNA)		+= libexif
dir_$(CONFIG_USER_MINIDLNA)		+= libjpeg
dir_$(CONFIG_USER_MINIDLNA)		+= libogg
dir_$(CONFIG_USER_MINIDLNA)		+= libvorbis
dir_$(CONFIG_USER_MINIDLNA)		+= libflac
dir_$(CONFIG_USER_MINIDLNA)		+= libav
dir_$(CONFIG_USER_MINIDLNA)		+= libid3tag
dir_$(CONFIG_USER_MINIDLNA)		+= libsqlite

all: 
	CC=$(CONFIG_CROSS_COMPILER_PATH)/mipsel-linux-uclibc-gcc ; \
	GCC=$(CONFIG_CROSS_COMPILER_PATH)/mipsel-linux-uclibc-gcc ; \
	CXX=$(CONFIG_CROSS_COMPILER_PATH)/mipsel-linux-uclibc-g++ ; \
	AR=$(CONFIG_CROSS_COMPILER_PATH)/mipsel-linux-uclibc-ar ; \
	AS=$(CONFIG_CROSS_COMPILER_PATH)/mipsel-linux-uclibc-as ; \
	LD=$(CONFIG_CROSS_COMPILER_PATH)/mipsel-linux-uclibc-ld ; \
	NM=$(CONFIG_CROSS_COMPILER_PATH)/mipsel-linux-uclibc-nm ; \
	RANLIB=$(CONFIG_CROSS_COMPILER_PATH)/mipsel-linux-uclibc-ranlib ; \
	STRIP=$(CONFIG_CROSS_COMPILER_PATH)/mipsel-linux-uclibc-strip ;

	for i in $(dir_y) ; do \
		[ -d $$i ] && \
		$(MAKE) -j$(HOST_NCPU) -C $$i || \
		exit $$? ; \
		if [ ! -f $(DESTDIR)/.installed_$$i ] ; then \
			$(MAKE) -C $$i install && \
			touch $(DESTDIR)/.installed_$$i ; \
		fi ; \
	done

romfs:
	if [ $(CONFIG_USER_MINIDLNA) != "y" ] ; then \
		cp -fP $(DESTDIR)/lib/libvorbis.so* $(ROMFSDIR)/lib ; \
	fi

%_only:
	$(MAKE) -C $(@:_only=)

%_clean:
	$(MAKE) -C $(@:_clean=) clean

clean:
	for i in $(dir_y) $(dir_n) $(dir_) ; do \
		if [ -d $$i ] ; then \
			$(MAKE) -C $$i clean ; \
			rm -f $(DESTDIR)/.installed_$$i ; \
		fi ; \
	done

