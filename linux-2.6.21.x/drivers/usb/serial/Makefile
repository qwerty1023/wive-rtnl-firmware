#
# Makefile for the USB serial device drivers.
#

# Object file lists.

obj-$(CONFIG_USB_SERIAL)			+= usbserial.o

usbserial-obj-$(CONFIG_USB_SERIAL_CONSOLE)	+= console.o
usbserial-obj-$(CONFIG_USB_EZUSB)		+= ezusb.o

usbserial-objs	:= usb-serial.o generic.o bus.o $(usbserial-obj-y)

obj-$(CONFIG_USB_SERIAL_AIRCABLE)		+= aircable.o
obj-$(CONFIG_USB_SERIAL_AIRPRIME)		+= airprime.o
obj-$(CONFIG_USB_SERIAL_ARK3116)		+= ark3116.o
obj-$(CONFIG_USB_SERIAL_BELKIN)			+= belkin_sa.o
obj-$(CONFIG_USB_SERIAL_CH341)			+= ch341.o
obj-$(CONFIG_USB_SERIAL_CP2101)			+= cp2101.o
obj-$(CONFIG_USB_SERIAL_CYBERJACK)		+= cyberjack.o
obj-$(CONFIG_USB_SERIAL_CYPRESS_M8)		+= cypress_m8.o
obj-$(CONFIG_USB_SERIAL_DEBUG)			+= usb_debug.o
obj-$(CONFIG_USB_SERIAL_DIGI_ACCELEPORT)	+= digi_acceleport.o
obj-$(CONFIG_USB_SERIAL_EDGEPORT)		+= io_edgeport.o
obj-$(CONFIG_USB_SERIAL_EDGEPORT_TI)		+= io_ti.o
obj-$(CONFIG_USB_SERIAL_EMPEG)			+= empeg.o
obj-$(CONFIG_USB_SERIAL_FTDI_SIO)		+= ftdi_sio.o
obj-$(CONFIG_USB_SERIAL_FUNSOFT)		+= funsoft.o
obj-$(CONFIG_USB_SERIAL_GARMIN)			+= garmin_gps.o
obj-$(CONFIG_USB_SERIAL_HP4X)			+= hp4x.o
obj-$(CONFIG_USB_SERIAL_IPAQ)			+= ipaq.o
obj-$(CONFIG_USB_SERIAL_IPW)			+= ipw.o
obj-$(CONFIG_USB_SERIAL_IR)			+= ir-usb.o
obj-$(CONFIG_USB_SERIAL_KEYSPAN)		+= keyspan.o
obj-$(CONFIG_USB_SERIAL_KEYSPAN_PDA)		+= keyspan_pda.o
obj-$(CONFIG_USB_SERIAL_KLSI)			+= kl5kusb105.o
obj-$(CONFIG_USB_SERIAL_KOBIL_SCT)		+= kobil_sct.o
obj-$(CONFIG_USB_SERIAL_MCT_U232)		+= mct_u232.o
obj-$(CONFIG_USB_SERIAL_MOS7720)		+= mos7720.o
obj-$(CONFIG_USB_SERIAL_MOS7840)		+= mos7840.o
obj-$(CONFIG_USB_SERIAL_NAVMAN)			+= navman.o
obj-$(CONFIG_USB_SERIAL_OMNINET)		+= omninet.o
obj-$(CONFIG_USB_SERIAL_OPTION)			+= option.o
obj-$(CONFIG_USB_SERIAL_PL2303)			+= pl2303.o
obj-$(CONFIG_USB_SERIAL_SAFE)			+= safe_serial.o
obj-$(CONFIG_USB_SERIAL_SIERRAWIRELESS)		+= sierra.o
obj-$(CONFIG_USB_SERIAL_TI)			+= ti_usb_3410_5052.o
obj-$(CONFIG_USB_SERIAL_VISOR)			+= visor.o
obj-$(CONFIG_USB_SERIAL_WHITEHEAT)		+= whiteheat.o
obj-$(CONFIG_USB_SERIAL_XIRCOM)			+= keyspan_pda.o
obj-$(CONFIG_USB_SERIAL_HSO)			+= hso.o
