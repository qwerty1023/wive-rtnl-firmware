#ifndef _S390_KDEBUG_H
#define _S390_KDEBUG_H

/*
 * Feb 2006 Ported to s390 <grundym@us.ibm.com>
 */
#include <linux/notifier.h>

struct pt_regs;

struct die_args {
	struct pt_regs *regs;
	const char *str;
	long err;
	int trapnr;
	int signr;
};

/* Note - you should never unregister because that can race with NMIs.
 * If you really want to do it first unregister - then synchronize_sched
 *  - then free.
 */
extern int register_die_notifier(struct notifier_block *);
extern int unregister_die_notifier(struct notifier_block *);
extern int register_page_fault_notifier(struct notifier_block *);
extern int unregister_page_fault_notifier(struct notifier_block *);
extern struct atomic_notifier_head s390die_chain;

enum die_val {
	DIE_OOPS = 1,
	DIE_BPT,
	DIE_SSTEP,
	DIE_PANIC,
	DIE_NMI,
	DIE_DIE,
	DIE_NMIWATCHDOG,
	DIE_KERNELDEBUG,
	DIE_TRAP,
	DIE_GPF,
	DIE_CALL,
	DIE_NMI_IPI,
	DIE_PAGE_FAULT,
};

extern void die(const char *, struct pt_regs *, long);

#endif
