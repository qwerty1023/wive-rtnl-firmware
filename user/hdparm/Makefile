# Makefile for hdparm


OBJS = hdparm.o identify.o sgio.o sysfs.o geom.o fallocate.o fibmap.o fwdownload.o dvdspeed.o

all: hdparm

hdparm: hdparm.h sgio.h $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o hdparm $(OBJS)

hdparm.o:	hdparm.h sgio.h

identify.o:	hdparm.h

dvdspeed.o:     dvdspeed.c

sgio.o: sgio.c sgio.h hdparm.h

fwdownload.o: fwdownload.c sgio.h hdparm.h

install: all hdparm.8

romfs:
	$(ROMFSINST) /sbin/hdparm

clean:
	-rm -f hdparm $(OBJS) core 2>/dev/null

