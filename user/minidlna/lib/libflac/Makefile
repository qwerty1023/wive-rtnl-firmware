SRC_NAME=flac-1.2.1

all:	config_test
	$(MAKE) -C $(SRC_NAME)

config_test:
	( if [ -f ./config_done ]; then \
		echo "the same configuration"; \
	else \
		make configure && touch config_done; \
	fi ) 

configure:
	( cd $(SRC_NAME) ; \
	./configure \
		--prefix=/ \
		--disable-cpplibs \
		--enable-static \
		--disable-shared \
		--with-ogg-libraries=$(DESTDIR)/lib \
		--disable-oggtest \
		--host=mipsel-linux \
		--build=i686-pc-linux-gnu ; \
	)

clean:
	$(MAKE) -C $(SRC_NAME) distclean; \
	rm -f config_done

install:
	$(MAKE) -C $(SRC_NAME) install

romfs:
	if [ $(CONFIG_LIB_BUILD_SHARED) = "y" ] ; then \
		cp -fP $(DESTDIR)/lib/libFLAC.so* $(ROMFSDIR)/lib ; \
	fi
