#ifndef _ASM_POWERPC_PROCESSOR_H
#define _ASM_POWERPC_PROCESSOR_H

/*
 * Copyright (C) 2001 PPC 64 Team, IBM Corp
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <asm/reg.h>

#ifndef __ASSEMBLY__
#include <linux/compiler.h>
#include <linux/prefetch.h>
#include <asm/ptrace.h>
#include <asm/types.h>

/* We do _not_ want to define new machine types at all, those must die
 * in favor of using the device-tree
 * -- BenH.
 */

/* PREP sub-platform types see residual.h for these */
#define _PREP_Motorola	0x01	/* motorola prep */
#define _PREP_Firm	0x02	/* firmworks prep */
#define _PREP_IBM	0x00	/* ibm prep */
#define _PREP_Bull	0x03	/* bull prep */

/* CHRP sub-platform types. These are arbitrary */
#define _CHRP_Motorola	0x04	/* motorola chrp, the cobra */
#define _CHRP_IBM	0x05	/* IBM chrp, the longtrail and longtrail 2 */
#define _CHRP_Pegasos	0x06	/* Genesi/bplan's Pegasos and Pegasos2 */
#define _CHRP_briq	0x07	/* TotalImpact's briQ */

#if defined(__KERNEL__) && defined(CONFIG_PPC32)

extern int _chrp_type;

#ifdef CONFIG_PPC_PREP

/* what kind of prep workstation we are */
extern int _prep_type;

/*
 * This is used to identify the board type from a given PReP board
 * vendor. Board revision is also made available. This will be moved
 * elsewhere soon
 */
extern unsigned char ucBoardRev;
extern unsigned char ucBoardRevMaj, ucBoardRevMin;

#endif /* CONFIG_PPC_PREP */

#endif /* defined(__KERNEL__) && defined(CONFIG_PPC32) */

/*
 * Default implementation of macro that returns current
 * instruction pointer ("program counter").
 */
#define current_text_addr() ({ __label__ _l; _l: &&_l;})

/* Macros for adjusting thread priority (hardware multi-threading) */
#define HMT_very_low()   asm volatile("or 31,31,31   # very low priority")
#define HMT_low()	 asm volatile("or 1,1,1	     # low priority")
#define HMT_medium_low() asm volatile("or 6,6,6      # medium low priority")
#define HMT_medium()	 asm volatile("or 2,2,2	     # medium priority")
#define HMT_medium_high() asm volatile("or 5,5,5      # medium high priority")
#define HMT_high()	 asm volatile("or 3,3,3	     # high priority")

#ifdef __KERNEL__

extern int have_of;

struct task_struct;
void start_thread(struct pt_regs *regs, unsigned long fdptr, unsigned long sp);
void release_thread(struct task_struct *);

/* Prepare to copy thread state - unlazy all lazy status */
extern void prepare_to_copy(struct task_struct *tsk);

/* Create a new kernel thread. */
extern long kernel_thread(int (*fn)(void *), void *arg, unsigned long flags);

/* Lazy FPU handling on uni-processor */
extern struct task_struct *last_task_used_math;
extern struct task_struct *last_task_used_altivec;
extern struct task_struct *last_task_used_spe;

#ifdef CONFIG_PPC32
#define TASK_SIZE	(CONFIG_TASK_SIZE)

/* This decides where the kernel will search for a free chunk of vm
 * space during mmap's.
 */
#define TASK_UNMAPPED_BASE	(TASK_SIZE / 8 * 3)
#endif

#ifdef CONFIG_PPC64
/* 64-bit user address space is 44-bits (16TB user VM) */
#define TASK_SIZE_USER64 (0x0000100000000000UL)

/* 
 * 32-bit user address space is 4GB - 1 page 
 * (this 1 page is needed so referencing of 0xFFFFFFFF generates EFAULT
 */
#define TASK_SIZE_USER32 (0x0000000100000000UL - (1*PAGE_SIZE))

#define TASK_SIZE_OF(tsk) (test_tsk_thread_flag(tsk, TIF_32BIT) ? \
		TASK_SIZE_USER32 : TASK_SIZE_USER64)
#define TASK_SIZE	  TASK_SIZE_OF(current)

/* This decides where the kernel will search for a free chunk of vm
 * space during mmap's.
 */
#define TASK_UNMAPPED_BASE_USER32 (PAGE_ALIGN(TASK_SIZE_USER32 / 4))
#define TASK_UNMAPPED_BASE_USER64 (PAGE_ALIGN(TASK_SIZE_USER64 / 4))

#define TASK_UNMAPPED_BASE ((test_thread_flag(TIF_32BIT)) ? \
		TASK_UNMAPPED_BASE_USER32 : TASK_UNMAPPED_BASE_USER64 )
#endif

typedef struct {
	unsigned long seg;
} mm_segment_t;

struct thread_struct {
	unsigned long	ksp;		/* Kernel stack pointer */
#ifdef CONFIG_PPC64
	unsigned long	ksp_vsid;
#endif
	struct pt_regs	*regs;		/* Pointer to saved register state */
	mm_segment_t	fs;		/* for get_fs() validation */
#ifdef CONFIG_PPC32
	void		*pgdir;		/* root of page-table tree */
	signed long	last_syscall;
#endif
#if defined(CONFIG_4xx) || defined (CONFIG_BOOKE)
	unsigned long	dbcr0;		/* debug control register values */
	unsigned long	dbcr1;
#endif
	double		fpr[32];	/* Complete floating point set */
	struct {			/* fpr ... fpscr must be contiguous */

		unsigned int pad;
		unsigned int val;	/* Floating point status */
	} fpscr;
	int		fpexc_mode;	/* floating-point exception mode */
	unsigned int	align_ctl;	/* alignment handling control */
#ifdef CONFIG_PPC64
	unsigned long	start_tb;	/* Start purr when proc switched in */
	unsigned long	accum_tb;	/* Total accumilated purr for process */
#endif
	unsigned long	dabr;		/* Data address breakpoint register */
#ifdef CONFIG_ALTIVEC
	/* Complete AltiVec register set */
	vector128	vr[32] __attribute((aligned(16)));
	/* AltiVec status */
	vector128	vscr __attribute((aligned(16)));
	unsigned long	vrsave;
	int		used_vr;	/* set if process has used altivec */
#endif /* CONFIG_ALTIVEC */
#ifdef CONFIG_SPE
	unsigned long	evr[32];	/* upper 32-bits of SPE regs */
	u64		acc;		/* Accumulator */
	unsigned long	spefscr;	/* SPE & eFP status */
	int		used_spe;	/* set if process has used spe */
#endif /* CONFIG_SPE */
};

#define ARCH_MIN_TASKALIGN 16

#define INIT_SP		(sizeof(init_stack) + (unsigned long) &init_stack)


#ifdef CONFIG_PPC32
#define INIT_THREAD { \
	.ksp = INIT_SP, \
	.fs = KERNEL_DS, \
	.pgdir = swapper_pg_dir, \
	.fpexc_mode = MSR_FE0 | MSR_FE1, \
}
#else
#define INIT_THREAD  { \
	.ksp = INIT_SP, \
	.regs = (struct pt_regs *)INIT_SP - 1, /* XXX bogus, I think */ \
	.fs = KERNEL_DS, \
	.fpr = {0}, \
	.fpscr = { .val = 0, }, \
	.fpexc_mode = 0, \
}
#endif

/*
 * Return saved PC of a blocked thread. For now, this is the "user" PC
 */
#define thread_saved_pc(tsk)    \
        ((tsk)->thread.regs? (tsk)->thread.regs->nip: 0)

unsigned long get_wchan(struct task_struct *p);

#define KSTK_EIP(tsk)  ((tsk)->thread.regs? (tsk)->thread.regs->nip: 0)
#define KSTK_ESP(tsk)  ((tsk)->thread.regs? (tsk)->thread.regs->gpr[1]: 0)

/* Get/set floating-point exception mode */
#define GET_FPEXC_CTL(tsk, adr) get_fpexc_mode((tsk), (adr))
#define SET_FPEXC_CTL(tsk, val) set_fpexc_mode((tsk), (val))

extern int get_fpexc_mode(struct task_struct *tsk, unsigned long adr);
extern int set_fpexc_mode(struct task_struct *tsk, unsigned int val);

#define GET_ENDIAN(tsk, adr) get_endian((tsk), (adr))
#define SET_ENDIAN(tsk, val) set_endian((tsk), (val))

extern int get_endian(struct task_struct *tsk, unsigned long adr);
extern int set_endian(struct task_struct *tsk, unsigned int val);

#define GET_UNALIGN_CTL(tsk, adr)	get_unalign_ctl((tsk), (adr))
#define SET_UNALIGN_CTL(tsk, val)	set_unalign_ctl((tsk), (val))

extern int get_unalign_ctl(struct task_struct *tsk, unsigned long adr);
extern int set_unalign_ctl(struct task_struct *tsk, unsigned int val);

static inline unsigned int __unpack_fe01(unsigned long msr_bits)
{
	return ((msr_bits & MSR_FE0) >> 10) | ((msr_bits & MSR_FE1) >> 8);
}

static inline unsigned long __pack_fe01(unsigned int fpmode)
{
	return ((fpmode << 10) & MSR_FE0) | ((fpmode << 8) & MSR_FE1);
}

#ifdef CONFIG_PPC64
#define cpu_relax()	do { HMT_low(); HMT_medium(); barrier(); } while (0)
#else
#define cpu_relax()	barrier()
#endif

/* Check that a certain kernel stack pointer is valid in task_struct p */
int validate_sp(unsigned long sp, struct task_struct *p,
                       unsigned long nbytes);

/*
 * Prefetch macros.
 */
#define ARCH_HAS_PREFETCH
#define ARCH_HAS_PREFETCHW
#define ARCH_HAS_SPINLOCK_PREFETCH

static inline void prefetch(const void *x)
{
	if (unlikely(!x))
		return;

	__asm__ __volatile__ ("dcbt 0,%0" : : "r" (x));
}

static inline void prefetchw(const void *x)
{
	if (unlikely(!x))
		return;

	__asm__ __volatile__ ("dcbtst 0,%0" : : "r" (x));
}

#define spin_lock_prefetch(x)	prefetchw(x)

#ifdef CONFIG_PPC64
#define HAVE_ARCH_PICK_MMAP_LAYOUT
#endif

#endif /* __KERNEL__ */
#endif /* __ASSEMBLY__ */
#endif /* _ASM_POWERPC_PROCESSOR_H */
