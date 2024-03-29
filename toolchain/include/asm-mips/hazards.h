/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2003, 2004 Ralf Baechle <ralf@linux-mips.org>
 * Copyright (C) MIPS Technologies, Inc.
 *   written by Ralf Baechle <ralf@linux-mips.org>
 */
#ifndef _ASM_HAZARDS_H
#define _ASM_HAZARDS_H

#ifdef __ASSEMBLY__
#define ASMMACRO(name, code...) .macro name; code; .endm
#else

#include <asm/cpu-features.h>

#define ASMMACRO(name, code...)						\
__asm__(".macro " #name "; " #code "; .endm");				\
									\
static inline void name(void)						\
{									\
	__asm__ __volatile__ (#name);					\
}

#endif

ASMMACRO(_ssnop,
	 sll	$0, $0, 1
	)

ASMMACRO(_ehb,
	 sll	$0, $0, 3
	)

/*
 * TLB hazards
 */
#if defined(CONFIG_CPU_MIPSR2)

/*
 * MIPSR2 defines ehb for hazard avoidance
 */

ASMMACRO(mtc0_tlbw_hazard,
	 _ehb
	)
ASMMACRO(tlbw_use_hazard,
	 _ehb
	)
ASMMACRO(tlb_probe_hazard,
	 _ehb
	)
ASMMACRO(irq_enable_hazard,
	 _ehb
	)
ASMMACRO(irq_disable_hazard,
	_ehb
	)
ASMMACRO(back_to_back_c0_hazard,
	 _ehb
	)
/*
 * gcc has a tradition of misscompiling the previous construct using the
 * address of a label as argument to inline assembler.  Gas otoh has the
 * annoying difference between la and dla which are only usable for 32-bit
 * rsp. 64-bit code, so can't be used without conditional compilation.
 * The alterantive is switching the assembler to 64-bit code which happens
 * to work right even for 32-bit code ...
 */
#define instruction_hazard()						\
do {									\
	unsigned long tmp;						\
									\
	__asm__ __volatile__(						\
	"	.set	mips64r2				\n"	\
	"	dla	%0, 1f					\n"	\
	"	jr.hb	%0					\n"	\
	"	.set	mips0					\n"	\
	"1:							\n"	\
	: "=r" (tmp));							\
} while (0)

#elif defined(CONFIG_CPU_MIPSR1)

/*
 * These are slightly complicated by the fact that we guarantee R1 kernels to
 * run fine on R2 processors.
 */
ASMMACRO(mtc0_tlbw_hazard,
	_ssnop; _ssnop; _ehb
	)
ASMMACRO(tlbw_use_hazard,
	_ssnop; _ssnop; _ssnop; _ehb
	)
ASMMACRO(tlb_probe_hazard,
	 _ssnop; _ssnop; _ssnop; _ehb
	)
ASMMACRO(irq_enable_hazard,
	 _ssnop; _ssnop; _ssnop; _ehb
	)
ASMMACRO(irq_disable_hazard,
	_ssnop; _ssnop; _ssnop; _ehb
	)
ASMMACRO(back_to_back_c0_hazard,
	 _ssnop; _ssnop; _ssnop; _ehb
	)
/*
 * gcc has a tradition of misscompiling the previous construct using the
 * address of a label as argument to inline assembler.  Gas otoh has the
 * annoying difference between la and dla which are only usable for 32-bit
 * rsp. 64-bit code, so can't be used without conditional compilation.
 * The alterantive is switching the assembler to 64-bit code which happens
 * to work right even for 32-bit code ...
 */
#define __instruction_hazard()						\
do {									\
	unsigned long tmp;						\
									\
	__asm__ __volatile__(						\
	"	.set	mips64r2				\n"	\
	"	dla	%0, 1f					\n"	\
	"	jr.hb	%0					\n"	\
	"	.set	mips0					\n"	\
	"1:							\n"	\
	: "=r" (tmp));							\
} while (0)

#define instruction_hazard()						\
do {									\
	if (cpu_has_mips_r2)						\
		__instruction_hazard();					\
} while (0)

#elif defined(CONFIG_CPU_R10000)

/*
 * R10000 rocks - all hazards handled in hardware, so this becomes a nobrainer.
 */

ASMMACRO(mtc0_tlbw_hazard,
	)
ASMMACRO(tlbw_use_hazard,
	)
ASMMACRO(tlb_probe_hazard,
	)
ASMMACRO(irq_enable_hazard,
	)
ASMMACRO(irq_disable_hazard,
	)
ASMMACRO(back_to_back_c0_hazard,
	)
#define instruction_hazard() do { } while (0)

#elif defined(CONFIG_CPU_RM9000)

/*
 * RM9000 hazards.  When the JTLB is updated by tlbwi or tlbwr, a subsequent
 * use of the JTLB for instructions should not occur for 4 cpu cycles and use
 * for data translations should not occur for 3 cpu cycles.
 */

ASMMACRO(mtc0_tlbw_hazard,
	 _ssnop; _ssnop; _ssnop; _ssnop
	)
ASMMACRO(tlbw_use_hazard,
	 _ssnop; _ssnop; _ssnop; _ssnop
	)
ASMMACRO(tlb_probe_hazard,
	 _ssnop; _ssnop; _ssnop; _ssnop
	)
ASMMACRO(irq_enable_hazard,
	)
ASMMACRO(irq_disable_hazard,
	)
ASMMACRO(back_to_back_c0_hazard,
	)
#define instruction_hazard() do { } while (0)

#elif defined(CONFIG_CPU_SB1)

/*
 * Mostly like R4000 for historic reasons
 */
ASMMACRO(mtc0_tlbw_hazard,
	)
ASMMACRO(tlbw_use_hazard,
	)
ASMMACRO(tlb_probe_hazard,
	)
ASMMACRO(irq_enable_hazard,
	)
ASMMACRO(irq_disable_hazard,
	 _ssnop; _ssnop; _ssnop
	)
ASMMACRO(back_to_back_c0_hazard,
	)
#define instruction_hazard() do { } while (0)

#else

/*
 * Finally the catchall case for all other processors including R4000, R4400,
 * R4600, R4700, R5000, RM7000, NEC VR41xx etc.
 *
 * The taken branch will result in a two cycle penalty for the two killed
 * instructions on R4000 / R4400.  Other processors only have a single cycle
 * hazard so this is nice trick to have an optimal code for a range of
 * processors.
 * Make it compatible with Mips32r2 processors
 */
ASMMACRO(mtc0_tlbw_hazard,
	nop; nop; _ehb
	)
ASMMACRO(tlbw_use_hazard,
	nop; nop; nop; _ehb
	)
ASMMACRO(tlb_probe_hazard,
	nop; nop; nop; _ehb
	)
ASMMACRO(irq_enable_hazard,
	_ehb
	)
ASMMACRO(irq_disable_hazard,
	nop; nop; nop; _ehb
	)
ASMMACRO(back_to_back_c0_hazard,
	_ssnop; _ssnop; _ssnop; _ehb
	)
#define instruction_hazard() do { } while (0)

#endif

#endif /* _ASM_HAZARDS_H */
