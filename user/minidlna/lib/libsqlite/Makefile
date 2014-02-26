SRC_NAME=sqlite-autoconf-3070701

all:	config_test
	$(MAKE) -C $(SRC_NAME)

config_test:
	( if [ -f ./config_done ]; then \
		echo "the same configuration"; \
	else \
		make configure && touch config_done; \
	fi ) 

configure:
	( cd $(SRC_NAME) && CFLAGS="-Wall -Wstrict-prototypes -Wno-trigraphs  -mips32r2 -march=mips32r2 -fno-stack-protector -fno-builtin -nostdinc -D_LIBC -I./include -I.  -Os -funit-at-a-time -fforce-addr -fno-stack-protector -isystem /home/sandra/source/wive-rtnl-firmware/toolchain/lib/gcc/mipsel-linux-uclibc/4.8.2/include -DNDEBUG -fPIC -Os -I/home/sandra/source/wive-rtnl-firmware/linux/include -I/home/sandra/source/wive-rtnl-firmware/lib/include" \
	./configure \
		--prefix=/ \
		--disable-readline \
		--enable-static \
		--disable-shared \
		--host=mipsel-linux \
		--build=i686-pc-linux-gnu ; \
	)

clean:
	$(MAKE) -C $(SRC_NAME) distclean; \
	rm -f config_done

install:
	$(MAKE) -C $(SRC_NAME) install


