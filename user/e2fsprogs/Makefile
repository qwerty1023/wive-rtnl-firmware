SRC_NAME=e2fsprogs-1.42.12

all:	config_test
	$(MAKE) -C $(SRC_NAME)

config_test:
	( if [ ! -f ./config_done ]; then \
		make configure && touch config_done; \
	fi )

configure:
	( cd $(SRC_NAME) ; \
	PKG_CONFIG_PATH=$FIRMROOT/lib/lib/ \
	./configure \
		--prefix= \
		--disable-compression \
		--disable-debugfs \
		--disable-testio-debug \
		--disable-imager \
		--disable-resizer \
		--disable-e2initrd-helper \
		--disable-uuidd \
		--disable-nls \
		--disable-tls \
		--enable-symlink-build \
		--enable-verbose-makecmds \
		--enable-elf-shlibs \
		--disable-defrag \
		--host=mipsel-linux \
		--build=i686-pc-linux-gnu ; \
	)

clean:
	if [ -f $(SRC_NAME)/Makefile ] ; then \
		$(MAKE) -C $(SRC_NAME) distclean ; \
	fi ; \
	rm -f config_done

romfs:
	$(ROMFSINST) -S $(SRC_NAME)/e2fsck/e2fsck /bin/e2fsck
	$(ROMFSINST) -S $(SRC_NAME)/misc/mke2fs /bin/mke2fs
	$(ROMFSINST) -S $(SRC_NAME)/misc/tune2fs /bin/tune2fs
	$(ROMFSINST) -S $(SRC_NAME)/lib/e2p/libe2p.so.2.3 /lib/libe2p.so.2.3
	$(ROMFSINST) -S $(SRC_NAME)/lib/et/libcom_err.so.2.1 /lib/libcom_err.so.2.1
	$(ROMFSINST) -S $(SRC_NAME)/lib/ext2fs/libext2fs.so.2.4 /lib/libext2fs.so.2.4
	$(ROMFSINST) -S $(SRC_NAME)/lib/blkid/libblkid.so.1.0 /lib/libblkid.so.1.0
	$(ROMFSINST) -S $(SRC_NAME)/lib/uuid/libuuid.so.1.2 /lib/libuuid.so.1.2
	$(ROMFSINST) -s libe2p.so.2.3 /lib/libe2p.so.2
	$(ROMFSINST) -s libcom_err.so.2.1 /lib/libcom_err.so.2
	$(ROMFSINST) -s libext2fs.so.2.4 /lib/libext2fs.so.2
	$(ROMFSINST) -s libblkid.so.1.0 /lib/libblkid.so
	$(ROMFSINST) -s libblkid.so.1.0 /lib/libblkid.so.1
	$(ROMFSINST) -s libuuid.so.1.2 /lib/libuuid.so
	$(ROMFSINST) -s libuuid.so.1.2 /lib/libuuid.so.1
	$(ROMFSINST) -S /etc/e2fsck.conf
	$(ROMFSINST) -s mke2fs /bin/mkfs.ext2
	$(ROMFSINST) -s e2fsck /bin/fsck.ext2
	$(ROMFSINST) -s mke2fs /bin/mkfs.ext3
	$(ROMFSINST) -s e2fsck /bin/fsck.ext3

