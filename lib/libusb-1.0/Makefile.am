AUTOMAKE_OPTIONS = dist-bzip2 no-dist-gzip
ACLOCAL_AMFLAGS = -I m4
DISTCLEANFILES = libusb-1.0.pc filesystem
EXTRA_DIST = TODO PORTING msvc libusb/libusb-1.0.def libusb/version_nano.h \
  examples/getopt/getopt.c examples/getopt/getopt1.c examples/getopt/getopt.h
SUBDIRS = libusb

pkgconfigdir=
pkgconfig_DATA=libusb-1.0.pc

.PHONY: dist-up

reldir = .release/$(distdir)
dist-up: dist
	rm -rf $(reldir)
	mkdir -p $(reldir)
	cp $(distdir).tar.bz2 $(reldir)
	rsync -rv $(reldir) frs.sourceforge.net:/home/frs/project/l/li/libusb/libusb-1.0/
	rm -rf $(reldir)

shared: install
	cp -vfa ./filesystem/lib/*.*    $(ROOTDIR)/lib/lib
	cp -vfra ./filesystem/include/* $(ROOTDIR)/lib/include_shared
	rm -rf ./filesystem
