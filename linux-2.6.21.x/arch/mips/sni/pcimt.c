/*
 * PCIMT specific code
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1996, 97, 98, 2000, 03, 04, 06 Ralf Baechle (ralf@linux-mips.org)
 * Copyright (C) 2006 Thomas Bogendoerfer (tsbogend@alpha.franken.de)
 */

#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/pci.h>
#include <linux/serial_8250.h>

#include <asm/mc146818-time.h>
#include <asm/sni.h>
#include <asm/time.h>
#include <asm/i8259.h>
#include <asm/irq_cpu.h>

#define cacheconf (*(volatile unsigned int *)PCIMT_CACHECONF)
#define invspace (*(volatile unsigned int *)PCIMT_INVSPACE)

static void __init sni_pcimt_sc_init(void)
{
	unsigned int scsiz, sc_size;

	scsiz = cacheconf & 7;
	if (scsiz == 0) {
		printk("Second level cache is deactived.\n");
		return;
	}
	if (scsiz >= 6) {
		printk("Invalid second level cache size configured, "
		       "deactivating second level cache.\n");
		cacheconf = 0;
		return;
	}

	sc_size = 128 << scsiz;
	printk("%dkb second level cache detected, deactivating.\n", sc_size);
	cacheconf = 0;
}


/*
 * A bit more gossip about the iron we're running on ...
 */
static inline void sni_pcimt_detect(void)
{
	char boardtype[80];
	unsigned char csmsr;
	char *p = boardtype;
	unsigned int asic;

	csmsr = *(volatile unsigned char *)PCIMT_CSMSR;

	p += sprintf(p, "%s PCI", (csmsr & 0x80) ? "RM200" : "RM300");
	if ((csmsr & 0x80) == 0)
		p += sprintf(p, ", board revision %s",
		             (csmsr & 0x20) ? "D" : "C");
	asic = csmsr & 0x80;
	asic = (csmsr & 0x08) ? asic : !asic;
	p += sprintf(p, ", ASIC PCI Rev %s", asic ? "1.0" : "1.1");
	printk("%s.\n", boardtype);
}

#define PORT(_base,_irq)				\
	{						\
		.iobase		= _base,		\
		.irq		= _irq,			\
		.uartclk	= 1843200,		\
		.iotype		= UPIO_PORT,		\
		.flags		= UPF_BOOT_AUTOCONF,	\
	}

static struct plat_serial8250_port pcimt_data[] = {
	PORT(0x3f8, 4),
	PORT(0x2f8, 3),
	{ },
};

static struct platform_device pcimt_serial8250_device = {
	.name			= "serial8250",
	.id			= PLAT8250_DEV_PLATFORM,
	.dev			= {
		.platform_data	= pcimt_data,
	},
};

static struct resource sni_io_resource = {
	.start	= 0x00001000UL,
	.end	= 0x03bfffffUL,
	.name	= "PCIMT IO MEM",
	.flags	= IORESOURCE_IO,
};

static struct resource pcimt_io_resources[] = {
	{
		.start	= 0x00,
		.end	= 0x1f,
		.name	= "dma1",
		.flags	= IORESOURCE_BUSY
	}, {
		.start	=  0x40,
		.end	= 0x5f,
		.name	= "timer",
		.flags	= IORESOURCE_BUSY
	}, {
		.start	=  0x60,
		.end	= 0x6f,
		.name	= "keyboard",
		.flags	= IORESOURCE_BUSY
	}, {
		.start	=  0x80,
		.end	= 0x8f,
		.name	= "dma page reg",
		.flags	= IORESOURCE_BUSY
	}, {
		.start	=  0xc0,
		.end	= 0xdf,
		.name	= "dma2",
		.flags	= IORESOURCE_BUSY
	}, {
		.start	=  0xcfc,
		.end	= 0xcff,
		.name	= "PCI config data",
		.flags	= IORESOURCE_BUSY
	}
};

static struct resource sni_mem_resource = {
	.start	= 0x10000000UL,
	.end	= 0xffffffffUL,
	.name	= "PCIMT PCI MEM",
	.flags	= IORESOURCE_MEM
};

/*
 * The RM200/RM300 has a few holes in it's PCI/EISA memory address space used
 * for other purposes.  Be paranoid and allocate all of the before the PCI
 * code gets a chance to to map anything else there ...
 *
 * This leaves the following areas available:
 *
 * 0x10000000 - 0x1009ffff (640kB) PCI/EISA/ISA Bus Memory
 * 0x10100000 - 0x13ffffff ( 15MB) PCI/EISA/ISA Bus Memory
 * 0x18000000 - 0x1fbfffff (124MB) PCI/EISA Bus Memory
 * 0x1ff08000 - 0x1ffeffff (816kB) PCI/EISA Bus Memory
 * 0xa0000000 - 0xffffffff (1.5GB) PCI/EISA Bus Memory
 */
static struct resource pcimt_mem_resources[] = {
	{
		.start	= 0x100a0000,
		.end	= 0x100bffff,
		.name	= "Video RAM area",
		.flags	= IORESOURCE_BUSY
	}, {
		.start	= 0x100c0000,
		.end	= 0x100fffff,
		.name	= "ISA Reserved",
		.flags	= IORESOURCE_BUSY
	}, {
		.start	= 0x14000000,
		.end	= 0x17bfffff,
		.name	= "PCI IO",
		.flags	= IORESOURCE_BUSY
	}, {
		.start	= 0x17c00000,
		.end	= 0x17ffffff,
		.name	= "Cache Replacement Area",
		.flags	= IORESOURCE_BUSY
	}, {
		.start	= 0x1a000000,
		.end	= 0x1a000003,
		.name	= "PCI INT Acknowledge",
		.flags	= IORESOURCE_BUSY
	}, {
		.start	= 0x1fc00000,
		.end	= 0x1fc7ffff,
		.name	= "Boot PROM",
		.flags	= IORESOURCE_BUSY
	}, {
		.start	= 0x1fc80000,
		.end	= 0x1fcfffff,
		.name	= "Diag PROM",
		.flags	= IORESOURCE_BUSY
	}, {
		.start	= 0x1fd00000,
		.end	= 0x1fdfffff,
		.name	= "X-Bus",
		.flags	= IORESOURCE_BUSY
	}, {
		.start	= 0x1fe00000,
		.end	= 0x1fefffff,
		.name	= "BIOS map",
		.flags	= IORESOURCE_BUSY
	}, {
		.start	= 0x1ff00000,
		.end	= 0x1ff7ffff,
		.name	= "NVRAM / EEPROM",
		.flags	= IORESOURCE_BUSY
	}, {
		.start	= 0x1fff0000,
		.end	= 0x1fffefff,
		.name	= "ASIC PCI",
		.flags	= IORESOURCE_BUSY
	}, {
		.start	= 0x1ffff000,
		.end	= 0x1fffffff,
		.name	= "MP Agent",
		.flags	= IORESOURCE_BUSY
	}, {
		.start	= 0x20000000,
		.end	= 0x9fffffff,
		.name	= "Main Memory",
		.flags	= IORESOURCE_BUSY
	}
};

static void __init sni_pcimt_resource_init(void)
{
	int i;

	/* request I/O space for devices used on all i[345]86 PCs */
	for (i = 0; i < ARRAY_SIZE(pcimt_io_resources); i++)
		request_resource(&ioport_resource, pcimt_io_resources + i);

	/* request mem space for pcimt-specific devices */
	for (i = 0; i < ARRAY_SIZE(pcimt_mem_resources); i++)
		request_resource(&sni_mem_resource, pcimt_mem_resources + i);

	ioport_resource.end = sni_io_resource.end;
}

extern struct pci_ops sni_pcimt_ops;

static struct pci_controller sni_controller = {
	.pci_ops	= &sni_pcimt_ops,
	.mem_resource	= &sni_mem_resource,
	.mem_offset	= 0x10000000UL,
	.io_resource	= &sni_io_resource,
	.io_offset	= 0x00000000UL
};

static void enable_pcimt_irq(unsigned int irq)
{
	unsigned int mask = 1 << (irq - PCIMT_IRQ_INT2);

	*(volatile u8 *) PCIMT_IRQSEL |= mask;
}

void disable_pcimt_irq(unsigned int irq)
{
	unsigned int mask = ~(1 << (irq - PCIMT_IRQ_INT2));

	*(volatile u8 *) PCIMT_IRQSEL &= mask;
}

static void end_pcimt_irq(unsigned int irq)
{
	if (!(irq_desc[irq].status & (IRQ_DISABLED|IRQ_INPROGRESS)))
		enable_pcimt_irq(irq);
}

static struct irq_chip pcimt_irq_type = {
	.typename = "PCIMT",
	.ack = disable_pcimt_irq,
	.mask = disable_pcimt_irq,
	.mask_ack = disable_pcimt_irq,
	.unmask = enable_pcimt_irq,
	.end = end_pcimt_irq,
};

/*
 * hwint0 should deal with MP agent, ASIC PCI, EISA NMI and debug
 * button interrupts.  Later ...
 */
static void pcimt_hwint0(void)
{
	panic("Received int0 but no handler yet ...");
}

/*
 * hwint 1 deals with EISA and SCSI interrupts,
 *
 * The EISA_INT bit in CSITPEND is high active, all others are low active.
 */
static void pcimt_hwint1(void)
{
	u8 pend = *(volatile char *)PCIMT_CSITPEND;
	unsigned long flags;

	if (pend & IT_EISA) {
		int irq;
		/*
		 * Note: ASIC PCI's builtin interrupt achknowledge feature is
		 * broken.  Using it may result in loss of some or all i8259
		 * interupts, so don't use PCIMT_INT_ACKNOWLEDGE ...
		 */
		irq = i8259_irq();
		if (unlikely(irq < 0))
			return;

		do_IRQ(irq);
	}

	if (!(pend & IT_SCSI)) {
		flags = read_c0_status();
		clear_c0_status(ST0_IM);
		do_IRQ(PCIMT_IRQ_SCSI);
		write_c0_status(flags);
	}
}

/*
 * hwint 3 should deal with the PCI A - D interrupts,
 */
static void pcimt_hwint3(void)
{
	u8 pend = *(volatile char *)PCIMT_CSITPEND;
	int irq;

	pend &= (IT_INTA | IT_INTB | IT_INTC | IT_INTD);
	pend ^= (IT_INTA | IT_INTB | IT_INTC | IT_INTD);
	clear_c0_status(IE_IRQ3);
	irq = PCIMT_IRQ_INT2 + ffs(pend) - 1;
	do_IRQ(irq);
	set_c0_status(IE_IRQ3);
}

static void sni_pcimt_hwint(void)
{
	u32 pending = read_c0_cause() & read_c0_status();

	if (pending & C_IRQ5)
		do_IRQ(MIPS_CPU_IRQ_BASE + 7);
	else if (pending & C_IRQ4)
		do_IRQ(MIPS_CPU_IRQ_BASE + 6);
	else if (pending & C_IRQ3)
		pcimt_hwint3();
	else if (pending & C_IRQ1)
		pcimt_hwint1();
	else if (pending & C_IRQ0) {
		pcimt_hwint0();
	}
}

void __init sni_pcimt_irq_init(void)
{
	int i;

	*(volatile u8 *) PCIMT_IRQSEL = IT_ETH | IT_EISA;
	mips_cpu_irq_init();
	/* Actually we've got more interrupts to handle ...  */
	for (i = PCIMT_IRQ_INT2; i <= PCIMT_IRQ_SCSI; i++)
		set_irq_chip(i, &pcimt_irq_type);
	sni_hwint = sni_pcimt_hwint;
	change_c0_status(ST0_IM, IE_IRQ1|IE_IRQ3);
}

void sni_pcimt_init(void)
{
	sni_pcimt_resource_init();
	sni_pcimt_detect();
	sni_pcimt_sc_init();
	rtc_mips_get_time = mc146818_get_cmos_time;
	rtc_mips_set_time = mc146818_set_rtc_mmss;
	board_time_init = sni_cpu_time_init;
#ifdef CONFIG_PCI
	register_pci_controller(&sni_controller);
#endif
}

static int __init snirm_pcimt_setup_devinit(void)
{
	switch (sni_brd_type) {
	case SNI_BRD_PCI_MTOWER:
	case SNI_BRD_PCI_DESKTOP:
	case SNI_BRD_PCI_MTOWER_CPLUS:
	        platform_device_register(&pcimt_serial8250_device);
	        break;
	}

	return 0;
}

device_initcall(snirm_pcimt_setup_devinit);
