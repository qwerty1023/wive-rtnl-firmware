/* Copyright (C) 2004, 2005 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#ifndef _NETLINKACCESS_H
#define _NETLINKACCESS_H 1

#include <features.h>
#include <stdint.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

/* Should prob be a configure option or something */
#ifdef __UCLIBC_USE_NETLINK__
# define __ASSUME_NETLINK_SUPPORT 1
#else
# define __ASSUME_NETLINK_SUPPORT 0
#endif


struct netlink_res
{
  struct netlink_res *next;
  struct nlmsghdr *nlh;
  size_t size;			/* Size of response.  */
  uint32_t seq;			/* sequential number we used.  */
};


struct netlink_handle
{
  int fd;			/* Netlink file descriptor.  */
  pid_t pid;			/* Process ID.  */
  uint32_t seq;			/* The sequence number we use currently.  */
  struct netlink_res *nlm_list;	/* Pointer to list of responses.  */
  struct netlink_res *end_ptr;	/* For faster append of new entries.  */
};


#if __ASSUME_NETLINK_SUPPORT == 0
extern int __no_netlink_support attribute_hidden;
#else
# define __no_netlink_support 0
#endif

extern int __netlink_open (struct netlink_handle *h) attribute_hidden;
extern void __netlink_close (struct netlink_handle *h) attribute_hidden;
extern void __netlink_free_handle (struct netlink_handle *h) attribute_hidden;
extern int __netlink_request (struct netlink_handle *h, int type) attribute_hidden;


#endif /* netlinkaccess.h */
