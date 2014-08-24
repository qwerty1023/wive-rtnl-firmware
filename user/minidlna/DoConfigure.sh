#!/bin/bash

echo "==================CONFIGURE-MiniDLNA=========================="
APROOTDIR=`pwd`

if [ ! -f $APROOTDIR/configure ]; then
    aclocal
    autoconf
fi
if [ ! -f $APROOTDIR/Makefile.in ]; then
    automake --add-missing
    automake
fi

STAGEDIR=stage

export CFLAGS="$CFLAGS -I$STAGEDIR/include -I$FIRMROOT/lib/include_shared/"
export LDFLAGS="$LDFLAGS -L$STAGEDIR/lib -L$FIRMROOT/lib/lib/"
export LIBS="$LIBS -ldl -lm -lpthread -lz -lexif -ljpeg -lsqlite3 -lid3tag -lFLAC -lvorbis -logg -lavformat -lavcodec -lavutil"

HBUILD=`uname -m`-pc-linux-gnu
HTARGET=mipsel-linux

#arch options
CONFOPTS="--host=$HTARGET --target=$HTARGET --build=$HBUILD"

ac_cv_lib_avformat_av_open_input_file=yes

./configure $CONFOPTS \
	    --prefix=$APROOTDIR/filesystem \
	    --disable-nls \
	    --disable-asus \
	    --enable-tivo \
	    --with-libiconv-prefix=$FIRMROOT/lib/lib/ \
	    --with-zlib=$FIRMROOT/lib/lib/ \
	    --with-zlib-includes=$FIRMROOT/lib/include_shared/ \
	    --with-log-path=/media/sda1/minidlna \
	    --with-db-path=/media/sda1/minidlna \
	    --with-os-name="Linux" \
	    --with-os-version="Linux/Cross_compiled" \
	    --with-os-url="http://www.kernel.org/"

