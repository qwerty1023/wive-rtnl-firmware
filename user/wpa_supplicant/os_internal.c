/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
/*
 * wpa_supplicant/hostapd / Internal implementation of OS specific functions
 * Copyright (c) 2005-2006, Jouni Malinen <jkmaline@cc.hut.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 *
 * This file is an example of operating system specific  wrapper functions.
 * This version implements many of the functions internally, so it can be used
 * to fill in missing functions from the target system C libraries.
 *
 * Some of the functions are using standard C library calls in order to keep
 * this file in working condition to allow the functions to be tested on a
 * Linux target. Please note that OS_NO_C_LIB_DEFINES needs to be defined for
 * this file to work correctly. Note that these implementations are only
 * examples and are not optimized for speed.
 */

#include "includes.h"

#undef OS_REJECT_C_LIB_FUNCTIONS
#include "os.h"

void os_sleep(os_time_t sec, os_time_t usec)
{
	if (sec)
		sleep(sec);
	if (usec)
		usleep(usec);
}


int os_get_time(struct os_time *t)
{
	int res;
	struct timeval tv;
	res = gettimeofday(&tv, NULL);
	t->sec = tv.tv_sec;
	t->usec = tv.tv_usec;
	return res;
}


int os_mktime(int year, int month, int day, int hour, int min, int sec,
	      os_time_t *t)
{
	struct tm tm;

	if (year < 1970 || month < 1 || month > 12 || day < 1 || day > 31 ||
	    hour < 0 || hour > 23 || min < 0 || min > 59 || sec < 0 ||
	    sec > 60)
		return -1;

	os_memset(&tm, 0, sizeof(tm));
	tm.tm_year = year - 1900;
	tm.tm_mon = month - 1;
	tm.tm_mday = day;
	tm.tm_hour = hour;
	tm.tm_min = min;
	tm.tm_sec = sec;

	*t = (os_time_t) mktime(&tm);
	return 0;
}


int os_daemonize(const char *pid_file)
{
	if (daemon(0, 0)) {
		perror("daemon");
		return -1;
	}

	if (pid_file) {
		FILE *f = fopen(pid_file, "w");
		if (f) {
			fprintf(f, "%u\n", getpid());
			fclose(f);
		}
	}

	return -0;
}


void os_daemonize_terminate(const char *pid_file)
{
	if (pid_file)
		unlink(pid_file);
}


int os_get_random(unsigned char *buf, size_t len)
{
	FILE *f;
	size_t rc;

	f = fopen("/dev/urandom", "rb");
	if (f == NULL) {
		printf("Could not open /dev/urandom.\n");
		return -1;
	}

	rc = fread(buf, 1, len, f);
	fclose(f);

	return rc != len ? -1 : 0;
}


unsigned long os_random(void)
{
	return random();
}


char * os_rel2abs_path(const char *rel_path)
{
	char *buf = NULL, *cwd, *ret;
	size_t len = 128, cwd_len, rel_len, ret_len;

	if (rel_path[0] == '/')
		return os_strdup(rel_path);

	for (;;) {
		buf = os_malloc(len);
		if (buf == NULL)
			return NULL;
		cwd = getcwd(buf, len);
		if (cwd == NULL) {
			os_free(buf);
			if (errno != ERANGE) {
				return NULL;
			}
			len *= 2;
		} else {
			break;
		}
	}

	cwd_len = strlen(cwd);
	rel_len = strlen(rel_path);
	ret_len = cwd_len + 1 + rel_len + 1;
	ret = os_malloc(ret_len);
	if (ret) {
		os_memcpy(ret, cwd, cwd_len);
		ret[cwd_len] = '/';
		os_memcpy(ret + cwd_len + 1, rel_path, rel_len);
		ret[ret_len - 1] = '\0';
	}
	os_free(buf);
	return ret;
}


int os_program_init(void)
{
	return 0;
}


void os_program_deinit(void)
{
}


int os_setenv(const char *name, const char *value, int overwrite)
{
	return setenv(name, value, overwrite);
}


int os_unsetenv(const char *name)
{
#if defined(__FreeBSD__) || defined(__NetBSD__)
	unsetenv(name);
	return 0;
#else
	return unsetenv(name);
#endif
}


char * os_readfile(const char *name, size_t *len)
{
	FILE *f;
	char *buf;

	f = fopen(name, "rb");
	if (f == NULL)
		return NULL;

	fseek(f, 0, SEEK_END);
	*len = ftell(f);
	fseek(f, 0, SEEK_SET);

	buf = os_malloc(*len);
	if (buf == NULL) {
		fclose(f);
		return NULL;
	}

	fread(buf, 1, *len, f);
	fclose(f);

	return buf;
}


void * os_zalloc(size_t size)
{
	void *n = os_malloc(size);
	if (n)
		os_memset(n, 0, size);
	return n;
}


void * os_malloc(size_t size)
{
	return malloc(size);
}


void * os_realloc(void *ptr, size_t size)
{
	return realloc(ptr, size);
}


void os_free(void *ptr)
{
	free(ptr);
}


void * os_memcpy(void *dest, const void *src, size_t n)
{
	char *d = dest;
	const char *s = src;
	while (n--)
		*d++ = *s++;
	return dest;
}


void * os_memmove(void *dest, const void *src, size_t n)
{
	if (dest < src)
		os_memcpy(dest, src, n);
	else {
		/* overlapping areas */
		char *d = (char *) dest + n;
		const char *s = (const char *) src + n;
		while (n--)
			*--d = *--s;
	}
	return dest;
}


void * os_memset(void *s, int c, size_t n)
{
	char *p = s;
	while (n--)
		*p++ = c;
	return s;
}


int os_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char *p1 = s1, *p2 = s2;

	if (n == 0)
		return 0;

	while (*p1 == *p2) {
		p1++;
		p2++;
		n--;
		if (n == 0)
			return 0;
	}

	return *p1 - *p2;
}


char * os_strdup(const char *s)
{
	char *res;
	size_t len;
	if (s == NULL)
		return NULL;
	len = os_strlen(s);
	res = os_malloc(len + 1);
	if (res)
		os_memcpy(res, s, len + 1);
	return res;
}


size_t os_strlen(const char *s)
{
	const char *p = s;
	while (*p)
		p++;
	return p - s;
}


int os_strcasecmp(const char *s1, const char *s2)
{
	/*
	 * Ignoring case is not required for main functionality, so just use
	 * the case sensitive version of the function.
	 */
	return os_strcmp(s1, s2);
}


int os_strncasecmp(const char *s1, const char *s2, size_t n)
{
	/*
	 * Ignoring case is not required for main functionality, so just use
	 * the case sensitive version of the function.
	 */
	return os_strncmp(s1, s2, n);
}


char * os_strchr(const char *s, int c)
{
	while (*s) {
		if (*s == c)
			return (char *) s;
		s++;
	}
	return NULL;
}


char * os_strrchr(const char *s, int c)
{
	const char *p = s;
	while (*p)
		p++;
	p--;
	while (p >= s) {
		if (*p == c)
			return (char *) p;
		p--;
	}
	return NULL;
}


int os_strcmp(const char *s1, const char *s2)
{
	while (*s1 == *s2) {
		if (*s1 == '\0')
			break;
		s1++;
		s2++;
	}

	return *s1 - *s2;
}


int os_strncmp(const char *s1, const char *s2, size_t n)
{
	if (n == 0)
		return 0;

	while (*s1 == *s2) {
		if (*s1 == '\0')
			break;
		s1++;
		s2++;
		n--;
		if (n == 0)
			return 0;
	}

	return *s1 - *s2;
}


char * os_strncpy(char *dest, const char *src, size_t n)
{
	char *d = dest;

	while (n--) {
		*d = *src;
		if (*src == '\0')
			break;
		d++;
		src++;
	}

	return dest;
}


char * os_strstr(const char *haystack, const char *needle)
{
	size_t len = os_strlen(needle);
	while (*haystack) {
		if (os_strncmp(haystack, needle, len) == 0)
			return (char *) haystack;
		haystack++;
	}

	return NULL;
}


int os_snprintf(char *str, size_t size, const char *format, ...)
{
	va_list ap;
	int ret;

	/* See http://www.ijs.si/software/snprintf/ for portable
	 * implementation of snprintf.
	 */

	va_start(ap, format);
	ret = vsnprintf(str, size, format, ap);
	va_end(ap);
	if (size > 0)
		str[size - 1] = '\0';
	return ret;
}
