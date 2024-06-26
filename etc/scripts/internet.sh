#!/bin/sh

########################################################
# internet.sh - reconfigure network helper for goahead #
########################################################

# include global config
. /etc/scripts/global.sh

# restart mode
MODE="$1"

LOG="logger -t reconfig"

addWds() {
    # if kernel build without WDS support - exit
    wds_en=`nvram_get 2860 WdsEnable`
    wdsif="$1"
    if [ "$wds_en" != "0" ]; then
        for i in `seq 0 3`; do
	$LOG "Readd ${wdsif}${i} in br0"
	readdif_to_br ${wdsif}${i}
        done
    fi
}

addMBSSID() {
    # if kernel build without Multiple SSID support - exit
    bssidnum=`nvram_get 2860 BssidNum`
    mbssidif="$1"
    if [ "$bssidnum" != "0" ] && [ "$bssidnum" != "1" ]; then
	let "bssrealnum=$bssidnum-1"
	for i in `seq 1 $bssrealnum`; do
	    $LOG "Readd ${mbssidif}${i} in br0"
	    readdif_to_br ${mbssidif}${i}
	done
    fi
}

bridge_config() {
	$LOG "Bridge OperationMode: $OperationMode"
	# flush eth2 ip and remove from bridge
	# in bridge mode add only eth2 NOT ADD "$phys_lan_if" or "$phys_wan_if"
	$LOG "Readd eth2 in br0"
	readdif_to_br eth2
	if [ "$CONFIG_RAETH_GMAC2" != "" ]; then
	    $LOG "Readd eth3 in br0"
	    readdif_to_br eth3
	fi
	$LOG "Readd $first_wlan_root_if in br0"
	readdif_to_br $first_wlan_root_if
	if [ "$second_wlan_root_if" != "" ]; then
	    $LOG "Readd $second_wlan_root_if in br0"
	    readdif_to_br $second_wlan_root_if
	fi
	if [ "$first_wlan_mbss" != "" ]; then
	    addMBSSID $first_wlan_mbss
	fi
	if [ "$second_wlan_mbss" != "" ]; then
	    addMBSSID $second_wlan_mbss
	fi
	if [ "$first_wlan_wds" != "" ]; then
	    addWds $first_wlan_wds
	fi
	if [ "$second_wlan_wds" != "" ]; then
	    addWds $second_wlan_wds
	fi
}

gate_config() {
	$LOG "Gateway OperationMode: $OperationMode"
	# flush "$phys_lan_if" ip and remove from bridge
	# add lan interface
	$LOG "Readd $phys_lan_if in br0"
	readdif_to_br $phys_lan_if
	$LOG "Readd $first_wlan_root_if in br0"
	readdif_to_br $first_wlan_root_if
	if [ "$second_wlan_root_if" != "" ]; then
	    $LOG "Readd $second_wlan_root_if in br0"
	    readdif_to_br $second_wlan_root_if
	fi
	if [ "$first_wlan_mbss" != "" ]; then
	    addMBSSID $first_wlan_mbss
	fi
	if [ "$second_wlan_mbss" != "" ]; then
	    addMBSSID $second_wlan_mbss
	fi
	if [ "$first_wlan_wds" != "" ]; then
	    addWds $first_wlan_wds
	fi
	if [ "$second_wlan_wds" != "" ]; then
	    addWds $second_wlan_wds
	fi
}

apcli_config() {
	$LOG "ApClient OperationMode: $OperationMode"
	# flush eth2 ip and remove from bridge
	# in bridge mode add only eth2 NOT ADD "$phys_lan_if" or "$phys_wan_if"
	$LOG "Readd eth2 in br0"
	readdif_to_br eth2
	if [ "$CONFIG_RAETH_GMAC2" != "" ]; then
	    $LOG "Readd eth3 in br0"
	    readdif_to_br eth3
	fi
	$LOG "Readd $first_wlan_root_if in br0"
	readdif_to_br $first_wlan_root_if
	if [ "$second_wlan_root_if" != "" ]; then
	    $LOG "Readd $second_wlan_root_if in br0"
	    readdif_to_br $second_wlan_root_if
	fi
	if [ "$ApCliBridgeOnly" = "1" ]; then
	    if [ "$first_wlan_apcli" != "" ]; then
		$LOG "Readd $first_wlan_apcli in br0"
		readdif_to_br $first_wlan_apcli
	    fi
	    if [ "$second_wlan_apcli" != "" ]; then
		$LOG "Readd $second_wlan_apcli in br0"
		readdif_to_br $second_wlan_apcli
	    fi
	fi
	if [ "$first_wlan_mbss" != "" ]; then
	    addMBSSID $first_wlan_mbss
	fi
	if [ "$second_wlan_mbss" != "" ]; then
	    addMBSSID $second_wlan_mbss
	fi
	# delete ra0 from bridge and down if only apcli-bridge
	eval `nvram_buf_get 2860 ApCliClientOnly`
	if  [ "$ApCliClientOnly" = "1" ] && [ "$OperationMode" = "0" -o "$OperationMode" = "3" ]; then
    	    echo "APCLI Only client mode enable, shutdown $first_wlan_root_if interface."
    	    delif_from_br $first_wlan_root_if
	fi
}

spot_config() {
	$LOG "HotSpot OperationMode: $OperationMode"
	# flush "$phys_lan_if" ip and remove from bridge
	# add lan interface
	$LOG "Readd $phys_lan_if in br0"
	readdif_to_br $phys_lan_if
	$LOG "Readd $first_wlan_root_if in br0"
	readdif_to_br $first_wlan_root_if
	if [ "$second_wlan_root_if" != "" ]; then
	    $LOG "Readd $second_wlan_root_if in br0"
	    readdif_to_br $second_wlan_root_if
	fi
	if [ "$first_wlan_mbss" != "" ]; then
	    addMBSSID $first_wlan_mbss
	fi
	if [ "$second_wlan_mbss" != "" ]; then
	    addMBSSID $second_wlan_mbss
	fi
	if [ "$first_wlan_wds" != "" ]; then
	    addWds $first_wlan_wds
	fi
	if [ "$second_wlan_wds" != "" ]; then
	    addWds $second_wlan_wds
	fi
}

ethcv_config() {
	$LOG "Ethernet Converter OperationMode: $OperationMode"
}

# WiFi modules reloand and reconfigure
if [ "$MODE" != "connect_sta" ]; then
    if [ "$MODE" != "wifionly" ]; then
	# all tunnelst deconfig before modules reload
	if [ -e /etc/init.d/radvd ] && [ -d /proc/sys/net/ipv6 ]; then
	    service radvd stop
	fi
	service vpnhelper stop_safe
    fi
    $LOG "Reload wireless modules..."
    service modules restart
    if [ "$MODE" != "wifionly" ]; then
	$LOG "Reconfigure lan..."
	service lan restart
    fi
fi

# OperationMode adjustment:
# init ip address to all interfaces for different OperationMode:
#   0 = Bridge Mode
#   1 = Gateway Mode
#   2 = Ethernet Converter Mode
#   3 = AP Client
#   4 = Hotspot Mode
#
if [ "$OperationMode" = "0" ]; then
    bridge_config
elif [ "$OperationMode" = "1" ]; then
    gate_config
elif [ "$OperationMode" = "2" ] && [ "$CONFIG_RT2860V2_STA" != "" ]; then
    ethcv_config
elif [ "$OperationMode" = "3" ] && [ "$CONFIG_RT2860V2_AP_APCLI" != "" ]; then
    apcli_config
elif [ "$OperationMode" = "4" ] && [ -f /bin/chilli ]; then
    spot_config
else
    $LOG "unknown OperationMode use gate_config: $OperationMode"
    OperationMode=1
    gate_config
fi

if [ "$MODE" != "wifionly" ] && [ "$MODE" != "connect_sta" ]; then
    $LOG "Reconfigure wan..."
    service wan restart
fi

if [ "$MODE" != "connect_sta" ]; then
    $LOG "Tune wifi modules..."
    service modules retune_wifi
fi

if [ "$OperationMode" = "0" -o "$OperationMode" = "3" ] && [ "$MODE" != "connect_sta" ]; then
    # this is workaround for internal ESW in some RT305x chips in config all lan ports in one bridge
    # for restore normal swich work after reconfigure wifi drivers
    if [ "$CONFIG_RT_3052_ESW" != "" ] && [ "$CONFIG_RALINK_RT3052" != "" ]; then
	$LOG "Reconfigure switch..."
	/etc/scripts/config-switch.sh
    fi
fi

##########################################################
# Regenerate resolv only if wan_static_dns on		 #
##########################################################
if [ "$wan_static_dns" = "on" ]; then
    service resolv start
fi

# some daemons need restart
services_restart.sh all

# in dhcp client mode restart from dhcp script
# in static/zeroconf or pure pppoe mode need restart anyway
if [ "$vpnEnabled" = "on" -a "$vpnType" = "0" -a "$vpnPurePPPOE" = "1" ] || [ "$wanConnectionMode" != "DHCP" ]; then
    service vpnhelper restart
fi

# this is hook for exec user script after physycal connection configured
# may be used for add scripts needed by some provides
# example: http://wive-ng.sf.net/downloads/wan_up.sh - load external routes for www.kvidex.ru ISP
if [ -f /etc/wan_up.sh ]; then
    chmod 755 /etc/wan_up.sh
    $LOG "Call user /etc/wan_up.sh script."
    sh /etc/wan_up.sh &
fi
