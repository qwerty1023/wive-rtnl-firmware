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
/* V850 ELF support for BFD.
   Copyright (C) 1997 Free Software Foundation, Inc.
   Created by Michael Meissner, Cygnus Support <meissner@cygnus.com>

This file is part of BFD, the Binary File Descriptor library.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

/* This file holds definitions specific to the MIPS ELF ABI.  Note
   that most of this is not actually implemented by BFD.  */

#ifndef _ELF_V850_H
#define _ELF_V850_H

/* Processor specific flags for the ELF header e_flags field.  */

/* Four bit V850 architecture field.  */
#define EF_V850_ARCH		0xf0000000

/* v850 code.  */
#define E_V850_ARCH		0x00000000

/* v850e code.  */
#define E_V850E_ARCH		0x10000000

/* v850ea code.  */
#define E_V850EA_ARCH		0x20000000


/* Flags for the st_other field */
#define V850_OTHER_SDA		0x01	/* symbol had SDA relocations */
#define V850_OTHER_ZDA		0x02	/* symbol had ZDA relocations */
#define V850_OTHER_TDA		0x04	/* symbol had TDA relocations */
#define V850_OTHER_TDA_BYTE	0x08	/* symbol had TDA byte relocations */
#define V850_OTHER_ERROR	0x80	/* symbol had an error reported */

/* V850 relocations */
#include "elf/reloc-macros.h"

START_RELOC_NUMBERS (v850_reloc_type)
     RELOC_NUMBER (R_V850_NONE, 0)
     RELOC_NUMBER (R_V850_9_PCREL, 1)
     RELOC_NUMBER (R_V850_22_PCREL, 2)
     RELOC_NUMBER (R_V850_HI16_S, 3)
     RELOC_NUMBER (R_V850_HI16, 4)
     RELOC_NUMBER (R_V850_LO16, 5)
     RELOC_NUMBER (R_V850_32, 6)
     RELOC_NUMBER (R_V850_16, 7)
     RELOC_NUMBER (R_V850_8, 8)
     RELOC_NUMBER( R_V850_SDA_16_16_OFFSET, 9)		/* For ld.b, st.b, set1, clr1, not1, tst1, movea, movhi */
     RELOC_NUMBER( R_V850_SDA_15_16_OFFSET, 10)		/* For ld.w, ld.h, ld.hu, st.w, st.h */
     RELOC_NUMBER( R_V850_ZDA_16_16_OFFSET, 11)		/* For ld.b, st.b, set1, clr1, not1, tst1, movea, movhi */
     RELOC_NUMBER( R_V850_ZDA_15_16_OFFSET, 12)		/* For ld.w, ld.h, ld.hu, st.w, st.h */
     RELOC_NUMBER( R_V850_TDA_6_8_OFFSET, 13)		/* For sst.w, sld.w */
     RELOC_NUMBER( R_V850_TDA_7_8_OFFSET, 14)		/* For sst.h, sld.h */
     RELOC_NUMBER( R_V850_TDA_7_7_OFFSET, 15)		/* For sst.b, sld.b */
     RELOC_NUMBER( R_V850_TDA_16_16_OFFSET, 16)		/* For set1, clr1, not1, tst1, movea, movhi */
/* CYGNUS LOCAL v850e */
     RELOC_NUMBER( R_V850_TDA_4_5_OFFSET, 17)		/* For sld.hu */
     RELOC_NUMBER( R_V850_TDA_4_4_OFFSET, 18)		/* For sld.bu */
     RELOC_NUMBER( R_V850_SDA_16_16_SPLIT_OFFSET, 19)	/* For ld.bu */
     RELOC_NUMBER( R_V850_ZDA_16_16_SPLIT_OFFSET, 20)	/* For ld.bu */
     RELOC_NUMBER( R_V850_CALLT_6_7_OFFSET, 21)		/* For callt */
     RELOC_NUMBER( R_V850_CALLT_16_16_OFFSET, 22)	/* For callt */
/* END CYGNUS LOCAL */
     RELOC_NUMBER (R_V850_GNU_VTINHERIT, 23)
     RELOC_NUMBER (R_V850_GNU_VTENTRY, 24)

     EMPTY_RELOC (R_V850_max)
END_RELOC_NUMBERS


/* Processor specific section indices.  These sections do not actually
   exist.  Symbols with a st_shndx field corresponding to one of these
   values have a special meaning.  */

/* Small data area common symbol.  */
#define SHN_V850_SCOMMON	0xff00

/* Tiny data area common symbol.  */
#define SHN_V850_TCOMMON	0xff01

/* Zero data area common symbol.  */
#define SHN_V850_ZCOMMON	0xff02


/* Processor specific section types.  */

/* Section contains the .scommon data.  */
#define SHT_V850_SCOMMON	0x70000000

/* Section contains the .scommon data.  */
#define SHT_V850_TCOMMON	0x70000001

/* Section contains the .scommon data.  */
#define SHT_V850_ZCOMMON	0x70000002


#endif /* _ELF_V850_H */
