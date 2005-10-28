/* Determine whether a DIE covers a PC address.
   Copyright (C) 2005 Red Hat, Inc.

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

#include "libdwP.h"
#include <dwarf.h>


int
dwarf_haspc (Dwarf_Die *die, Dwarf_Addr pc)
{
  if (die == NULL)
    return -1;

  Dwarf_Addr base;
  Dwarf_Addr begin;
  Dwarf_Addr end;
  ptrdiff_t offset = 0;
  while ((offset = INTUSE(dwarf_ranges) (die, offset, &base,
					 &begin, &end)) > 0)
    if (pc >= begin && pc < end)
      return 1;

  return offset;
}
INTDEF (dwarf_haspc)
