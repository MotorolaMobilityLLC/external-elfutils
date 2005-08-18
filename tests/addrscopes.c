/* Test program for dwarf_getscopes.
   Copyright (C) 2005 Red Hat, Inc.

   This program is Open Source software; you can redistribute it and/or
   modify it under the terms of the Open Software License version 1.0 as
   published by the Open Source Initiative.

   You should have received a copy of the Open Software License along
   with this program; if not, you may obtain a copy of the Open Software
   License version 1.0 from http://www.opensource.org/licenses/osl.php or
   by writing the Open Source Initiative c/o Lawrence Rosen, Esq.,
   3001 King Ranch Road, Ukiah, CA 95482.   */

#include <config.h>
#include <assert.h>
#include <inttypes.h>
#include <libdwfl.h>
#include <dwarf.h>
#include <argp.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <locale.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>


static void
paddr (const char *prefix, Dwarf_Addr addr, Dwfl_Line *line)
{
  const char *src;
  int lineno, linecol;
  if (line != NULL
      && (src = dwfl_lineinfo (line, &addr, &lineno, &linecol,
			       NULL, NULL)) != NULL)
    {
      if (linecol != 0)
	printf ("%s%#" PRIx64 " (%s:%d:%d)",
		prefix, addr, src, lineno, linecol);
      else
	printf ("%s%#" PRIx64 " (%s:%d)",
		prefix, addr, src, lineno);
    }
  else
    printf ("%s%#" PRIx64, prefix, addr);
}

static const char *
dwarf_diename_integrate (Dwarf_Die *die)
{
  Dwarf_Attribute attr_mem;
  return dwarf_formstring (dwarf_attr_integrate (die, DW_AT_name, &attr_mem));
}

static void
print_vars (unsigned int indent, Dwarf_Die *die)
{
  Dwarf_Die child;
  if (dwarf_child (die, &child) == 0)
    do
      switch (dwarf_tag (&child))
	{
	case DW_TAG_variable:
	case DW_TAG_formal_parameter:
	  printf ("%*s%-30s[%6" PRIx64 "]\n", indent, "",
		  dwarf_diename_integrate (&child),
		  (uint64_t) dwarf_dieoffset (&child));
	  break;
	default:
	  break;
	}
    while (dwarf_siblingof (&child, &child) == 0);

  Dwarf_Attribute attr_mem;
  Dwarf_Die origin;
  if (dwarf_hasattr (die, DW_AT_abstract_origin)
      && dwarf_formref_die (dwarf_attr (die, DW_AT_abstract_origin, &attr_mem),
			    &origin) != NULL
      && dwarf_child (&origin, &child) == 0)
    do
      switch (dwarf_tag (&child))
	{
	case DW_TAG_variable:
	case DW_TAG_formal_parameter:
	  printf ("%*s%s (abstract)\n", indent, "",
		  dwarf_diename_integrate (&child));
	  break;
	default:
	  break;
	}
    while (dwarf_siblingof (&child, &child) == 0);
}


#define INDENT 4

static void
handle_address (GElf_Addr pc, Dwfl *dwfl)
{
  Dwarf_Addr cubias;
  Dwarf_Die *cudie = dwfl_addrdie (dwfl, pc, &cubias);
  if (cudie == NULL)
    error (EXIT_FAILURE, 0, "dwfl_addrdie: %s", dwfl_errmsg (-1));

  Dwarf_Die *scopes;
  int n = dwarf_getscopes (cudie, pc - cubias, &scopes);
  if (n < 0)
    error (EXIT_FAILURE, 0, "dwarf_getscopes: %s", dwarf_errmsg (-1));
  else if (n == 0)
    printf ("%#" PRIx64 ": not in any scope\n", pc);
  else
    {
      printf ("%#" PRIx64 ":\n", pc);
      unsigned int indent = 0;
      while (n-- > 0)
	{
	  Dwarf_Die *const die = &scopes[n];

	  indent += INDENT;
	  printf ("%*s%s (%#x)", indent, "",
		  dwarf_diename_integrate (die) ?: "<unnamed>",
		  dwarf_tag (die));

	  Dwarf_Addr lowpc, highpc;
	  if (dwarf_lowpc (die, &lowpc) == 0
	      && dwarf_highpc (die, &highpc) == 0)
	    {
	      Dwfl_Line *loline = dwfl_getsrc (dwfl, lowpc);
	      Dwfl_Line *hiline = dwfl_getsrc (dwfl, highpc);
	      paddr (": ", lowpc, loline);
	      if (highpc != lowpc)
		paddr (" .. ", lowpc, hiline == loline ? NULL : hiline);
	    }
	  puts ("");

	  print_vars (indent + INDENT, die);
	}
    }
}

int
main (int argc, char *argv[])
{
  int remaining;

  /* Set locale.  */
  (void) setlocale (LC_ALL, "");

  Dwfl *dwfl = NULL;
  (void) argp_parse (dwfl_standard_argp (), argc, argv, 0, &remaining, &dwfl);
  assert (dwfl != NULL);

  int result = 0;

  /* Now handle the addresses.  In case none are given on the command
     line, read from stdin.  */
  if (remaining == argc)
    {
      /* We use no threads here which can interfere with handling a stream.  */
      (void) __fsetlocking (stdin, FSETLOCKING_BYCALLER);

      char *buf = NULL;
      size_t len = 0;
      while (!feof_unlocked (stdin))
	{
	  if (getline (&buf, &len, stdin) < 0)
	    break;

	  char *endp;
	  uintmax_t addr = strtoumax (buf, &endp, 0);
	  if (endp != buf)
	    handle_address (addr, dwfl);
	  else
	    result = 1;
	}

      free (buf);
    }
  else
    {
      do
	{
	  char *endp;
	  uintmax_t addr = strtoumax (argv[remaining], &endp, 0);
	  if (endp != argv[remaining])
	    handle_address (addr, dwfl);
	  else
	    result = 1;
	}
      while (++remaining < argc);
    }

  return result;
}
