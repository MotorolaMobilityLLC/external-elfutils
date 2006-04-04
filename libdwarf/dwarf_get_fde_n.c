/* Get nth frame descriptions.
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

#include "libdwarfP.h"


int
dwarf_get_fde_n (fde_data, fde_index, returned_fde, error)
     Dwarf_Fde *fde_data;
     Dwarf_Unsigned fde_index;
     Dwarf_Fde *returned_fde;
     Dwarf_Error *error;
{
  Dwarf_Debug dbg = fde_data[0]->cie->dbg;

  if (fde_index >= dbg->fde_cnt)
    return DW_DLV_NO_ENTRY;

  *returned_fde = fde_data[fde_index];
  return DW_DLV_OK;
}
