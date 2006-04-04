/* Return error message.
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

#include <libdwarfP.h>


/* Map error values to strings.  */
/* XXX This table should avoid string pointers.  Fixing it can wait
   until the code is stable.  */
static const char *msgs[] =
{
  [DW_E_NOERROR] = N_("no error"),
  [DW_E_INVALID_ACCESS] = N_("invalid access mode"),
  [DW_E_NO_REGFILE] = N_("no regular file"),
  [DW_E_IO_ERROR] = N_("I/O error"),
  [DW_E_NOMEM] = N_("out of memory"),
  [DW_E_NOELF] = N_("file is not an ELF file"),
  [DW_E_GETEHDR_ERROR] = N_("getehdr call failed"),
  [DW_E_INVALID_ELF] = N_("invalid ELF file"),
  [DW_E_INVALID_DWARF] = N_("invalid DWARF debugging information"),
  [DW_E_NO_DWARF] = N_("no DWARF debugging information available"),
  [DW_E_NO_CU] = N_("no compilation unit"),
  [DW_E_1ST_NO_CU] = N_("first die is no compile unit die"),
  [DW_E_INVALID_OFFSET] = N_("invalid offset"),
  [DW_E_INVALID_REFERENCE] = N_("invalid reference form"),
  [DW_E_NO_REFERENCE] = N_("no reference form"),
  [DW_E_NO_ADDR] = N_("no address form"),
  [DW_E_NO_FLAG] = N_("no flag form"),
  [DW_E_NO_CONSTANT] = N_("no constant form"),
  [DW_E_NO_BLOCK] = N_("no block form"),
  [DW_E_NO_STRING] = N_("no string form"),
  [DW_E_WRONG_ATTR] = N_("wrong attribute code"),
  [DW_E_NO_DATA] = N_("no data form"),
  [DW_E_NO_DEBUG_LINE] = N_(".debug_line section missing"),
  [DW_E_VERSION_ERROR] = N_("version mismatch"),
  [DW_E_INVALID_DIR_IDX] = N_("invalid directory index"),
  [DW_E_INVALID_ADDR] = N_("invalid address"),
  [DW_E_NO_ABBR] = N_("no valid abbreviation"),
};
#define nmsgs (sizeof (msgs) / sizeof (msgs[0]))


const char *
dwarf_errmsg (Dwarf_Error error)
{
  const char *retval = N_("unknown error");

  if (error->de_error < nmsgs)
    retval = msgs[error->de_error];

  return _(retval);
}
