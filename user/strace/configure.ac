dnl Process this file with autoconf to create configure.  Use autoreconf.
AC_PREREQ(2.57)
AC_INIT([strace],[4.5.15])
AC_CONFIG_SRCDIR([strace.c])
AС_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE([foreign check-news dist-bzip2])
AM_MAINTAINER_MODE
AC_CANONICAL_HOST

AC_MSG_CHECKING([for supported operating system])
case "$host_os" in
linux*)
	opsys=linux
	AC_DEFINE([LINUX], 1, [Define for the Linux operating system.])
	;;
sunos4*)
	opsys=sunos4
	AC_DEFINE([SUNOS4], 1, [Define for the SunOS 4.x operating system.])
	;;
solaris2* | sysv[[45]]* | irix[[56]]*)
	opsys=svr4
	AC_DEFINE([SVR4], 1, [Define for the System V release 4 operating
system or a derivative like Solaris 2.x or Irix 5.x.])
	case "$host_os" in
	sysv4.2uw*)
		AC_DEFINE(UNIXWARE, 2, [Define for UnixWare systems.])
		;;
	sysv5*)
		AC_DEFINE(UNIXWARE, 7, [Define for UnixWare systems.])
		;;
	esac
	;;
freebsd*)
	opsys=freebsd
	AC_DEFINE([FREEBSD], 1, [Define for the FreeBSD operating system.])
	;;
*)
	AC_MSG_RESULT([NO!])
	AC_MSG_ERROR([operating system $host_os is not supported by strace])
	;;
esac
AC_MSG_RESULT($opsys)

AC_MSG_CHECKING([for supported architecture])
case "$host_cpu" in
i[[3456]]86|pentium)
	arch=i386
	AC_DEFINE([I386], 1, [Define for the i386 architecture.])
	;;
ia64)
	arch=ia64
	AC_DEFINE([IA64], 1, [Define for the IA64 architecture.])
	;;
m68k)
	arch=m68k
	AC_DEFINE([M68K], 1, [Define for the m68k architecture.])
	;;
sparc64*)
	arch=sparc64
	AC_DEFINE([SPARC64], 1, [Define for the SPARC64 architecture.])
	;;
sparc*)
	arch=sparc
	AC_DEFINE([SPARC], 1, [Define for the SPARC architecture.])
	;;
mips*)
	arch=mips
	AC_DEFINE([MIPS], 1, [Define for the MIPS architecture.])
	;;
alpha*)
	arch=alpha
	AC_DEFINE([ALPHA], 1, [Define for the Alpha architecture.])
	;;
powerpc*)
	arch=powerpc
	AC_DEFINE([POWERPC], 1, [Define for the PowerPC architecture.])
	;;
arm*)
	arch=arm
	AC_DEFINE([ARM], 1, [Define for the ARM architecture.])
	;;
s390)
	arch=s390
	AC_DEFINE([S390], 1, [Define for the S390 architecture.])
	;;
s390x)
	arch=s390x
	AC_DEFINE([S390X], 1, [Define for the S390x architecture.])
	;;
hppa*|parisc*)
	arch=hppa
	AC_DEFINE([HPPA], 1, [Define for the HPPA architecture.])
	;;
sh)
	arch=sh
	AC_DEFINE([SH], 1, [Define for the SH architecture.])
	;;
sh64)
	arch=sh64
	AC_DEFINE([SH64], 1, [Define for the SH64 architecture.])
	;;
x86?64*)
	arch=x86_64
	AC_DEFINE([X86_64], 1, [Define for the AMD x86-64 architecture.])
	;;
*)
	AC_MSG_RESULT([NO!])
	AC_MSG_ERROR([architecture $host_cpu is not supported by strace])
	;;
esac
AC_MSG_RESULT($arch)

AC_SUBST(opsys)
AC_SUBST(arch)

AM_CONDITIONAL([LINUX], [test x$opsys = xlinux])
AM_CONDITIONAL([I386], [test x$arch = xi386])
AM_CONDITIONAL([X86_64], [test x$arch = xx86_64])
AM_CONDITIONAL([SUNOS4], [test x$opsys = xsunos4])
AM_CONDITIONAL([SVR4], [test x$opsys = xsvr4])
AM_CONDITIONAL([FREEBSD], [test x$opsys = xfreebsd])

AC_PROG_CC
AC_GNU_SOURCE

AC_INCLUDEDIR

if test "x$opsys" = "xsunos4" && test "x$arch" = "xsparc"
then
	AC_MSG_CHECKING(for valid machine include directory)
	if test -d "$includedir/sun4"
	then
		rm -f machine
		ln -s $includedir/sun4 machine
		AC_MSG_RESULT(yes)
		AC_DEFINE(SUNOS4_KERNEL_ARCH_KLUDGE, 1, [
Define if you are have a SPARC with SUNOS4 and your want a version
of strace that will work on sun4, sun4c and sun4m kernel architectures.
Only useful if you have a symbolic link from machine to /usr/include/sun4
in the compilation directory.])
	else
		AC_MSG_RESULT(no)
	fi
fi

AC_WARNFLAGS
if test "x$opsys" = "xsunos4"
then
	if test -n "$GCC"
	then
		# SunOS 4.x header files don't declare int functions.
		WARNFLAGS="$WARNFLAGS -Wno-implicit"
	fi
fi

AC_PROG_CPP
AC_PROG_GCC_TRADITIONAL
AC_PROG_INSTALL
AC_C_CONST
AC_HEADER_STDC
AC_HEADER_DIRENT
AC_HEADER_STAT
AC_CHECK_MEMBERS([struct stat.st_blksize,
		  struct stat.st_blocks,
		  struct stat.st_aclcnt,
		  struct stat.st_flags,
		  struct stat.st_fstype,
		  struct stat.st_gen,
		  struct stat.st_level,
		  struct stat.st_rdev])
AC_STAT64

AC_TYPE_SIGNAL
AC_TYPE_UID_T
AC_TYPE_MODE_T
AC_TYPE_GETGROUPS
AC_HEADER_MAJOR
AC_CHECK_TYPES([sig_atomic_t, siginfo_t],,, [#include <signal.h>])
AC_CHECK_MEMBERS([struct sockaddr_in6.sin6_scope_id],,,
[#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>])
AC_CHECK_TYPES([long long])
AC_LITTLE_ENDIAN_LONG_LONG
AC_OFF_T_IS_LONG_LONG
AC_RLIM_T_IS_LONG_LONG
AC_CHECK_TYPES([struct opthdr],,, [#include <sys/socket.h>])
AC_CHECK_TYPES([struct t_opthdr],,, [#include <sys/tiuser.h>])

if test x$opsys != xlinux; then
AC_CHECK_LIB(nsl, main)
fi

AC_CHECK_FUNCS(sigaction strerror strsignal pread sys_siglist _sys_siglist getdents mctl prctl sendmsg inet_ntop if_indextoname)
AC_CHECK_HEADERS([sys/reg.h sys/filio.h sys/acl.h sys/asynch.h sys/door.h stropts.h sys/conf.h sys/stream.h sys/tihdr.h sys/tiuser.h sys/sysconfig.h ioctls.h sys/ioctl.h sys/ptrace.h termio.h linux/ptrace.h asm/reg.h sys/uio.h sys/aio.h poll.h sys/poll.h sys/vfs.h asm/sysmips.h linux/utsname.h sys/nscsys.h mqueue.h sys/epoll.h libaio.h inttypes.h], [], [])
AC_CHECK_HEADERS([linux/icmp.h linux/in6.h linux/netlink.h linux/if_packet.h],
                 [], [], [#include <linux/socket.h>])
AC_CHECK_HEADERS([asm/sigcontext.h], [], [], [#include <signal.h>])
AC_CHECK_HEADERS([netinet/tcp.h netinet/udp.h],,, [#include <netinet/in.h>])

AC_MP_PROCFS
AC_POLLABLE_PROCFS

AC_CHECK_MEMBERS([struct msghdr.msg_control],,, [#include <sys/socket.h>])
AC_STRUCT_PR_SYSCALL

AC_CHECK_MEMBERS([struct T_conn_res.QUEUE_ptr,
                  struct T_conn_res.ACCEPTOR_id],,,
[#include <sys/stream.h>
#include <sys/tihdr.h>])

AC_CHECK_TYPES([struct __old_kernel_stat],,, [#include <asm/stat.h>])

AC_CHECK_TYPES([struct pt_all_user_regs, struct ia64_fpreg],,,
	       [#include <sys/ptrace.h>])

AC_CHECK_TYPES([struct user_desc],,, [#include <asm/ldt.h>])

AC_CHECK_MEMBERS([struct dqblk.dqb_curblocks],,, [#include <sys/quota.h>])

AC_CHECK_DECLS([sys_errlist])
AC_CHECK_DECLS([sys_siglist, _sys_siglist],,, [#include <signal.h>])

AC_PATH_PROG([PERL], [perl])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
