#ifndef __WPS__H__
#define __WPS__H__

#include "utils.h"

#define WPS_AP_TIMEOUT_SECS		120000				// 120 seconds
#define WPS_AP_TIMEOUT_SECS_SEND_M7	120000				// 120 seconds
#define WPS_AP_CATCH_CONFIGURED_TIMER	100				// 0.1 sec

#define WPS_STA_TIMEOUT_SECS		120000				// 120 seconds
#define WPS_STA_CATCH_CONFIGURED_TIMER	10				// 10 * 1000 microsecond = every 0.010 sec
#define REGISTRAR_TIMER_MODE		0xdeadbeef			// okay, this is a magic number

#define LedReset()                  {ledWps(GPIO_WPS_LED_ORANGE, WPS_LED_RESET); ledWps(GPIO_WPS_LED_GREEN, WPS_LED_RESET);}
#define LedInProgress()             {ledWps(GPIO_WPS_LED_ORANGE, WPS_LED_RESET); ledWps(GPIO_WPS_LED_GREEN, WPS_LED_RESET); ledWps(GPIO_WPS_LED_GREEN, WPS_LED_PROGRESS);}
#define LedError()                  {ledWps(GPIO_WPS_LED_ORANGE, WPS_LED_RESET); ledWps(GPIO_WPS_LED_GREEN, WPS_LED_RESET); ledWps(GPIO_WPS_LED_ORANGE, WPS_LED_ERROR);}
#define LedSessionOverlapDetected() {ledWps(GPIO_WPS_LED_ORANGE, WPS_LED_RESET); ledWps(GPIO_WPS_LED_GREEN, WPS_LED_RESET); ledWps(GPIO_WPS_LED_ORANGE, WPS_LED_SESSION_OVERLAP);}
#define LedSuccess()                {ledWps(GPIO_WPS_LED_ORANGE, WPS_LED_RESET); ledWps(GPIO_WPS_LED_GREEN, WPS_LED_RESET); ledWps(GPIO_WPS_LED_GREEN, WPS_LED_SUCCESS);}

#define WSC_CONF_STATUS_STR "WscConfStatus"
#define WSC_CONF_STATUS_UNCONFIGURED    1   /* these value are taken from 2860 driver Release Note document. */
#define WSC_CONF_STATUS_CONFIGURED      2

/*
 * ripped from driver wsc.h,....ugly
*/
#define PACKED  __attribute__ ((packed))
#define USHORT  unsigned short
#define UCHAR   unsigned char
typedef struct PACKED _WSC_CONFIGURED_VALUE {
    USHORT WscConfigured; // 1 un-configured; 2 configured
    UCHAR   WscSsid[32 + 1];
    USHORT WscAuthMode; // mandatory, 0x01: open, 0x02: wpa-psk, 0x04: shared, 0x08:wpa, 0x10: wpa2, 0x
    USHORT  WscEncrypType;  // 0x01: none, 0x02: wep, 0x04: tkip, 0x08: aes
    UCHAR   DefaultKeyIdx;
    UCHAR   WscWPAKey[64 + 1];
} WSC_CONFIGURED_VALUE;


#ifdef CONFIG_RT2860V2_STA_WSC
// WSC configured credential
typedef struct  _WSC_CREDENTIAL
{
    NDIS_802_11_SSID    SSID;               // mandatory
    USHORT              AuthType;           // mandatory, 1: open, 2: wpa-psk, 4: shared, 8:wpa, 0x10: wpa2, 0x20: wpa-psk2
    USHORT              EncrType;           // mandatory, 1: none, 2: wep, 4: tkip, 8: aes
    UCHAR               Key[64];            // mandatory, Maximum 64 byte
    USHORT              KeyLength;
    UCHAR               MacAddr[6];         // mandatory, AP MAC address
    UCHAR               KeyIndex;           // optional, default is 1
    UCHAR               Rsvd[3];            // Make alignment
}   WSC_CREDENTIAL, *PWSC_CREDENTIAL;


// WSC configured profiles
typedef struct  _WSC_PROFILE
{
#ifndef UINT
#define UINT	unsigned long
#endif
    UINT           	ProfileCnt;
    UINT		ApplyProfileIdx;  // add by johnli, fix WPS test plan 5.1.1
    WSC_CREDENTIAL  	Profile[8];             // Support up to 8 profiles
}   WSC_PROFILE, *PWSC_PROFILE;
#endif

#define WSC_ID_VERSION					0x104A
#define WSC_ID_VERSION_LEN				1
#define WSC_ID_VERSION_BEACON			0x00000001

#define WSC_ID_SC_STATE					0x1044
#define WSC_ID_SC_STATE_LEN				1
#define WSC_ID_SC_STATE_BEACON			0x00000002

#define WSC_ID_AP_SETUP_LOCKED			0x1057
#define WSC_ID_AP_SETUP_LOCKED_LEN		1
#define WSC_ID_AP_SETUP_LOCKED_BEACON	0x00000004

#define WSC_ID_SEL_REGISTRAR			0x1041
#define WSC_ID_SEL_REGISTRAR_LEN		1
#define WSC_ID_SEL_REGISTRAR_BEACON		0x00000008

#define WSC_ID_DEVICE_PWD_ID			0x1012
#define WSC_ID_DEVICE_PWD_ID_LEN		2
#define WSC_ID_DEVICE_PWD_ID_BEACON		0x00000010


#define WSC_ID_SEL_REG_CFG_METHODS		0x1053
#define WSC_ID_SEL_REG_CFG_METHODS_LEN	2
#define WSC_ID_SEL_REG_CFG_METHODS_BEACON	0x00000020

#define WSC_ID_UUID_E					0x1047
#define WSC_ID_UUID_E_LEN				16
#define WSC_ID_UUID_E_BEACON			0x00000040

#define WSC_ID_RF_BAND					0x103C
#define WSC_ID_RF_BAND_LEN				1
#define WSC_ID_RF_BAND_BEACON			0x00000080

#define WSC_ID_PRIMARY_DEVICE_TYPE		0x1054
#define WSC_ID_PRIMARY_DEVICE_TYPE_LEN	8
#define WSC_ID_PRIMARY_DEVICE_TYPE_BEACON	0x00000100

#if defined(CONFIG_RTDEV_MII) || defined(CONFIG_RTDEV_USB) || defined(CONFIG_RTDEV_PCI)
void formDefineRaixWPS(void);
unsigned int getAPPIN(char *interface);
int isSafeForShell(char *str);
int getWscProfile(char *interface, WSC_CONFIGURED_VALUE *data, int len);
void getWPSAuthMode(WSC_CONFIGURED_VALUE *result, char *ret_str);
void getWPSEncrypType(WSC_CONFIGURED_VALUE *result, char *ret_str);
int getWscStatus(char *interface);
char *getWscStatusStr(int status);
void RaixWPSRestart();
#endif

#ifdef CONFIG_USER_802_1X
void restart8021XDaemon(int nvram);
void updateFlash8021x(int nvram);
#endif

void resetTimerAll(void);
void WPSRestart(void);
void formDefineWPS(void);
void WPSAPPBCStartAll(void);
void WPSSTAPBCStartEnr(void);
#endif /* __WPS__H_ */
