#!/bin/bash
	
echo "==================CONFIGURE-MiniDLNA=============================="
APROOTDIR=`pwd`

if [ ! -f config.h ]; then
    sh ./genconfig.sh
fi
