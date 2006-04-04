/* Get required symbol version information at the given offset.
   Copyright (C) 1999, 2000, 2001, 2002 Red Hat, Inc.
   This file is part of Red Hat elfutils.
   Written by Ulrich Drepper <drepper@redhat.com>, 1999.

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

#include <assert.h>
#include <gelf.h>
#include <string.h>

#include "libelfP.h"


GElf_Verneed *
gelf_getverneed (data, offset, dst)
     Elf_Data *data;
     int offset;
     GElf_Verneed *dst;
{
  GElf_Verneed *result;

  if (data == NULL)
    return NULL;

  if (unlikely (data->d_type != ELF_T_VNEED))
    {
      __libelf_seterrno (ELF_E_INVALID_HANDLE);
      return NULL;
    }

  /* It's easy to handle this type.  It has the same size for 32 and
     64 bit objects.  And fortunately the `ElfXXX_Vernaux' records
     also have the same size.  */
  assert (sizeof (GElf_Verneed) == sizeof (Elf32_Verneed));
  assert (sizeof (GElf_Verneed) == sizeof (Elf64_Verneed));
  assert (sizeof (GElf_Verneed) == sizeof (Elf32_Vernaux));
  assert (sizeof (GElf_Verneed) == sizeof (Elf64_Vernaux));

  rwlock_rdlock (((Elf_Data_Scn *) data)->s->elf->lock);

  /* The data is already in the correct form.  Just make sure the
     index is OK.  */
  if (unlikely (offset < 0)
      || unlikely (offset + sizeof (GElf_Verneed) > data->d_size)
      || unlikely (offset % sizeof (GElf_Verneed) != 0))
    {
      __libelf_seterrno (ELF_E_OFFSET_RANGE);
      result = NULL;
    }
  else
    result = (GElf_Verneed *) memcpy (dst, (char *) data->d_buf + offset,
				      sizeof (GElf_Verneed));

  rwlock_unlock (((Elf_Data_Scn *) data)->s->elf->lock);

  return result;
}
