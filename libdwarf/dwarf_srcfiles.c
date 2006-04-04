/* Return source files of compilation unit.
   Copyright (C) 2000, 2002 Red Hat, Inc.
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

#include <dwarf.h>
#include <stdlib.h>
#include <string.h>

#include <libdwarfP.h>


struct dirlist
{
  char *dir;
  size_t len;
  struct dirlist *next;
};

struct filelist
{
  char *name;
  Dwarf_Unsigned mtime;
  Dwarf_Unsigned length;
  struct filelist *next;
};


static int
read_file_names (Dwarf_Debug dbg, char *comp_dir, Dwarf_Small **linepp,
		 char ***result, Dwarf_Signed *nresult, Dwarf_Error *error)
{
  Dwarf_Small *linep = *linepp;
  struct dirlist comp_dir_elem;
  struct dirlist *dirlist;
  unsigned int ndirlist;
  struct dirlist **dirarray;
  struct filelist *filelist = NULL;
  unsigned int nfilelist = 0;

  /* First comes the list of directories.  Add the compilation directory
     first since the index zero is used for it.  */
  comp_dir_elem.dir = comp_dir;
  comp_dir_elem.len = comp_dir ? strlen (comp_dir) : 0;
  comp_dir_elem.next = NULL;
  dirlist = &comp_dir_elem;
  ndirlist = 1;

  while (*linep != 0)
    {
      struct dirlist *new_dir = (struct dirlist *) alloca (sizeof (*new_dir));

      new_dir->dir = (char *) linep;
      new_dir->len = strlen ((char *) linep);
      new_dir->next = dirlist;
      dirlist = new_dir;
      ++ndirlist;
      linep += new_dir->len + 1;
    }
  /* Skip the final NUL byte.  */
  ++linep;

  /* Rearrange the list in array form.  */
  dirarray = (struct dirlist **) alloca (sizeof (*dirarray));
  while (ndirlist-- > 0)
    {
      dirarray[ndirlist] = dirlist;
      dirlist = dirlist->next;
    }

  /* Now read the files.  */
  while (*linep != 0)
    {
      struct filelist *new_file =
	(struct filelist *) alloca (sizeof (*new_file));
      char *fname;
      size_t fnamelen;
      Dwarf_Unsigned diridx;

      /* First comes the file name.  */
      fname = (char *) linep;
      fnamelen = strlen (fname);
      linep += fnamelen + 1;

      /* Then the index.  */
      get_uleb128 (diridx, linep);
      if (unlikely (diridx >= ndirlist))
	{
	  __libdwarf_error (dbg, error, DW_E_INVALID_DIR_IDX);
	  return DW_DLV_ERROR;
	}

      if (*fname == '/')
	/* It's an absolute path.  */
	new_file->name = strdup (fname);
      else
	{
	  new_file->name = (char *) malloc (dirarray[diridx]->len + 1
					    + fnamelen + 1);
	  if (new_file->name != NULL)
	    {
	      char *cp = new_file->name;

	      if (dirarray[diridx]->dir != NULL)
		/* This value could be NULL in case the DW_AT_comp_dir
		   was not present.  We cannot do much in this case.
		   The easiest thing is to convert the path in an
		   absolute path.  */
		cp = stpcpy (cp, dirarray[diridx]->dir);
	      *cp++ = '/';
	      strcpy (cp, fname);
	    }
	}
      if (new_file->name == NULL)
	{
	  /* XXX Should we bother to free all the memory?  */
	  __libdwarf_error (dbg, error, DW_E_NOMEM);
	  return DW_DLV_ERROR;
	}

      /* Next comes the modification time.  */
      get_uleb128 (new_file->mtime, linep);

      /* Finally the length of the file.  */
      get_uleb128 (new_file->length, linep);

      new_file->next = filelist;
      filelist = new_file;
      ++nfilelist;
    }

  /* Put all the files in an array.  */
  *result = (char **) malloc (nfilelist * sizeof (char *));
  if (*result == NULL)
    {
      /* XXX Should we bother to free all the memory?  */
      __libdwarf_error (dbg, error, DW_E_NOMEM);
      return DW_DLV_ERROR;
    }

  *nresult = nfilelist;
  while (nfilelist-- > 0)
    {
      (*result)[nfilelist] = filelist->name;
      filelist = filelist->next;
    }

  /* Provide caller address of next byte.  */
  *linepp = linep + 1;

  return DW_DLV_OK;
}


int
dwarf_srcfiles (die, srcfiles, srcfilecount, error)
     Dwarf_Die die;
     char ***srcfiles;
     Dwarf_Signed *srcfilecount;
     Dwarf_Error *error;
{
  Dwarf_CU_Info cu = die->cu;
  Dwarf_Debug dbg = cu->dbg;
  Dwarf_Attribute stmt_list;
  Dwarf_Attribute comp_dir_attr;
  char *comp_dir;
  Dwarf_Unsigned offset;
  Dwarf_Small *linep;
  Dwarf_Small *lineendp;
  Dwarf_Small *header_start;
  Dwarf_Unsigned header_length;
  unsigned int unit_length;
  unsigned int version;
  unsigned int opcode_base;
  int length;
  int res;

  /* For now we haven't found anything.  */
  *srcfilecount = 0;

  /* The die must be for a compilation unit.  */
  if (die->abbrev->tag != DW_TAG_compile_unit)
    {
      __libdwarf_error (die->cu->dbg, error, DW_E_NO_CU);
      return DW_DLV_ERROR;
    }

  /* The die must have a statement list associated.  */
  res = dwarf_attr (die, DW_AT_stmt_list, &stmt_list, error);
  if (res != DW_DLV_OK)
    return res;

  /* Get the offset into the .debug_line section.  */
  res = dwarf_formudata (stmt_list, &offset, error);
  if (res != DW_DLV_OK)
    {
      dwarf_dealloc (dbg, stmt_list, DW_DLA_ATTR);
      return res;
    }

  /* We need a .debug_line section.  */
  if (dbg->sections[IDX_debug_line].addr == NULL)
    {
      dwarf_dealloc (dbg, stmt_list, DW_DLA_ATTR);
      __libdwarf_error (dbg, error, DW_E_NO_DEBUG_LINE);
      return DW_DLV_ERROR;
    }

  linep = (Dwarf_Small *) dbg->sections[IDX_debug_line].addr + offset;
  lineendp = ((Dwarf_Small *) dbg->sections[IDX_debug_line].addr
	      + dbg->sections[IDX_debug_line].size);

  /* Test whether at least the first 4 bytes are available.  */
  if (unlikely (linep + 4 > lineendp))
    {
      dwarf_dealloc (dbg, stmt_list, DW_DLA_ATTR);
      __libdwarf_error (dbg, error, DW_E_INVALID_DWARF);
      return DW_DLV_ERROR;
    }

  /* Get the compilation directory.  */
  res = dwarf_attr (die, DW_AT_comp_dir, &comp_dir_attr, error);
  if (unlikely (res == DW_DLV_ERROR)
      || (res == DW_DLV_OK
	  && unlikely (dwarf_formstring (comp_dir_attr, &comp_dir, error)
		       == DW_DLV_ERROR)))
    {
      dwarf_dealloc (dbg, stmt_list, DW_DLA_ATTR);
      __libdwarf_error (dbg, error, DW_E_INVALID_DWARF);
      return DW_DLV_ERROR;
    }
  else if (res == DW_DLV_OK)
    dwarf_dealloc (dbg, comp_dir_attr, DW_DLA_ATTR);
  else
    comp_dir = NULL;

  /* Read the unit_length.  */
  unit_length = read_4ubyte_unaligned (dbg, linep);
  linep += 4;
  length = 4;
  if (unit_length == 0xffffffff)
    {
      if (unlikely (linep + 8 > lineendp))
	{
	  dwarf_dealloc (dbg, stmt_list, DW_DLA_ATTR);
	  __libdwarf_error (dbg, error, DW_E_INVALID_DWARF);
	  return DW_DLV_ERROR;
	}

      unit_length = read_8ubyte_unaligned (dbg, linep);
      linep += 8;
      length = 8;
    }

  /* Check whether we have enough room in the section.  */
  if (unlikely (linep + unit_length > lineendp))
    {
      dwarf_dealloc (dbg, stmt_list, DW_DLA_ATTR);
      __libdwarf_error (dbg, error, DW_E_INVALID_DWARF);
      return DW_DLV_ERROR;
    }
  lineendp = linep + unit_length;

  /* The next element of the header is the version identifier.  */
  version = read_2ubyte_unaligned (dbg, linep);
  if (unlikely (version != DWARF_VERSION))
    {
      dwarf_dealloc (dbg, stmt_list, DW_DLA_ATTR);
      __libdwarf_error (dbg, error, DW_E_VERSION_ERROR);
      return DW_DLV_ERROR;
    }
  linep += 2;

  /* Next comes the header length.  */
  if (length == 4)
    {
      header_length = read_4ubyte_unaligned (dbg, linep);
      linep += 4;
    }
  else
    {
      header_length = read_8ubyte_unaligned (dbg, linep);
      linep += 8;
    }
  header_start = linep;

  /* Next the minimum instruction length.  Skip it.  */
  ++linep;

  /* Then the flag determining the default value of the is_stmt
     register.  Skip it.  */
  ++linep;

  /* Now the line base.  Skip it.  */
  ++linep;

  /* And the line range.  Skip it.  */
  ++linep;

  /* The opcode base.  */
  opcode_base = *linep++;

  /* Skip the array with the standard opcode length.  */
  linep += opcode_base - 1;

  /* Next the include directories and the file names.  */
  if (unlikely (read_file_names (dbg, comp_dir, &linep, srcfiles, srcfilecount,
				 error) != DW_DLV_OK))
    {
      dwarf_dealloc (dbg, stmt_list, DW_DLA_ATTR);
      return DW_DLV_ERROR;
    }

  /* Consistency check.  */
  if (unlikely (linep != header_start + header_length))
    {
      int i;

      for (i = 0; i < *srcfilecount; ++i)
	dwarf_dealloc (dbg, (*srcfiles)[i], DW_DLA_STRING);
      dwarf_dealloc (dbg, *srcfiles, DW_DLA_LIST);

      dwarf_dealloc (dbg, stmt_list, DW_DLA_ATTR);
      __libdwarf_error (dbg, error, DW_E_INVALID_DWARF);
      return DW_DLV_ERROR;
    }

  dwarf_dealloc (dbg, stmt_list, DW_DLA_ATTR);

  return DW_DLV_OK;
}
