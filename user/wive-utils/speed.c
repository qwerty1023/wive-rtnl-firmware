/*****************************************************************************/

/*
 *	cpu.c -- simple CPU usage reporting tool.
 *
 *	(C) Copyright 2000, Greg Ungerer (gerg@snapgear.com)
 */

/*****************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <getopt.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/termios.h>
#include <sys/time.h>


/*****************************************************************************/

#define PROC_IF_STATISTIC	"/proc/net/dev"

#define TXBYTE		0
#define TXPACKET	1
#define RXBYTE		2
#define RXPACKET	3

/*****************************************************************************/

static long long WANRxByteDataPrev, WANTxByteDataPrev;
static long long WANRxClockPrev, WANTxClockPrev;


/*****************************************************************************/

long long time_msec (void)
{
  struct timeval tv;

  gettimeofday (&tv, NULL);
  return (long long) tv.tv_sec * 1000 + (tv.tv_usec / 1000);
}

/*****************************************************************************/

void calcSpeed(char * strBuf, long long traffByteCur, long long traffBytePrev, long long clockCur, long long clockPrev)
{
long long traff = traffByteCur - traffBytePrev;
long long clockCount = clockCur - clockPrev;
long long scaleFactor;
double p1;

/*
if (clockCount > 0)
{
	p1=(double)traff / (double)clockCount;
	snprintf(strBuf, 32, "%.2f - %lld - %lld", p1, traff, clockCount);
}
else
{
	strcpy(strBuf, "n/a");
}

*/

if (!(traff < 0) && (clockCount > 0))
{
	scaleFactor = (traff * 1000) / clockCount;

	if (scaleFactor > (unsigned long long)1048575)
	{
		//show in MB/s
		p1=((double)traff * 1000) / (double)1048576 / (double)clockCount ;
		snprintf(strBuf, 16, "%.1f MB/s", p1);
	}
	else
	{
		//show in kB/s
		p1=((double)traff * 1000) / (double)1024 / (double)clockCount;
		snprintf(strBuf, 16, "%.1f kB/s", p1);
	}
}
else
{
	strcpy(strBuf, "n/a");
}

}

/*****************************************************************************/

void scale(char * strBuf, long long data)
{
double p1;
 
if (data > (unsigned long long)1099511627775)
{
  p1=(double)data / (double)1099511627776;
  snprintf(strBuf, 16, "%.2f T", p1);
}
else
{
  if (data > (unsigned long long)1073741823)
  {
    p1=(double)data / (double)1073741824;
    snprintf(strBuf, 16, "%.2f G", p1);
  }
  else
  {
    if (data > (unsigned long long)1048575)
    {
      p1=(double)data / (double)1048576;
      snprintf(strBuf, 16, "%.2f M", p1);
    }
    else
    {
      if (data > 1023)
      {
        p1=(double)data / (double)1024;
        snprintf(strBuf, 16, "%.2f k", p1);
      }
      else
      {
        snprintf(strBuf, 16, "%lld", data);
      }
    }
  }
}
}

/*****************************************************************************/

char* getField(char *a_line, char *delim, int count)
{
	int i=0;
	char *tok;
	tok = strtok(a_line, delim);

	while (tok)
	{
		if (i == count)
			break;
		i++;
		tok = strtok(NULL, delim);
	}

	if(tok && isdigit(*tok))
		return tok;

	return NULL;
}

/*****************************************************************************/

char *strip_space(char *str)
{
	while( *str == ' ')
		str++;
	return str;
}

/*****************************************************************************/

long long getIfStatistic(char *interface, int type)
{
	int found_flag = 0;
	int skip_line = 2;
	char buf[1024], *field, *semiColon = NULL;
	FILE *fp = fopen(PROC_IF_STATISTIC, "r");

	if(!fp)
	{
		printf("no proc?\n");
		return -1;
	}

	while (fgets(buf, 1024, fp))
	{
		char *ifname;
		if (skip_line != 0)
		{
			skip_line--;
			continue;
		}
		if ( !(semiColon = strchr(buf, ':')))
			continue;
		*semiColon = '\0';
		ifname = buf;
		ifname = strip_space(ifname);

		if (!strcmp(ifname, interface))
		{
			found_flag = 1;
			break;
		}
	}
	fclose(fp);

	semiColon++;

	switch(type){
	case TXBYTE:
		if(  (field = getField(semiColon, " ", 8))  ){
			return strtoll(field, NULL, 10);
		}
		break;
	case TXPACKET:
		if(  (field = getField(semiColon, " ", 9))  ){
			return strtoll(field, NULL, 10);
		}
		break;
	case RXBYTE:
		if(  (field = getField(semiColon, " ", 0))  ){
			return strtoll(field, NULL, 10);
		}
		break;
	case RXPACKET:
		if(  (field = getField(semiColon, " ", 1))  ){
			return strtoll(field, NULL, 10);
		}
		break;
	}
	return -1;
}

/*****************************************************************************/

int printWANRxByte()
{
	char buf[32], buf2[32];
	long long curClock = time_msec();
	long long data = getIfStatistic("eth2.2", RXBYTE);

	scale(buf, data);
	calcSpeed(buf2, data, WANRxByteDataPrev, curClock, WANRxClockPrev);
	WANRxByteDataPrev = data;
	WANRxClockPrev = curClock;
	printf("RX %s (speed %s)\n", buf, buf2);
	return 0;
}

/*****************************************************************************/

int printWANTxByte()
{
	char buf[32], buf2[32];
	long long curClock = time_msec();
	long long data = getIfStatistic("eth2.2", TXBYTE);

	scale(buf, data);
	calcSpeed(buf2, data, WANTxByteDataPrev, curClock, WANTxClockPrev);
	WANTxByteDataPrev = data;
	WANTxClockPrev = curClock;
	printf("TX %s (speed %s)\n", buf, buf2);
	return 0;
}

/*****************************************************************************/

int main(int argc, char *argv[])
{

while (1){

printWANRxByte();
printWANTxByte();


sleep(5);
}

}

/*****************************************************************************/
