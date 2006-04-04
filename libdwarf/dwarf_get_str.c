/* Return string from debug string section.
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

#include <string.h>

#include "libdwarfP.h"


int
dwarf_get_str (dbg, offset, string, returned_str_len, error)
     Dwarf_Debug dbg;
     Dwarf_Off offset;
     char **string;
     Dwarf_Signed *returned_str_len;
     Dwarf_Error *error;
{
  *string = (char *) dbg->sections[IDX_debug_str].addr;
  if (*string == NULL)
    return DW_DLV_NO_ENTRY;

  if (unlikely (offset >= dbg->sections[IDX_debug_str].size))
    return DW_DLV_NO_ENTRY;

  *string += offset;
  *returned_str_len = strlen (*string);

  return DW_DLV_OK;
}
