# $Id: Makefile.linux,v 1.94 2016/04/18 09:06:42 nanard Exp $
# MiniUPnP project
# (c) 2006-2015 Thomas Bernard
# http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
# Author : Thomas Bernard
# for use with GNU Make
#
# options can be passed to genconfig.sh through CONFIG_OPTIONS :
# $ CONFIG_OPTIONS="--ipv6 --igd2" make -f Makefile.linux
#
# To install use :
# $ DESTDIR=/dummyinstalldir make -f Makefile.linux install
# or :
# $ INSTALLPREFIX=/usr/local make -f Makefile.linux install
# or :
# $ make -f Makefile.linux install
# (default INSTALLPREFIX is /usr)
#
# if your system hasn't iptables libiptc headers and binary correctly
# installed, you need to get iptables sources from http://netfilter.org/
# ./configure them and build them then miniupnpd will build using :
# $ IPTABLESPATH=/path/to/iptables-1.4.1 make -f Makefile.linux
#
#CFLAGS = -Wall -O -D_GNU_SOURCE -g -DDEBUG

IPTABLESPATH=../iptables
CFLAGS += -Wall -Os -D_GNU_SOURCE -I../iptables -I../iptables/include -I../iptables/libiptc
LDFLAGS += $(ROOTDIR)/lib/lib

CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
LDFLAGS += -Wl,--gc-sections

CC ?= gcc
RM = rm -f
INSTALL = install
STRIP ?= strip
CP = cp

PREFIX=./filesystem

INSTALLPREFIX ?= $(PREFIX)/usr
SBININSTALLDIR = $(INSTALLPREFIX)/sbin
ETCINSTALLDIR = $(PREFIX)/etc/miniupnpd
MANINSTALLDIR = $(INSTALLPREFIX)/share/man/man8

BASEOBJS = miniupnpd.o upnphttp.o upnpdescgen.o upnpsoap.o \
           upnpreplyparse.o minixml.o portinuse.o \
           upnpredirect.o getifaddr.o daemonize.o upnpglobalvars.o \
           options.o upnppermissions.o minissdp.o natpmp.o pcpserver.o \
           upnpevents.o upnputils.o getconnstatus.o \
           upnppinhole.o pcplearndscp.o asyncsendto.o

LNXOBJS = linux/getifstats.o linux/ifacewatcher.o
NETFILTEROBJS = netfilter/iptcrdr.o netfilter/iptpinhole.o netfilter/nfct_get.o

ALLOBJS = $(BASEOBJS) $(LNXOBJS) $(NETFILTEROBJS)

PCFILE_FOUND :=

ifeq (${PCFILE_FOUND},0)

IPTABLESVERSION := $(shell pkg-config --modversion libiptc)
IPTVER1 := $(shell echo $(IPTABLESVERSION) | cut -d. -f1 )
IPTVER2 := $(shell echo $(IPTABLESVERSION) | cut -d. -f2 )
IPTVER3 := $(shell echo $(IPTABLESVERSION) | cut -d. -f3 )
# test if iptables version >= 1.4.3
TEST := $(shell [ $(IPTVER1) -gt 1 ] || \
	[ \( $(IPTVER1) -eq 1 \) -a \
	  \( \( $(IPTVER2) -gt 4 \) -o \
	     \( \( $(IPTVER2) -eq 4 \) -a \( $(IPTVER3) -ge 3 \) \) \) ] && echo 1 )
ifeq ($(TEST), 1)
CFLAGS += -DIPTABLES_143
endif

LDFLAGS += $(ROOTDIR)/lib/lib

else

ifeq "$(wildcard /etc/gentoo-release )" ""
LIBS ?= -liptc
else # gentoo
# the following is better, at least on gentoo with iptables 1.4.6
# see http://miniupnp.tuxfamily.org/forum/viewtopic.php?p=1618
# and http://miniupnp.tuxfamily.org/forum/viewtopic.php?p=2183
LIBS ?= -lip4tc
CFLAGS := -DIPTABLES_143 $(CFLAGS)
endif

ARCH ?= $(shell uname -m | grep -q "x86_64" && echo 64)
ifdef IPTABLESPATH
CFLAGS := $(CFLAGS) -I$(IPTABLESPATH)/include/
LDFLAGS := $(LDFLAFGS) -L$(IPTABLESPATH)/libiptc/ -L$(ROOTDIR)/lib/lib
# get iptables version and set IPTABLES_143 macro if needed
ifeq ($(TARGET_OPENWRT),)
IPTABLESVERSION := $(shell grep "\#define VERSION" $(IPTABLESPATH)/config.h | tr -d \" |cut -d" " -f3 )
IPTVER1 := $(shell echo $(IPTABLESVERSION) | cut -d. -f1 )
IPTVER2 := $(shell echo $(IPTABLESVERSION) | cut -d. -f2 )
IPTVER3 := $(shell echo $(IPTABLESVERSION) | cut -d. -f3 )
# test if iptables version >= 1.4.3
TEST := $(shell [ $(IPTVER1) -gt 1 ] || \
	[ \( $(IPTVER1) -eq 1 \) -a \
	  \( \( $(IPTVER2) -gt 4 \) -o \
	     \( \( $(IPTVER2) -eq 4 \) -a \( $(IPTVER3) -ge 3 \) \) \) ] && echo 1 )
ifeq ($(TEST), 1)
CFLAGS := $(CFLAGS) -DIPTABLES_143
# the following sucks, but works
LIBS = $(IPTABLESPATH)/libiptc/libip4tc.o
#LIBS = $(IPTABLESPATH)/libiptc/.libs/libiptc.a
else # ifeq ($(TEST), 1)
LIBS = $(IPTABLESPATH)/libiptc/libiptc.a
endif # ifeq ($(TEST), 1)
else # ($(TARGET_OPENWRT),)
# openWRT :
# check for system-wide iptables files. Test if iptables version >= 1.4.3
# the following test has to be verified :
TEST := $(shell test -f /usr/include/iptables/internal.h && grep -q "\#define IPTABLES_VERSION" /usr/include/iptables/internal.h && echo 1)
ifeq ($(TEST), 1)
CFLAGS := $(CFLAGS) -DIPTABLES_143
LIBS = -liptc
endif  # ($(TEST), 1)
TEST_LIB := $(shell test -f /usr/lib$(ARCH)/libiptc.a && echo 1)
ifeq ($(TEST_LIB), 1)
LIBS = -liptc /usr/lib$(ARCH)/libiptc.a
endif # ($(TEST_LIB), 1)
endif # ($(TARGET_OPENWRT),)
else # ifdef IPTABLESPATH
# IPTABLESPATH not defined
# the following test has to be verified :
TEST := $(shell test -f /usr/include/xtables.h && grep -q "XTABLES_VERSION_CODE" /usr/include/xtables.h && echo 1)
ifeq ($(TEST), 1)
CFLAGS := $(CFLAGS) -DIPTABLES_143
LIBS = -liptc
TESTIP4TC := $(shell test -f /lib/libip4tc.so && echo 1)
ifeq ($(TESTIP4TC), 1)
LIBS := $(LIBS) -lip4tc
endif # ($(TESTIP4TC), 1)
TESTIP6TC := $(shell test -f /lib/libip6tc.so && echo 1)
ifeq ($(TESTIP6TC), 1)
LIBS := $(LIBS) -lip6tc
endif # ($(TESTIP6TC), 1)
endif # ($(TEST), 1)
endif # ifdef IPTABLESPATH
endif # ifdef PCFILE_FOUND

LIBS += $(IPTABLESPATH)/libiptc/libiptc.a

TEST := $(shell pkg-config --atleast-version=1.0.2 libnetfilter_conntrack && pkg-config --atleast-version=1.0.3 libmnl && echo 1)
ifeq ($(TEST),1)
CFLAGS += -DUSE_NFCT
LIBS += $(shell pkg-config --static --libs-only-l libmnl)
LIBS += $(shell pkg-config --static --libs-only-l libnetfilter_conntrack)
endif # ($(TEST),1)

#LIBS += $(shell pkg-config --static --libs-only-l libssl)

#TEST := $(shell $(PKG_CONFIG) --exists uuid && echo 1)
#ifeq ($(TEST),1)
#LDLIBS += $(shell $(PKG_CONFIG) --static --libs-only-l uuid)
#else
#$(info please install uuid-dev package / libuuid)
#endif # ($(TEST),1)

TESTUPNPDESCGENOBJS = testupnpdescgen.o upnpdescgen.o

EXECUTABLES = miniupnpd testupnpdescgen testgetifstats \
              testupnppermissions miniupnpdctl testgetifaddr \
              testasyncsendto testportinuse \
              testssdppktgen

.PHONY:	all clean install depend genuuid

all:	$(EXECUTABLES)

clean:
	$(RM) $(ALLOBJS)
	$(RM) $(EXECUTABLES)
	$(RM) testupnpdescgen.o testgetifstats.o
	$(RM) testupnppermissions.o testgetifaddr.o
	$(RM) testasyncsendto.o
	$(RM) testportinuse.o
	$(RM) miniupnpdctl.o
	$(RM) validateupnppermissions validategetifaddr validatessdppktgen
	$(RM) -rf filesystem

install:	miniupnpd miniupnpd.8 miniupnpd.conf genuuid \
	netfilter/iptables_init.sh netfilter/iptables_removeall.sh \
	netfilter/ip6tables_init.sh netfilter/ip6tables_removeall.sh \
	linux/miniupnpd.init.d.script
	$(STRIP) miniupnpd
	$(INSTALL) -d $(DESTDIR)$(SBININSTALLDIR)
	$(INSTALL) miniupnpd $(DESTDIR)$(SBININSTALLDIR)
	$(INSTALL) -d $(DESTDIR)$(ETCINSTALLDIR)
	$(INSTALL) netfilter/iptables_init.sh $(DESTDIR)$(ETCINSTALLDIR)
	$(INSTALL) netfilter/iptables_removeall.sh $(DESTDIR)$(ETCINSTALLDIR)
	$(INSTALL) netfilter/ip6tables_init.sh $(DESTDIR)$(ETCINSTALLDIR)
	$(INSTALL) netfilter/ip6tables_removeall.sh $(DESTDIR)$(ETCINSTALLDIR)
	$(INSTALL) --mode=0644 -b miniupnpd.conf $(DESTDIR)$(ETCINSTALLDIR)
	$(INSTALL) -d $(DESTDIR)$(PREFIX)/etc/init.d
	$(INSTALL) linux/miniupnpd.init.d.script $(DESTDIR)$(PREFIX)/etc/init.d/miniupnpd
	$(INSTALL) -d $(DESTDIR)$(MANINSTALLDIR)
#	$(INSTALL) --mode=0644 miniupnpd.8 $(DESTDIR)$(MANINSTALLDIR)
#	gzip -f $(DESTDIR)$(MANINSTALLDIR)/miniupnpd.8

romfs: install
	$(ROMFSINST) -S filesystem/usr/sbin/miniupnpd /bin/miniupnpd
	$(ROMFSINST) miniupnpd.conf /etc/miniupnpd.conf

# genuuid is using the uuidgen CLI tool which is part of libuuid
# from the e2fsprogs
# 'cat /proc/sys/kernel/random/uuid' could be also used
genuuid:
ifeq ($(TARGET_OPENWRT),)
	sed -i -e "s/^uuid=[-0-9a-f]*/uuid=`(genuuid||uuidgen||uuid) 2>/dev/null`/" miniupnpd.conf
else
	sed -i -e "s/^uuid=[-0-9a-f]*/uuid=`($(STAGING_DIR_HOST)/bin/genuuid||$(STAGING_DIR_HOST)/bin/uuidgen||$(STAGING_DIR_HOST)/bin/uuid) 2>/dev/null`/" miniupnpd.conf
endif

check:	validateupnppermissions validategetifaddr validatessdppktgen

validateupnppermissions:	testupnppermissions testupnppermissions.sh
	./testupnppermissions.sh
	touch $@

validategetifaddr:	testgetifaddr testgetifaddr.sh
	./testgetifaddr.sh
	touch $@

validatessdppktgen:	testssdppktgen
	./testssdppktgen
	touch $@

miniupnpd:	$(BASEOBJS) $(LNXOBJS) $(NETFILTEROBJS) $(LIBS)

testupnpdescgen:	$(TESTUPNPDESCGENOBJS)

testgetifstats:	testgetifstats.o linux/getifstats.o

testupnppermissions:	testupnppermissions.o upnppermissions.o

testssdppktgen:	testssdppktgen.o

testgetifaddr:	testgetifaddr.o getifaddr.o

testasyncsendto:	testasyncsendto.o asyncsendto.o upnputils.o $(LIBS)

testportinuse:	testportinuse.o portinuse.o getifaddr.o \
	netfilter/iptcrdr.o $(LIBS)

miniupnpdctl:	miniupnpdctl.o

config.h:	genconfig.sh VERSION
	./genconfig.sh $(CONFIG_OPTIONS)

depend:	config.h
	makedepend -f$(MAKEFILE_LIST) -Y \
	$(ALLOBJS:.o=.c) $(TESTUPNPDESCGENOBJS:.o=.c) \
	testgetifstats.c testupnppermissions.c testgetifaddr.c \
	testasyncsendto.c testportinuse.c \
	miniupnpdctl.c testssdppktgen.c 2>/dev/null

# DO NOT DELETE

miniupnpd.o: config.h macros.h upnpglobalvars.h upnppermissions.h
miniupnpd.o: miniupnpdtypes.h upnphttp.h upnpdescgen.h miniupnpdpath.h
miniupnpd.o: getifaddr.h upnpsoap.h options.h minissdp.h upnpredirect.h
miniupnpd.o: upnppinhole.h daemonize.h upnpevents.h asyncsendto.h natpmp.h
miniupnpd.o: pcpserver.h commonrdr.h upnputils.h ifacewatcher.h
upnphttp.o: config.h upnphttp.h upnpdescgen.h miniupnpdpath.h upnpsoap.h
upnphttp.o: upnpevents.h upnputils.h
upnpdescgen.o: config.h getifaddr.h upnpredirect.h upnpdescgen.h
upnpdescgen.o: miniupnpdpath.h upnpglobalvars.h upnppermissions.h
upnpdescgen.o: miniupnpdtypes.h upnpdescstrings.h upnpurns.h getconnstatus.h
upnpsoap.o: macros.h config.h upnpglobalvars.h upnppermissions.h
upnpsoap.o: miniupnpdtypes.h upnphttp.h upnpsoap.h upnpreplyparse.h
upnpsoap.o: upnpredirect.h upnppinhole.h getifaddr.h getifstats.h
upnpsoap.o: getconnstatus.h upnpurns.h
upnpreplyparse.o: upnpreplyparse.h minixml.h
minixml.o: minixml.h
portinuse.o: macros.h config.h upnpglobalvars.h upnppermissions.h
portinuse.o: miniupnpdtypes.h getifaddr.h portinuse.h netfilter/iptcrdr.h
portinuse.o: commonrdr.h
upnpredirect.o: macros.h config.h upnpredirect.h upnpglobalvars.h
upnpredirect.o: upnppermissions.h miniupnpdtypes.h upnpevents.h portinuse.h
upnpredirect.o: netfilter/iptcrdr.h commonrdr.h
getifaddr.o: config.h getifaddr.h
daemonize.o: daemonize.h config.h
upnpglobalvars.o: config.h upnpglobalvars.h upnppermissions.h
upnpglobalvars.o: miniupnpdtypes.h upnpdescstrings.h
options.o: config.h options.h upnppermissions.h upnpglobalvars.h
options.o: miniupnpdtypes.h
upnppermissions.o: config.h upnppermissions.h
minissdp.o: config.h upnpdescstrings.h miniupnpdpath.h upnphttp.h
minissdp.o: upnpglobalvars.h upnppermissions.h miniupnpdtypes.h minissdp.h
minissdp.o: upnputils.h getroute.h asyncsendto.h codelength.h
natpmp.o: macros.h config.h natpmp.h upnpglobalvars.h upnppermissions.h
natpmp.o: miniupnpdtypes.h getifaddr.h upnpredirect.h commonrdr.h upnputils.h
natpmp.o: portinuse.h asyncsendto.h
pcpserver.o: config.h pcpserver.h macros.h upnpglobalvars.h upnppermissions.h
pcpserver.o: miniupnpdtypes.h pcplearndscp.h upnpredirect.h commonrdr.h
pcpserver.o: getifaddr.h asyncsendto.h pcp_msg_struct.h netfilter/iptcrdr.h
pcpserver.o: commonrdr.h
upnpevents.o: config.h upnpevents.h miniupnpdpath.h upnpglobalvars.h
upnpevents.o: upnppermissions.h miniupnpdtypes.h upnpdescgen.h upnputils.h
upnputils.o: config.h upnputils.h upnpglobalvars.h upnppermissions.h
upnputils.o: miniupnpdtypes.h getroute.h
getconnstatus.o: getconnstatus.h getifaddr.h
upnppinhole.o: macros.h config.h upnpredirect.h upnpglobalvars.h
upnppinhole.o: upnppermissions.h miniupnpdtypes.h upnpevents.h
upnppinhole.o: netfilter/iptpinhole.h
pcplearndscp.o: config.h upnpglobalvars.h upnppermissions.h miniupnpdtypes.h
pcplearndscp.o: pcplearndscp.h
asyncsendto.o: asyncsendto.h
linux/getifstats.o: config.h getifstats.h
linux/ifacewatcher.o: config.h ifacewatcher.h config.h minissdp.h
linux/ifacewatcher.o: miniupnpdtypes.h getifaddr.h upnpglobalvars.h
linux/ifacewatcher.o: upnppermissions.h natpmp.h
netfilter/iptcrdr.o: macros.h config.h netfilter/iptcrdr.h commonrdr.h
netfilter/iptcrdr.o: config.h upnpglobalvars.h upnppermissions.h
netfilter/iptcrdr.o: miniupnpdtypes.h
netfilter/iptpinhole.o: config.h netfilter/iptpinhole.h upnpglobalvars.h
netfilter/iptpinhole.o: upnppermissions.h config.h miniupnpdtypes.h
testupnpdescgen.o: macros.h config.h upnpdescgen.h upnpdescstrings.h
testupnpdescgen.o: getifaddr.h
upnpdescgen.o: config.h getifaddr.h upnpredirect.h upnpdescgen.h
upnpdescgen.o: miniupnpdpath.h upnpglobalvars.h upnppermissions.h
upnpdescgen.o: miniupnpdtypes.h upnpdescstrings.h upnpurns.h getconnstatus.h
testgetifstats.o: getifstats.h
testupnppermissions.o: upnppermissions.h config.h
testgetifaddr.o: config.h getifaddr.h
testasyncsendto.o: miniupnpdtypes.h config.h upnputils.h asyncsendto.h
testportinuse.o: macros.h config.h portinuse.h
miniupnpdctl.o: macros.h
testssdppktgen.o: macros.h config.h miniupnpdpath.h upnphttp.h
