/* Standard libdwfl callbacks for debugging the running Linux kernel.
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
#undef	_FILE_OFFSET_BITS	/* Doesn't jibe with fts.  */

#include "libdwflP.h"
#include <inttypes.h>
#include <errno.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <fcntl.h>
#include <unistd.h>
#include <fts.h>


#define MODULEDIRFMT	"/lib/modules/%s"

#define MODULELIST	"/proc/modules"
#define	SECADDRFMT	"/sys/module/%s/sections/%s"


/* Try to open the given file as it is or under the debuginfo directory.  */
static int
try_kernel_name (Dwfl *dwfl, char **fname)
{
  if (*fname == NULL)
    return -1;

  int fd = TEMP_FAILURE_RETRY (open64 (*fname, O_RDONLY));
  if (fd < 0)
    {
      char *debugfname = NULL;
      Dwfl_Module fakemod = { .dwfl = dwfl };
      fd = INTUSE(dwfl_standard_find_debuginfo) (&fakemod, NULL, NULL, 0,
						 *fname, basename (*fname), 0,
						 &debugfname);
      free (*fname);
      *fname = debugfname;
    }

  return fd;
}

static inline const char *
kernel_release (void)
{
  /* Cache the `uname -r` string we'll use.  */
  static struct utsname utsname;
  if (utsname.release[0] == '\0' && uname (&utsname) != 0)
    return NULL;
  return utsname.release;
}

static int
report_kernel (Dwfl *dwfl, const char *release,
	       int (*predicate) (const char *module, const char *file))
{
  if (dwfl == NULL)
    return -1;

  char *fname = NULL;
  if (release[0] == '/')
    asprintf (&fname, "%s/vmlinux", release);
  else
    asprintf (&fname, "/boot/vmlinux-%s", release);
  int fd = try_kernel_name (dwfl, &fname);
  if (fd < 0 && release[0] != '/')
    {
      free (fname);
      fname = NULL;
      asprintf (&fname, MODULEDIRFMT "/vmlinux", release);
      fd = try_kernel_name (dwfl, &fname);
    }

  int result = 0;
  if (fd < 0)
    result = (predicate != NULL && !(*predicate) ("kernel", NULL)) ? 0 : errno;
  else
    {
      bool report = true;

      if (predicate != NULL)
	{
	  /* Let the predicate decide whether to use this one.  */
	  int want = (*predicate) ("kernel", fname);
	  if (want < 0)
	    result = errno;
	  report = want > 0;
	}

      if (report
	  && INTUSE(dwfl_report_elf) (dwfl, "kernel", fname, fd, 0) == NULL)
	{
	  close (fd);
	  result = -1;
	}
    }

  free (fname);

  return result;
}

/* Report a kernel and all its modules found on disk, for offline use.
   If RELEASE starts with '/', it names a directory to look in;
   if not, it names a directory to find under /lib/modules/;
   if null, /lib/modules/`uname -r` is used.
   Returns zero on success, -1 if dwfl_report_module failed,
   or an errno code if finding the files on disk failed.  */
int
dwfl_linux_kernel_report_offline (Dwfl *dwfl, const char *release,
				  int (*predicate) (const char *module,
						    const char *file))
{
  if (release == NULL)
    {
      release = kernel_release ();
      if (release == NULL)
	return errno;
    }

  /* First report the kernel.  */
  int result = report_kernel (dwfl, release, predicate);
  if (result == 0)
    {
      /* Do "find /lib/modules/RELEASE/kernel -name *.ko".  */

      char *modulesdir[] = { NULL, NULL };
      if (release[0] == '/')
	modulesdir[0] = (char *) release;
      else
	{
	  asprintf (&modulesdir[0], MODULEDIRFMT "/kernel", release);
	  if (modulesdir[0] == NULL)
	    return errno;
	}

      FTS *fts = fts_open (modulesdir, FTS_LOGICAL | FTS_NOSTAT, NULL);
      if (modulesdir[0] == (char *) release)
	modulesdir[0] = NULL;
      if (fts == NULL)
	{
	  free (modulesdir[0]);
	  return errno;
	}

      FTSENT *f;
      while ((f = fts_read (fts)) != NULL)
	{
	  switch (f->fts_info)
	    {
	    case FTS_F:
	    case FTS_NSOK:
	      /* See if this file name matches "*.ko".  */
	      if (f->fts_namelen > 3
		  && !memcmp (f->fts_name + f->fts_namelen - 3, ".ko", 4))
		{
		  /* We have a .ko file to report.  Following the algorithm
		     by which the kernel makefiles set KBUILD_MODNAME, we
		     replace all ',' or '-' with '_' in the file name and
		     call that the module name.  Modules could well be
		     built using different embedded names than their file
		     names.  To handle that, we would have to look at the
		     __this_module.name contents in the module's text.  */

		  char name[f->fts_namelen - 3 + 1];
		  for (size_t i = 0; i < f->fts_namelen - 3U; ++i)
		    if (f->fts_name[i] == '-' || f->fts_name[i] == ',')
		      name[i] = '_';
		    else
		      name[i] = f->fts_name[i];
		  name[f->fts_namelen - 3] = '\0';

		  if (predicate != NULL)
		    {
		      /* Let the predicate decide whether to use this one.  */
		      int want = (*predicate) (name, f->fts_path);
		      if (want < 0)
			{
			  result = -1;
			  break;
			}
		      if (!want)
			continue;
		    }

		  if (dwfl_report_offline (dwfl, name,
					   f->fts_path, -1) == NULL)
		    {
		      result = -1;
		      break;
		    }
		}
	      continue;

	    case FTS_ERR:
	    case FTS_DNR:
	    case FTS_NS:
	      result = f->fts_errno;
	      break;

	    default:
	      continue;
	    }

	  /* We only get here in error cases.  */
	  break;
	}
      fts_close (fts);
      free (modulesdir[0]);
    }

  return result;
}
INTDEF (dwfl_linux_kernel_report_offline)


/* Find the ELF file for the running kernel and dwfl_report_elf it.  */
int
dwfl_linux_kernel_report_kernel (Dwfl *dwfl)
{
  const char *release = kernel_release ();
  if (release == NULL)
    return errno;

  return report_kernel (dwfl, release, NULL);
}
INTDEF (dwfl_linux_kernel_report_kernel)


/* Dwfl_Callbacks.find_elf for the running Linux kernel and its modules.  */

int
dwfl_linux_kernel_find_elf (Dwfl_Module *mod __attribute__ ((unused)),
			    void **userdata __attribute__ ((unused)),
			    const char *module_name,
			    Dwarf_Addr base __attribute__ ((unused)),
			    char **file_name,
			    Elf **elfp __attribute__ ((unused)))
{
  const char *release = kernel_release ();
  if (release == NULL)
    return errno;

  /* Do "find /lib/modules/`uname -r`/kernel -name MODULE_NAME.ko".  */

  char *modulesdir[] = { NULL, NULL };
  asprintf (&modulesdir[0], MODULEDIRFMT "/kernel", release);
  if (modulesdir[0] == NULL)
    return -1;

  FTS *fts = fts_open (modulesdir, FTS_LOGICAL | FTS_NOSTAT, NULL);
  if (fts == NULL)
    {
      free (modulesdir[0]);
      return -1;
    }

  size_t namelen = strlen (module_name);

  /* This is a kludge.  There is no actual necessary relationship between
     the name of the .ko file installed and the module name the kernel
     knows it by when it's loaded.  The kernel's only idea of the module
     name comes from the name embedded in the object's magic
     .gnu.linkonce.this_module section.

     In practice, these module names match the .ko file names except for
     some using '_' and some using '-'.  So our cheap kludge is to look for
     two files when either a '_' or '-' appears in a module name, one using
     only '_' and one only using '-'.  */

  char alternate_name[namelen + 1];
  inline bool subst_name (char from, char to)
    {
      const char *n = memchr (module_name, from, namelen);
      if (n == NULL)
	return false;
      char *a = mempcpy (alternate_name, module_name, n - module_name);
      *a++ = to;
      ++n;
      const char *p;
      while ((p = memchr (n, from, namelen - (n - module_name))) != NULL)
	{
	  a = mempcpy (a, n, p - n);
	  *a++ = to;
	  n = p + 1;
	}
      memcpy (a, n, namelen - (n - module_name) + 1);
      return true;
    }
  if (!subst_name ('-', '_') && !subst_name ('_', '-'))
    alternate_name[0] = '\0';

  FTSENT *f;
  int error = ENOENT;
  while ((f = fts_read (fts)) != NULL)
    {
      error = ENOENT;
      switch (f->fts_info)
	{
	case FTS_F:
	case FTS_NSOK:
	  /* See if this file name is "MODULE_NAME.ko".  */
	  if (f->fts_namelen == namelen + 3
	      && !memcmp (f->fts_name + namelen, ".ko", 4)
	      && (!memcmp (f->fts_name, module_name, namelen)
		  || !memcmp (f->fts_name, alternate_name, namelen)))
	    {
	      int fd = open64 (f->fts_accpath, O_RDONLY);
	      *file_name = strdup (f->fts_path);
	      fts_close (fts);
	      free (modulesdir[0]);
	      if (fd < 0)
		free (*file_name);
	      else if (*file_name == NULL)
		{
		  close (fd);
		  fd = -1;
		}
	      return fd;
	    }
	  break;

	case FTS_ERR:
	case FTS_DNR:
	case FTS_NS:
	  error = f->fts_errno;
	  break;

	default:
	  break;
	}
    }

  fts_close (fts);
  free (modulesdir[0]);
  errno = error;
  return -1;
}
INTDEF (dwfl_linux_kernel_find_elf)


/* Dwfl_Callbacks.section_address for kernel modules in the running Linux.
   We read the information from /sys/module directly.  */

int
dwfl_linux_kernel_module_section_address
(Dwfl_Module *mod __attribute__ ((unused)),
 void **userdata __attribute__ ((unused)),
 const char *modname, Dwarf_Addr base __attribute__ ((unused)),
 const char *secname, Elf32_Word shndx __attribute__ ((unused)),
 const GElf_Shdr *shdr __attribute__ ((unused)),
 Dwarf_Addr *addr)
{
  char *sysfile = NULL;
  asprintf (&sysfile, SECADDRFMT, modname, secname);
  if (sysfile == NULL)
    return ENOMEM;

  FILE *f = fopen (sysfile, "r");
  if (f == NULL)
    {
      if (errno == ENOENT)
	{
	  /* The .modinfo and .data.percpu sections are never kept
	     loaded in the kernel.  If the kernel was compiled without
	     CONFIG_MODULE_UNLOAD, the .exit.* sections are not
	     actually loaded at all.

	     Just relocate these bogusly to zero.  This part of the
	     debug information will not be of any use.  */

	  if (!strcmp (secname, ".modinfo")
	      || !strcmp (secname, ".data.percpu")
	      || !strncmp (secname, ".exit", 5))
	    {
	      *addr = 0;
	      return DWARF_CB_OK;
	    }
	}

      return DWARF_CB_ABORT;
    }

  (void) __fsetlocking (f, FSETLOCKING_BYCALLER);

  int result = (fscanf (f, "%" PRIi64 "\n", addr) == 1 ? 0
		: ferror_unlocked (f) ? errno : ENOEXEC);
  fclose (f);

  if (result == 0)
    return DWARF_CB_OK;

  errno = result;
  return DWARF_CB_ABORT;
}
INTDEF (dwfl_linux_kernel_module_section_address)

int
dwfl_linux_kernel_report_modules (Dwfl *dwfl)
{
  FILE *f = fopen (MODULELIST, "r");
  if (f == NULL)
    return errno;

  (void) __fsetlocking (f, FSETLOCKING_BYCALLER);

  int result = 0;
  Dwarf_Addr modaddr;
  unsigned long int modsz;
  char modname[128];
  while (fscanf (f, "%128s %lu %*s %*s %*s %" PRIi64 "\n",
		 modname, &modsz, &modaddr) == 3)
    if (INTUSE(dwfl_report_module) (dwfl, modname,
				    modaddr, modaddr + modsz) == NULL)
      {
	result = -1;
	break;
      }

  if (result == 0)
    result = ferror_unlocked (f) ? errno : feof_unlocked (f) ? 0 : ENOEXEC;

  fclose (f);

  return result;
}
INTDEF (dwfl_linux_kernel_report_modules)
