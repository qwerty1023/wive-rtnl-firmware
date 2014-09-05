CFLAGS    += -ffunction-sections -fdata-sections
CXXFLAGS  += -ffunction-sections -fdata-sections
LDFLAGS   += -Wl,--gc-sections

all:
	$(MAKE) -C source

install:
	#DO NOT MAKE INSTALL NEED!!! Run make install may be crashed you system!

romfs: all
	$(ROMFSINST) -S source/bin/smbmulti	/bin/smbmulti
	$(ROMFSINST) -S smb.conf.template	/etc/smb.conf.template
	$(ROMFSINST) -S smbpasswd		/etc/smbpasswd
	$(ROMFSINST) -s /bin/smbmulti 		/bin/smbd
	$(ROMFSINST) -s /bin/smbmulti 		/bin/nmbd
	$(ROMFSINST) -s /bin/smbmulti 		/bin/smbpasswd

clean:
	$(MAKE) -C source clean

distclean:
	$(MAKE) -C source distclean
