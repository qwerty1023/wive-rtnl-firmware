GENLOBJ=genl.o

GENLMODULES :=
GENLMODULES += ctrl.o

GENLOBJ += $(GENLMODULES)

GENLLIB :=

LDFLAGS += -Wl,-export-dynamic
LDLIBS  += -lm -ldl

all: genl

genl: $(GENLOBJ) $(LIBNETLINK) $(LIBUTIL) $(GENLLIB)

romfs: all
	$(ROMFSINST) -S /bin/genl

clean:
	rm -f $(GENLOBJ) $(GENLLIB) genl
