.EXPORT_ALL_VARIABLES:

-include $(LINUX_CONFIG)

ifdef CONFIG_RALINK_RT3052
ifdef CONFIG_RT_3052_ESW
CFLAGS += -DCONFIG_RT_3052_ESW -DCONFIG_RALINK_RT3052
endif
endif

ifdef CONFIG_RALINK_RT3352
ifdef CONFIG_RT_3052_ESW
CFLAGS += -DCONFIG_RT_3052_ESW -DCONFIG_RALINK_RT3352
endif
endif

ifdef CONFIG_RALINK_RT5350
ifdef CONFIG_RT_3052_ESW
CFLAGS += -DCONFIG_RT_3052_ESW -DCONFIG_RALINK_RT5350
endif
endif

ifdef CONFIG_RALINK_RT6855
ifdef CONFIG_RT_3052_ESW
CFLAGS += -DCONFIG_RT_3052_ESW -DCONFIG_RALINK_RT6855
endif
endif

ifdef CONFIG_RALINK_RT6855A
ifdef CONFIG_RT_3052_ESW
CFLAGS += -DCONFIG_RT_3052_ESW -DCONFIG_RALINK_RT6855A
endif
endif

ifdef CONFIG_RALINK_MT7620
ifdef CONFIG_RT_3052_ESW
CFLAGS += -DCONFIG_RT_3052_ESW -DCONFIG_RALINK_MT7620
endif
endif

DIRS = src cgi-src
ROOT_DIRECTORY = /web

all:
	for i in $(DIRS) ; do make -j$(HOST_NCPU) -C $$i $@ || exit $?; done

romfs:
	$(ROMFSINST) $(ROOT_DIRECTORY)
ifeq ("$(CONFIG_RT2860V2_AP_WDS)", "")
	rm -f $(ROMFSDIR)$(ROOT_DIRECTORY)/wireless/wds.asp
endif
ifeq ("$(CONFIG_RT2860V2_STA)", "")
	rm -rf $(ROMFSDIR)$(ROOT_DIRECTORY)/station
	rm -f $(ROMFSDIR)$(ROOT_DIRECTORY)/wps/wps_sta.asp
endif
ifeq ("$(CONFIG_RT2860V2_AP_WSC)", "")
ifeq ("$(CONFIG_RT2860V2_STA_WSC)", "")
	rm -rf $(ROMFSDIR)$(ROOT_DIRECTORY)/adm/wps*
	rm -rf $(ROMFSDIR)$(ROOT_DIRECTORY)/wps
endif
endif
ifeq ("$(CONFIG_RT2860V2_RT3XXX_AP_ANTENNA_DIVERSITY)", "")
ifeq ("$(CONFIG_RT2860V2_RT3XXX_STA_ANTENNA_DIVERSITY)", "")
	rm -f $(ROMFSDIR)$(ROOT_DIRECTORY)/wireless/ant_diversity.asp
endif
endif
ifeq ("$(CONFIG_USER_CHILLISPOT)", "")
	rm -rf $(ROMFSDIR)$(ROOT_DIRECTORY)/internet/hotspot.asp
endif
ifeq ("$(CONFIG_USB)", "")
	rm -rf $(ROMFSDIR)$(ROOT_DIRECTORY)/usb
ifeq ("$(CONFIG_USER_STORAGE)", "")
	rm -rf $(ROMFSDIR)$(ROOT_DIRECTORY)/usb/STORAGE*
	rm -rf $(ROMFSDIR)$(ROOT_DIRECTORY)/storage
endif
else
ifeq ("$(CONFIG_USER_STORAGE)", "")
	rm -rf $(ROMFSDIR)$(ROOT_DIRECTORY)/usb/STORAGE*
	rm -rf $(ROMFSDIR)$(ROOT_DIRECTORY)/storage
endif
ifeq ("$(CONFIG_USER_USHARE)", "")
	rm -rf $(ROMFSDIR)$(ROOT_DIRECTORY)/usb/USHARE*
endif
ifeq ("$(CONFIG_USER_UVC_STREAM)", "")
	rm -rf $(ROMFSDIR)$(ROOT_DIRECTORY)/usb/UVC*
endif
ifeq ("$(CONFIG_USER_P910ND)", "")
	rm -rf $(ROMFSDIR)$(ROOT_DIRECTORY)/usb/P910NDprintersrv.asp
endif
ifeq ("$(CONFIG_USB_MODESWITCH)", "")
	rm -rf $(ROMFSDIR)$(ROOT_DIRECTORY)/usb/USBmodem.asp
endif
endif
	for i in $(DIRS) ; do make -C $$i $@ ; done
	rm -f $(ROMFSDIR)$(ROOT_DIRECTORY)/cgi-bin/.keep_me
	###for user pages and scripts###
	mkdir -p $(ROMFSDIR)/etc/web

clean:
	for i in $(DIRS) ; do make -C $$i clean ; done
