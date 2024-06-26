#! /usr/bin/make

all: $(SHARED_LIBS) $(SHARED_SE_LIBS) $(EXTRAS)

$(STATIC_LIBS): libiptc/libiptc.a

experimental: $(EXTRAS_EXP)

# Have to handle extensions which no longer exist.
clean: $(EXTRA_CLEANS)
	-rm -f $(SHARED_LIBS) $(SHARED_SE_LIBS) $(EXTRAS) $(EXTRAS_EXP) $(SHARED_LIBS:%.so=%_sh.o) $(SHARED_SE_LIBS:%.so=%_sh.o)
	-rm -f $(STATIC_LIBS) $(STATIC6_LIBS)
	-rm -f extensions/initext.c extensions/initext6.c
	-rm -f ip6tables
	@-find . -name '*.[ao]' -o -name '*.so' | xargs rm -f
	@-find . -name '*.gdb' -print | xargs rm -f

install: all $(EXTRA_INSTALLS)
	@if [ -f /usr/local/bin/iptables -a "$(BINDIR)" = "/usr/local/sbin" ];\
	then echo 'Erasing iptables from old location (now /usr/local/sbin).';\
	rm -f /usr/local/bin/iptables;\
	fi

install-experimental: $(EXTRA_INSTALLS_EXP)

TAGS:
	@rm -f $@
	find . -name '*.[ch]' | xargs etags -a

ifdef NOT_DEFINED
dep: $(DEPFILES) $(EXTRA_DEPENDS)
	@echo Dependencies will be generated on next make.
	rm -f $(DEPFILES) $(EXTRA_DEPENDS) .makefirst

$(SHARED_LIBS:%.so=%.d): %.d: %.c
	@-$(CC) -M -MG $(CFLAGS) $< | \
	    sed -e 's@^.*\.o:@$*.d $*_sh.o:@' > $@

$(SHARED_LIBS): %.so : %_sh.o
	$(CC) -shared $(EXT_LDFLAGS) -o $@ $<

$(SHARED_SE_LIBS:%.so=%.d): %.d: %.c
	@-$(CC) -M -MG $(CFLAGS) $< | \
		sed -e 's@^.*\.o:@$*.d $*_sh.o:@' > $@

$(SHARED_SE_LIBS): %.so : %_sh.o
	$(LD) -shared $(EXT_LDFLAGS) -o $@ $< $(LDLIBS)

%_sh.o : %.c
	$(CC) $(SH_CFLAGS) -o $@ -c $<

.makefirst:
	@echo Making dependencies: please wait...
	@touch .makefirst

# This is useful for when dependencies completely screwed
%.h::
	@echo "Unable to resolve dependency on $@. Try 'make clean'."
	@-rm -f $(DEPFILES) $(EXTRA_DEPENDS) .makefirst
	@[ -d $(KERNEL_DIR)/include/linux/netfilter_ipv4 ] || echo -e '\n\n    Please try `make KERNEL_DIR=path-to-correct-kernel'\'.'\n\n'
	@exit 1

-include $(DEPFILES) $(EXTRA_DEPENDS)
-include .makefirst
endif
