CFLAGS    += -ffunction-sections -fdata-sections
CXXFLAGS  += -ffunction-sections -fdata-sections
LDFLAGS   += -Wl,--gc-sections

DIRS = snmplib modules snmpd

all romfs test:
	for i in $(DIRS) ; do make -C $$i $@ || exit $?; done

clean:
	for i in $(DIRS) ; do make -C $$i clean ; done
