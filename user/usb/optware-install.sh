#!/bin/sh

# Optware pre-installation script, Leon Kos 2006, 2008
# added -verbose_wget to some lines, MrAlvin 2009

REPOSITORY=http://ipkg.nslu2-linux.org/feeds/optware/ddwrt/cross/stable
TMP=/tmp

PATH=/bin:/sbin:/usr/bin:/usr/sbin:/opt/bin:/opt/sbin
unset LD_PRELOAD
unset LD_LIBRARY_PATH

_check_config()
{
    echo "Checking system config ..."
    if mount | grep -q /opt ; then
	[ -d /opt/etc ] && echo "Warning: /opt partition not empty!"
    else
	echo "Error: /opt partition not mounted."
	exit 4
    fi
}

_install_package()
{
    PACKAGE=$1
    echo "Installing package ${PACKAGE} ..."
	echo "   Some newer versions of DD-WRT does not show download progress bar,"
	echo "   so just be patient - or check STATUS -> BANDWIDTH tab for download"
	echo "   activity in your routers Web-GUI, and then still wait a minute or two."
    wget -O ${TMP}/${PACKAGE} ${REPOSITORY}/${PACKAGE}
    cd  ${TMP} 
    tar xf ${TMP}/${PACKAGE} 
    tar xf ${TMP}/control.tar.gz
    cd /
    if [ -f ${TMP}/preinst ] ; then
	sh ${TMP}/preinst
	rm -f ${TMP}/preints
    fi
    tar xf ${TMP}/data.tar.gz
    if [ -f ${TMP}/postinst ] ; then
	sh ${TMP}/postinst
	rm -f ${TMP}/postinst
    fi
    rm -f ${TMP}/data.tar.gz
    rm -f ${TMP}/control.tar.gz
    rm -f ${TMP}/control
    rm -f ${TMP}/${PACKAGE}
}

_check_config
_install_package uclibc-opt_0.9.28-13_mipsel.ipk
_install_package ipkg-opt_0.99.163-10_mipsel.ipk
/opt/sbin/ldconfig
/opt/bin/ipkg -verbose_wget update 
/opt/bin/ipkg -force-reinstall -verbose_wget install uclibc-opt
/opt/bin/ipkg -force-reinstall -verbose_wget install ipkg-opt
