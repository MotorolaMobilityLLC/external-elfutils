/* Set up a session using libdwfl.
   Copyright (C) 2005 Red Hat, Inc.
   This file is part of elfutils.

   This file is free software; you can redistribute it and/or modify
   it under the terms of either

     * the GNU Lesser General Public License as published by the Free
       Software Foundation; either version 3 of the License, or (at
       your option) any later version

   or

     * the GNU General Public License as published by the Free
       Software Foundation; either version 2 of the License, or (at
       your option) any later version

   or both in parallel, as here.

   elfutils is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received copies of the GNU General Public License and
   the GNU Lesser General Public License along with this program.  If
   not, see <http://www.gnu.org/licenses/>.  */

#include "libdwflP.h"

Dwfl *
dwfl_begin (const Dwfl_Callbacks *callbacks)
{
  if (elf_version (EV_CURRENT) == EV_NONE)
    {
      __libdwfl_seterrno (DWFL_E_LIBELF);
      return NULL;
    }

  Dwfl *dwfl = calloc (1, sizeof *dwfl);
  if (dwfl == NULL)
    __libdwfl_seterrno (DWFL_E_NOMEM);
  else
    {
      dwfl->callbacks = callbacks;
      dwfl->offline_next_address = OFFLINE_REDZONE;
      dwfl->process_attach_error = DWFL_E_NO_ATTACH_STATE;
    }

  return dwfl;
}
INTDEF (dwfl_begin)
