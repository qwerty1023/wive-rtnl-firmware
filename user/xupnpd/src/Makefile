#LUA     = lua-5.1.x
LUA     = lua-5.2.x
STATIC  = false
CFLAGS  += -fno-exceptions -fno-rtti -I$(LUA) -L$(LUA) -I$(ROOTDIR)/lib/include
SRC     = main.cpp soap.cpp mem.cpp mcast.cpp luaxlib.cpp luaxcore.cpp luajson.cpp luajson_parser.cpp
LUAMYCFLAGS = -DLUA_USE_LINUX

CFLAGS  += -ffunction-sections -fdata-sections
CPPFLAGS += -ffunction-sections -fdata-sections
LUAMYCFLAGS += -ffunction-sections -fdata-sections
LDFLAGS += -Wl,--gc-sections

SDK_8_09 = /u01/home/shocker_old/staff/openwrt_8_09/staging_dir
SDK_BACKFIRE = /u01/home/shocker_old/staff/openwrt/staging_dir
SDK_BBREAKER = /home/shocker/staff/openwrt/staging_dir

ifeq ($(STATIC),true)
CFLAGS+=-static
LUAMYCFLAGS+=-static
endif

x86:
	make -C $(LUA) CC=gcc a
	gcc -Os -c -o md5.o md5c.c
	gcc $(CFLAGS) -DWITH_LIBUUID -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -o xupnpd $(SRC) md5.o -llua -ldl -lm -luuid
	strip xupnpd

# Cygwin
x86win:
	make -C $(LUA) CC=gcc a
	gcc -O2 -c -o md5.o md5c.c
	gcc $(CFLAGS) -DWITH_LIBUUID -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_UNDER_WIN -o xupnpd $(SRC) md5.o -llua -ldl -lm -luuid
	strip xupnpd

x86_bsd:
	make -C $(LUA) CC=gcc a
	gcc -O2 -c -o md5.o md5c.c
	gcc $(CFLAGS) -DINET -DWITH_LIBUUID -o xupnpd $(SRC) md5.o -llua -lm
	strip xupnpd


# D-Link DIR-320/DD-WRT (mipsel)
bcm947xx_ddwrt:
	$(MAKE) embedded TARGET=bcm947xx-ddwrt SDK=/u01/home/shocker/staff/openwrt_8_09/staging_dir/toolchain-mipsel_gcc3.4.6/mipsel-linux/bin PATH=$(PATH):/u01/home/shocker/staff/openwrt_8_09/staging_dir/toolchain-mipsel_gcc3.4.6/libexec/gcc/mipsel-linux-uclibc/3.4.6

# OpenWrt Backfire
bcm947xx:
	$(MAKE) embedded TARGET=bcm947xx SDK=$(STAGING_DIR)/toolchain-mipsel_gcc-3.4.6-bcm947xx/mipsel-openwrt-linux/bin PATH=$(PATH):$(STAGING_DIR)/toolchain-mipsel_gcc-3.4.6-bcm947xx/libexec/gcc/mipsel-openwrt-linux-uclibc/3.4.6 STAGING_DIR=$(STAGING_DIR)

ar71xx:
	$(MAKE) embedded TARGET=ar71xx SDK=$(STAGING_DIR)/toolchain-mips_r2_gcc-4.3.3+cs-ar71xx/mips-openwrt-linux/bin PATH=$(PATH):$(STAGING_DIR)/toolchain-mips_r2_gcc-4.3.3+cs-ar71xx/libexec/gcc/mips-openwrt-linux-uclibc/4.3.3 STAGING_DIR=$(STAGING_DIR)

ar231x:
	$(MAKE) embedded TARGET=ar231x SDK=$(STAGING_DIR)/toolchain-mips_gcc-4.3.3+cs-ar231x/mips-openwrt-linux/bin PATH=$(PATH):$(STAGING_DIR)/toolchain-mips_gcc-4.3.3+cs-ar231x/libexec/gcc/mips-openwrt-linux-uclibc/4.3.3 STAGING_DIR=$(STAGING_DIR)

# Other
samsung_d:
	$(MAKE) embedded TARGET=arm-v7a8 SDK=/opt/VDLinux-armv7a8/bin

samsung_c:
	$(MAKE) embedded TARGET=arm-v7_vfp_le SDK=/opt/arm_v7_vfp_le/bin

dreambox:
	$(MAKE) embedded TARGET=mipsel_dreambox SDK=/opt/mipsel-unknown-linux-gnu/bin


embedded:
	export PATH
	export STAGING_DIR
	make -C $(LUA) CC=$(CROSS_COMPILE)gcc a MYCFLAGS='$(LUAMYCFLAGS)'
	$(CROSS_COMPILE)gcc -Os -c -o md5.o md5c.c
	$(CROSS_COMPILE)gcc $(CFLAGS) -DWITH_URANDOM -o xupnpd $(SRC) md5.o -llua -lm -ldl

clean:
	make -C $(LUA) clean
	rm -f $(LUA)/liblua.a
	rm -f md5.o
	rm -f xupnpd
