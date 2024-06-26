/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2009, 2010 Cavium Networks, Inc.
 */


#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/binfmts.h>
#include <linux/elf.h>
#include <linux/vmalloc.h>
#include <linux/unistd.h>

#include <asm/vdso.h>
#include <asm/uasm.h>

/*
 * Including <asm/unistd.h> would give use the 64-bit syscall numbers ...
 */
#define __NR_O32_sigreturn		4119
#define __NR_O32_rt_sigreturn		4193
#define __NR_N32_rt_sigreturn		6211

static struct page *vdso_page;

static unsigned long vdso_addr(unsigned long start)
{
	return STACK_TOP;
}

int arch_setup_additional_pages(struct linux_binprm *bprm, int uses_interp)
{
	int ret;
	unsigned long addr;
	struct mm_struct *mm = current->mm;

	down_write(&mm->mmap_sem);

	addr = vdso_addr(mm->start_stack);

	addr = get_unmapped_area(NULL, addr, PAGE_SIZE, 0, 0);
	if (IS_ERR_VALUE(addr)) {
		ret = addr;
		goto up_fail;
	}

	ret = install_special_mapping(mm, addr, PAGE_SIZE,
				      VM_READ|VM_EXEC|
				      VM_MAYREAD|VM_MAYWRITE|VM_MAYEXEC|
				      VM_ALWAYSDUMP,
				      &vdso_page);

	if (ret)
		goto up_fail;

	mm->context.vdso = (void *)addr;

up_fail:
	up_write(&mm->mmap_sem);
	return ret;
}

const char *arch_vma_name(struct vm_area_struct *vma)
{
	if (vma->vm_mm && vma->vm_start == (long)vma->vm_mm->context.vdso)
		return "[vdso]";
	return NULL;
}
