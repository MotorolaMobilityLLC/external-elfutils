/* Determine whether die has attribute specified of given type.
   Copyright (C) 2000, 2001, 2002 Red Hat, Inc.
   This file is part of Red Hat elfutils.
   Written by Ulrich Drepper <drepper@redhat.com>, 2000.

   Red Hat elfutils is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by the
   Free Software Foundation; version 2 of the License.

   Red Hat elfutils is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with Red Hat elfutils; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.

   In addition, as a special exception, Red Hat, Inc. gives You the
   additional right to link the code of Red Hat elfutils with code licensed
   under any Open Source Initiative certified open source license
   (http://www.opensource.org/licenses/index.php) which requires the
   distribution of source code with any binary distribution and to
   distribute linked combinations of the two.  Non-GPL Code permitted under
   this exception must only link to the code of Red Hat elfutils through
   those well defined interfaces identified in the file named EXCEPTION
   found in the source code files (the "Approved Interfaces").  The files
   of Non-GPL Code may instantiate templates or use macros or inline
   functions from the Approved Interfaces without causing the resulting
   work to be covered by the GNU General Public License.  Only Red Hat,
   Inc. may make changes or additions to the list of Approved Interfaces.
   Red Hat's grant of this exception is conditioned upon your not adding
   any new exceptions.  If you wish to add a new Approved Interface or
   exception, please contact Red Hat.  You must obey the GNU General Public
   License in all respects for all of the Red Hat elfutils code and other
   code used in conjunction with Red Hat elfutils except the Non-GPL Code
   covered by this exception.  If you modify this file, you may extend this
   exception to your version of the file, but you are not obligated to do
   so.  If you do not wish to provide this exception without modification,
   you must delete this exception statement from your version and license
   this file solely under the GPL without exception.

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

#include <libdwarfP.h>


int
dwarf_hasattr (die, attr, return_bool, error)
     Dwarf_Die die;
     Dwarf_Half attr;
     Dwarf_Bool *return_bool;
     Dwarf_Error *error;
{
  Dwarf_Debug dbg = die->cu->dbg;
  Dwarf_Small *die_addr;
  Dwarf_Word u128;
  Dwarf_Abbrev abbrev;
  Dwarf_Small *attrp;

  /* Address of the given die.  */
  die_addr = die->addr;

  /* Get abbrev code.  */
  get_uleb128 (u128, die_addr);
  /*  And get the abbreviation itself.  */
  abbrev = __libdwarf_get_abbrev (dbg, die->cu, u128, error);
  if (abbrev == NULL)
    return DW_DLV_ERROR;

  /* This is where the attributes start.  */
  attrp = abbrev->attrp;

  /* Search the name attribute.  */
  while (1)
    {
      Dwarf_Word attr_name;
      Dwarf_Word attr_form;

      /* Are we still in bounds?  */
      if (unlikely (attrp
		    >= ((Dwarf_Small *) dbg->sections[IDX_debug_abbrev].addr
			+ dbg->sections[IDX_debug_abbrev].size)))
	{
	  __libdwarf_error (dbg, error, DW_E_INVALID_DWARF);
	  return DW_DLV_ERROR;
	}

      /* Get attribute name and form.

	 XXX We don't check whether this reads beyond the end of the
	 section.  */
      get_uleb128 (attr_name, attrp);
      get_uleb128 (attr_form, attrp);

      /* We can stop if we found the attribute with value zero.  */
      if (attr_name == 0 && attr_form == 0)
	break;

      /* Is this the name attribute?  */
      if (attr_name == attr)
	{
	  *return_bool = 1;
	  return DW_DLV_OK;
	}

      /* Skip over the rest of this attribute (if there is any).  */
      if (attr_form != 0)
	{
	  size_t len;

	  if (unlikely (__libdwarf_form_val_len (dbg, die->cu, attr_form,
						 die_addr, &len, error)
			!= DW_DLV_OK))
	    return DW_DLV_ERROR;

	  die_addr += len;
	}
    }

  /* No such attribute present.  */
  *return_bool = 0;
  return DW_DLV_OK;
}
