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
/* Define how to access the int that the wait system call stores.
   This has been compatible in all Unix systems since time immemorial,
   but various well-meaning people have defined various different
   words for the same old bits in the same old int (sometimes claimed
   to be a struct).  We just know it's an int and we use these macros
   to access the bits.  */

/* The following macros are defined equivalently to their definitions
   in POSIX.1.  We fail to define WNOHANG and WUNTRACED, which POSIX.1
   <sys/wait.h> defines, since our code does not use waitpid().  We
   also fail to declare wait() and waitpid().  */   

#ifndef	WIFEXITED
#define WIFEXITED(w)	(((w)&0377) == 0)
#endif

#ifndef	WIFSIGNALED
#define WIFSIGNALED(w)	(((w)&0377) != 0177 && ((w)&~0377) == 0)
#endif

#ifndef	WIFSTOPPED
#ifdef IBM6000

/* Unfortunately, the above comment (about being compatible in all Unix 
   systems) is not quite correct for AIX, sigh.  And AIX 3.2 can generate
   status words like 0x57c (sigtrap received after load), and gdb would
   choke on it. */

#define WIFSTOPPED(w)	((w)&0x40)

#else
#define WIFSTOPPED(w)	(((w)&0377) == 0177)
#endif
#endif

#ifndef	WEXITSTATUS
#define WEXITSTATUS(w)	(((w) >> 8) & 0377) /* same as WRETCODE */
#endif

#ifndef	WTERMSIG
#define WTERMSIG(w)	((w) & 0177)
#endif

#ifndef	WSTOPSIG
#define WSTOPSIG	WEXITSTATUS
#endif

/* These are not defined in POSIX, but are used by our programs.  */

#define WAITTYPE	int

#ifndef	WCOREDUMP
#define WCOREDUMP(w)	(((w)&0200) != 0)
#endif

#ifndef	WSETEXIT
#define WSETEXIT(w,status) ((w) = (0 | ((status) << 8)))
#endif

#ifndef	WSETSTOP
#define WSETSTOP(w,sig)	   ((w) = (0177 | ((sig) << 8)))
#endif

