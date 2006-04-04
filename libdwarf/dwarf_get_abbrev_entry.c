/* Get attribute from abbreviation record.
   Copyright (C) 2001, 2002 Red Hat, Inc.
   This file is part of Red Hat elfutils.
   Written by Ulrich Drepper <drepper@redhat.com>, 2001.

   Red Hat elfutils is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by the
   Free Software Foundation; version 2 of the License.

   Red Hat elfutils is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with Red Hat elfutils; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301 USA.

   Red Hat elfutils is an included package of the Open Invention Network.
   An included package of the Open Invention Network is a package for which
   Open Invention Network licensees cross-license their patents.  No patent
   license is granted, either expressly or impliedly, by designation as an
   included package.  Should you wish to participate in the Open Invention
   Network licensing program, please visit www.openinventionnetwork.com
   <http://www.openinventionnetwork.com>.  */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <dwarf.h>
#include <stdlib.h>

#include <libdwarfP.h>


int
dwarf_get_abbrev_entry (abbrev, idx, attr_num, form, offset, error)
     Dwarf_Abbrev abbrev;
     Dwarf_Signed idx;
     Dwarf_Half *attr_num;
     Dwarf_Signed *form;
     Dwarf_Off *offset;
     Dwarf_Error *error;
{
  Dwarf_Small *attrp;
  Dwarf_Small *start_attrp;
  Dwarf_Word attr_name;
  Dwarf_Word attr_form;

  if (idx < 0)
    return DW_DLV_NO_ENTRY;

  /* Address in memory.  */
  attrp = abbrev->attrp;

  /* Read the attributes, skip over the ones we don't want.  */
  do
    {
      start_attrp = attrp;

      get_uleb128 (attr_name, attrp);
      get_uleb128 (attr_form, attrp);

      if (attr_name == 0 || attr_form == 0)
	return DW_DLV_NO_ENTRY;
    }
  while (idx-- > 0);

  *attr_num = attr_name;
  *form = attr_form;
  *offset = (start_attrp - abbrev->attrp) + abbrev->offset;

  return DW_DLV_OK;
}
