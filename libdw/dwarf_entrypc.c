/* Return entry PC attribute of DIE.
   Copyright (C) 2003, 2005 Red Hat, Inc.

   This program is Open Source software; you can redistribute it and/or
   modify it under the terms of the Open Software License version 1.0 as
   published by the Open Source Initiative.

   You should have received a copy of the Open Software License along
   with this program; if not, you may obtain a copy of the Open Software
   License version 1.0 from http://www.opensource.org/licenses/osl.php or
   by writing the Open Source Initiative c/o Lawrence Rosen, Esq.,
   3001 King Ranch Road, Ukiah, CA 95482.   */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <dwarf.h>
#include "libdwP.h"


int
dwarf_entrypc (die, return_addr)
     Dwarf_Die *die;
     Dwarf_Addr *return_addr;
{
  Dwarf_Attribute attr_mem;

  return INTUSE(dwarf_formaddr) (INTUSE(dwarf_attr) (die, DW_AT_entry_pc,
						     &attr_mem)
				 ?: INTUSE(dwarf_attr) (die, DW_AT_low_pc,
							&attr_mem),
				 return_addr);
}
INTDEF(dwarf_entrypc)
