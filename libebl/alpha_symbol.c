/* Alpha specific symbolic name handling.
   Copyright (C) 2002, 2005 Red Hat, Inc.
   Written by Ulrich Drepper <drepper@redhat.com>, 2002.

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

#include <elf.h>
#include <stddef.h>

#include <libebl_alpha.h>


/* Return of the backend.  */
const char *
alpha_backend_name (void)
{
  return "alpha";
}


/* Relocation mapping table.  */
static const char *reloc_map_table[] =
  {
    [R_ALPHA_NONE] = "R_ALPHA_NONE",
    [R_ALPHA_REFLONG] = "R_ALPHA_REFLONG",
    [R_ALPHA_REFQUAD] = "R_ALPHA_REFQUAD",
    [R_ALPHA_GPREL32] = "R_ALPHA_GPREL32",
    [R_ALPHA_LITERAL] = "R_ALPHA_LITERAL",
    [R_ALPHA_LITUSE] = "R_ALPHA_LITUSE",
    [R_ALPHA_GPDISP] = "R_ALPHA_GPDISP",
    [R_ALPHA_BRADDR] = "R_ALPHA_BRADDR",
    [R_ALPHA_HINT] = "R_ALPHA_HINT",
    [R_ALPHA_SREL16] = "R_ALPHA_SREL16",
    [R_ALPHA_SREL32] = "R_ALPHA_SREL32",
    [R_ALPHA_SREL64] = "R_ALPHA_SREL64",
    [R_ALPHA_GPRELHIGH] = "R_ALPHA_GPRELHIGH",
    [R_ALPHA_GPRELLOW] = "R_ALPHA_GPRELLOW",
    [R_ALPHA_GPREL16] = "R_ALPHA_GPREL16",
    [R_ALPHA_COPY] = "R_ALPHA_COPY",
    [R_ALPHA_GLOB_DAT] = "R_ALPHA_GLOB_DAT",
    [R_ALPHA_JMP_SLOT] = "R_ALPHA_JMP_SLOT",
    [R_ALPHA_RELATIVE] = "R_ALPHA_RELATIVE",
    [R_ALPHA_TLS_GD_HI] = "R_ALPHA_TLS_GD_HI",
    [R_ALPHA_TLSGD] = "R_ALPHA_TLSGD",
    [R_ALPHA_TLS_LDM] = "R_ALPHA_TLS_LDM",
    [R_ALPHA_DTPMOD64] = "R_ALPHA_DTPMOD64",
    [R_ALPHA_GOTDTPREL] = "R_ALPHA_GOTDTPREL",
    [R_ALPHA_DTPREL64] = "R_ALPHA_DTPREL64",
    [R_ALPHA_DTPRELHI] = "R_ALPHA_DTPRELHI",
    [R_ALPHA_DTPRELLO] = "R_ALPHA_DTPRELLO",
    [R_ALPHA_DTPREL16] = "R_ALPHA_DTPREL16",
    [R_ALPHA_GOTTPREL] = "R_ALPHA_GOTTPREL",
    [R_ALPHA_TPREL64] = "R_ALPHA_TPREL64",
    [R_ALPHA_TPRELHI] = "R_ALPHA_TPRELHI",
    [R_ALPHA_TPRELLO] = "R_ALPHA_TPRELLO",
    [R_ALPHA_TPREL16] = "R_ALPHA_TPREL16"
  };


/* Determine relocation type string for Alpha.  */
const char *
alpha_reloc_type_name (int type, char *buf __attribute__ ((unused)),
		       size_t len __attribute__ ((unused)))
{
  if (type < 0
      || ((size_t) type
	  >= sizeof (reloc_map_table) / sizeof (reloc_map_table[0])))
    return NULL;

  return reloc_map_table[type];
}


/* Check for correct relocation type.  */
bool
alpha_reloc_type_check (int type)
{
  return (type >= R_ALPHA_NONE
	  && ((size_t) type
	      < sizeof (reloc_map_table) / sizeof (reloc_map_table[0]))
	  && reloc_map_table[type] != NULL) ? true : false;
}


const char *
alpha_dynamic_tag_name (int64_t tag, char *buf __attribute__ ((unused)),
			size_t len __attribute__ ((unused)))
{
  switch (tag)
    {
    case DT_ALPHA_PLTRO:
      return "ALPHA_PLTRO";
    default:
      break;
    }
  return NULL;
}


bool
alpha_dynamic_tag_check (int64_t tag)
{
  return tag == DT_ALPHA_PLTRO;
}


/* Check whether given relocation is a copy relocation.  */
bool
alpha_copy_reloc_p (int reloc)
{
  return reloc == R_ALPHA_COPY;
}
