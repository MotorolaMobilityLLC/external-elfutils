/* Function return value location for Alpha ELF ABI.
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

#include <assert.h>
#include <dwarf.h>

#define BACKEND alpha_
#include "libebl_CPU.h"


/* $0.  */
static const Dwarf_Op loc_intreg[] =
  {
    { .atom = DW_OP_reg0 }
  };
#define nloc_intreg	1

/* $f0, or pair $f0, $f1.  */
static const Dwarf_Op loc_fpreg[] =
  {
    { .atom = DW_OP_regx, .number = 32 }, { .atom = DW_OP_piece, .number = 4 },
    { .atom = DW_OP_regx, .number = 33 }, { .atom = DW_OP_piece, .number = 4 },
  };
#define nloc_fpreg	1
#define nloc_fpregpair	4

/* The return value is a structure and is actually stored in stack space
   passed in a hidden argument by the caller.  But, the compiler
   helpfully returns the address of that space in $0.  */
static const Dwarf_Op loc_aggregate[] =
  {
    { .atom = DW_OP_breg0, .number = 0 }
  };
#define nloc_aggregate 1

int
alpha_return_value_location (Dwarf_Die *functypedie, const Dwarf_Op **locp)
{
  /* Start with the function's type, and get the DW_AT_type attribute,
     which is the type of the return value.  */

  Dwarf_Attribute attr_mem;
  Dwarf_Attribute *attr = dwarf_attr (functypedie, DW_AT_type, &attr_mem);
  if (attr == NULL)
    /* The function has no return value, like a `void' function in C.  */
    return 0;

  Dwarf_Die die_mem;
  Dwarf_Die *typedie = dwarf_formref_die (attr, &die_mem);
  int tag = dwarf_tag (typedie);

  /* Follow typedefs and qualifiers to get to the actual type.  */
  while (tag == DW_TAG_typedef
	 || tag == DW_TAG_const_type || tag == DW_TAG_volatile_type
	 || tag == DW_TAG_restrict_type || tag == DW_TAG_mutable_type)
    {
      attr = dwarf_attr (typedie, DW_AT_type, &attr_mem);
      typedie = dwarf_formref_die (attr, &die_mem);
      tag = dwarf_tag (typedie);
    }

  switch (tag)
    {
    case -1:
      return -1;

    case DW_TAG_subrange_type:
      if (! dwarf_hasattr (typedie, DW_AT_byte_size))
	{
	  attr = dwarf_attr (typedie, DW_AT_type, &attr_mem);
	  typedie = dwarf_formref_die (attr, &die_mem);
	  tag = dwarf_tag (typedie);
	}
      /* Fall through.  */

    case DW_TAG_base_type:
    case DW_TAG_enumeration_type:
    case DW_TAG_pointer_type:
    case DW_TAG_ptr_to_member_type:
      {
	Dwarf_Word size;
	if (dwarf_formudata (dwarf_attr (typedie, DW_AT_byte_size,
					 &attr_mem), &size) != 0)
	  {
	    if (tag == DW_TAG_pointer_type || tag == DW_TAG_ptr_to_member_type)
	      size = 8;
	    else
	      return -1;
	  }
	if (tag == DW_TAG_base_type)
	  {
	    Dwarf_Word encoding;
	    if (dwarf_formudata (dwarf_attr (typedie, DW_AT_encoding,
					     &attr_mem), &encoding) != 0)
	      return -1;

	    *locp = loc_fpreg;
	    if (encoding == DW_ATE_float)
	      {
		if (size <= 8)
		  return nloc_fpreg;
		goto aggregate;
	      }
	    if (encoding == DW_ATE_complex_float)
	      {
		if (size <= 8 * 2)
		  return nloc_fpregpair;
		goto aggregate;
	      }
	  }
	if (size <= 8)
	  {
	    *locp = loc_intreg;
	    return nloc_intreg;
	  }
      }

      /* Else fall through.  */

    case DW_TAG_structure_type:
    case DW_TAG_class_type:
    case DW_TAG_union_type:
    case DW_TAG_string_type:
    case DW_TAG_array_type:
    aggregate:
      *locp = loc_aggregate;
      return nloc_aggregate;
    }

  /* XXX We don't have a good way to return specific errors from ebl calls.
     This value means we do not understand the type, but it is well-formed
     DWARF and might be valid.  */
  return -2;
}
