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

DESTDIR=/home/sandra/source/wive-rtnl-firmware/user/minidlna/stage

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
	for i in $(dir_y) ; do \
		[ -d $$i ] && \
		$(MAKE) -C $$i || \
		exit $$? ; \
		if [ ! -f $(DESTDIR)/.installed_$$i ] ; then \
			$(MAKE) -C $$i install && \
			touch $(DESTDIR)/.installed_$$i ; \
		fi ; \
	done

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

