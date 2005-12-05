/* Interfaces for libdwfl.
   Copyright (C) 2005 Red Hat, Inc.

   This program is Open Source software; you can redistribute it and/or
   modify it under the terms of the Open Software License version 1.0 as
   published by the Open Source Initiative.

   You should have received a copy of the Open Software License along
   with this program; if not, you may obtain a copy of the Open Software
   License version 1.0 from http://www.opensource.org/licenses/osl.php or
   by writing the Open Source Initiative c/o Lawrence Rosen, Esq.,
   3001 King Ranch Road, Ukiah, CA 95482.   */

#ifndef _LIBDWFL_H
#define _LIBDWFL_H	1

#include "libdw.h"

/* Handle for a session using the library.  */
typedef struct Dwfl Dwfl;

/* Handle for a module.  */
typedef struct Dwfl_Module Dwfl_Module;

/* Handle describing a line record.  */
typedef struct Dwfl_Line Dwfl_Line;

/* Callbacks.  */
typedef struct
{
  int (*find_elf) (Dwfl_Module *mod, void **userdata,
		   const char *modname, Dwarf_Addr base,
		   char **file_name, Elf **elfp);

  int (*find_debuginfo) (Dwfl_Module *mod, void **userdata,
			 const char *modname, Dwarf_Addr base,
			 const char *file_name,
			 const char *debuglink_file, GElf_Word debuglink_crc,
			 char **debuginfo_file_name);

  /* Fill *ADDR with the loaded address of the section called SECNAME in
     the given module.  This is called exactly once for each SHF_ALLOC
     section that relocations affecting DWARF data refer to, so it can
     easily be used to collect state about the sections referenced.  */
  int (*section_address) (Dwfl_Module *mod, void **userdata,
			  const char *modname, Dwarf_Addr base,
			  const char *secname,
			  Elf32_Word shndx, const GElf_Shdr *shdr,
			  Dwarf_Addr *addr);

  char **debuginfo_path;	/* See dwfl_standard_find_debuginfo.  */
} Dwfl_Callbacks;


/* Start a new session with the library.  */
extern Dwfl *dwfl_begin (const Dwfl_Callbacks *callbacks);

/* End a session.  */
extern void dwfl_end (Dwfl *);

/* Return error code of last failing function call.  This value is kept
   separately for each thread.  */
extern int dwfl_errno (void);

/* Return error string for ERROR.  If ERROR is zero, return error string
   for most recent error or NULL if none occurred.  If ERROR is -1 the
   behaviour is similar to the last case except that not NULL but a legal
   string is returned.  */
extern const char *dwfl_errmsg (int err);


/* Start reporting the current set of modules to the library.  No calls but
   dwfl_report_* can be made on DWFL until dwfl_report_end is called.  */
extern void dwfl_report_begin (Dwfl *dwfl);

/* Report that a module called NAME spans addresses [START, END).
   Returns the module handle, either existing or newly allocated,
   or returns a null pointer for an allocation error.  */
extern Dwfl_Module *dwfl_report_module (Dwfl *dwfl, const char *name,
					Dwarf_Addr start, Dwarf_Addr end);

/* Report a module with start and end addresses computed from the ELF
   program headers in the given file, plus BASE.  For an ET_REL file,
   does a simple absolute section layout starting at BASE.
   FD may be -1 to open FILE_NAME.  On success, FD is consumed by the
   library, and the `find_elf' callback will not be used for this module.  */
extern Dwfl_Module *dwfl_report_elf (Dwfl *dwfl, const char *name,
				     const char *file_name, int fd,
				     GElf_Addr base);

/* Similar, but report the module for offline use.  All ET_EXEC files
   being reported must be reported before any relocatable objects.
   If this is used, dwfl_report_module and dwfl_report_elf may not be
   used in the same reporting session.  */
extern Dwfl_Module *dwfl_report_offline (Dwfl *dwfl, const char *name,
					 const char *file_name, int fd);


/* Finish reporting the current set of modules to the library.
   If REMOVED is not null, it's called for each module that
   existed before but was not included in the current report.
   Returns a nonzero return value from the callback.
   The callback may call dwfl_report_module; doing so with the
   details of the module being removed prevents its removal.
   DWFL cannot be used until this function has returned zero.  */
extern int dwfl_report_end (Dwfl *dwfl,
			    int (*removed) (Dwfl_Module *, void *,
					    const char *, Dwarf_Addr,
					    void *arg),
			    void *arg);

/* Return the name of the module, and for each non-null argument store
   interesting details: *USERDATA is a location for storing your own
   pointer, **USERDATA is initially null; *START and *END give the address
   range covered by the module; *DWBIAS is the address bias for debugging
   information, and *SYMBIAS for symbol table entries (either is -1 if not
   yet accessed); *MAINFILE is the name of the ELF file, and *DEBUGFILE the
   name of the debuginfo file (might be equal to *MAINFILE; either is null
   if not yet accessed).  */
extern const char *dwfl_module_info (Dwfl_Module *mod, void ***userdata,
				     Dwarf_Addr *start, Dwarf_Addr *end,
				     Dwarf_Addr *dwbias, Dwarf_Addr *symbias,
				     const char **mainfile,
				     const char **debugfile);

/* Iterate through the modules, starting the walk with OFFSET == 0.
   Calls *CALLBACK for each module as long as it returns DWARF_CB_OK.
   When *CALLBACK returns another value, the walk stops and the
   return value can be passed as OFFSET to resume it.  Returns 0 when
   there are no more modules, or -1 for errors.  */
extern ptrdiff_t dwfl_getmodules (Dwfl *dwfl,
				  int (*callback) (Dwfl_Module *, void **,
						   const char *, Dwarf_Addr,
						   void *arg),
				  void *arg,
				  ptrdiff_t offset);


/*** Standard callbacks ***/

/* Standard find_debuginfo callback function.
   This is controlled by a string specifying directories to look in.
   If `debuginfo_path' is set in the Dwfl_Callbacks structure
   and the char * it points to is not null, that supplies the string.
   Otherwise a default path is used.

   If the first character of the string is + or - that says to check or to
   ignore (respectively) the CRC32 checksum from the .gnu_debuglink
   section.  The default is to check it.  The remainder of the string is
   composed of elements separated by colons.  Each element can start with +
   or - to override the global checksum behavior.  If the remainder of the
   element is empty, the directory containing the main file is tried; if
   it's an absolute path name, the absolute directory path containing the
   main file is taken as a subdirectory of this path; a relative path name
   is taken as a subdirectory of the directory containing the main file.
   Hence for /bin/ls, string ":.debug:/usr/lib/debug" says to look in /bin,
   then /bin/.debug, then /usr/lib/debug/bin, for the file name in the
   .gnu_debuglink section (or "ls.debug" if none was found).  */

extern int dwfl_standard_find_debuginfo (Dwfl_Module *, void **,
					 const char *, Dwarf_Addr,
					 const char *, const char *,
					 GElf_Word, char **);


/* This callback must be used when using dwfl_offline_* to report modules,
   if ET_REL is to be supported.  */
extern int dwfl_offline_section_address (Dwfl_Module *, void **,
					 const char *, Dwarf_Addr,
					 const char *, Elf32_Word,
					 const GElf_Shdr *,
					 Dwarf_Addr *addr);


/* Callbacks for working with kernel modules in the running Linux kernel.  */
extern int dwfl_linux_kernel_find_elf (Dwfl_Module *, void **,
				       const char *, Dwarf_Addr,
				       char **, Elf **);
extern int dwfl_linux_kernel_module_section_address (Dwfl_Module *, void **,
						     const char *, Dwarf_Addr,
						     const char *, Elf32_Word,
						     const GElf_Shdr *,
						     Dwarf_Addr *addr);

/* Call dwfl_report_elf for the running Linux kernel.
   Returns zero on success, -1 if dwfl_report_module failed,
   or an errno code if opening the kernel binary failed.  */
extern int dwfl_linux_kernel_report_kernel (Dwfl *dwfl);

/* Call dwfl_report_module for each kernel module in the running Linux kernel.
   Returns zero on success, -1 if dwfl_report_module failed,
   or an errno code if reading the list of modules failed.  */
extern int dwfl_linux_kernel_report_modules (Dwfl *dwfl);

/* Report a kernel and its modules found on disk, for offline use.
   If RELEASE starts with '/', it names a directory to look in;
   if not, it names a directory to find under /lib/modules/;
   if null, /lib/modules/`uname -r` is used.
   Returns zero on success, -1 if dwfl_report_module failed,
   or an errno code if finding the files on disk failed.

   If PREDICATE is not null, it is called with each module to be reported;
   its arguments are the module name, and the ELF file name or null if unknown,
   and its return value should be zero to skip the module, one to report it,
   or -1 to cause the call to fail and return errno.  */
extern int dwfl_linux_kernel_report_offline (Dwfl *dwfl, const char *release,
					     int (*predicate) (const char *,
							       const char *));


/* Call dwfl_report_module for each file mapped into the address space of PID.
   Returns zero on success, -1 if dwfl_report_module failed,
   or an errno code if opening the kernel binary failed.  */
extern int dwfl_linux_proc_report (Dwfl *dwfl, pid_t pid);

/* Trivial find_elf callback for use with dwfl_linux_proc_report.
   This uses the module name as a file name directly and tries to open it
   if it begin with a slash, or handles the magic string "[vdso]".  */
extern int dwfl_linux_proc_find_elf (Dwfl_Module *mod, void **userdata,
				     const char *module_name, Dwarf_Addr base,
				     char **file_name, Elf **);

/* Standard argument parsing for using a standard callback set.  */
struct argp;
extern const struct argp *dwfl_standard_argp (void) __attribute__ ((const));


/*** Relocation of addresses from Dwfl ***/

/* Return the number of relocatable bases associated with the module,
   which is zero for ET_EXEC and one for ET_DYN.  Returns -1 for errors.  */
extern int dwfl_module_relocations (Dwfl_Module *mod);

/* Return the relocation base index associated with the *ADDRESS location,
   and adjust *ADDRESS to be an offset relative to that base.
   Returns -1 for errors.  */
extern int dwfl_module_relocate_address (Dwfl_Module *mod,
					 Dwarf_Addr *address);

/* Return the ELF section name for the given relocation base index;
   if SHNDXP is not null, set *SHNDXP to the ELF section index.
   For ET_DYN, returns "" and sets *SHNDXP to SHN_ABS; the relocation
   base is the runtime start address reported for the module.
   Returns null for errors.  */
extern const char *dwfl_module_relocation_info (Dwfl_Module *mod,
						unsigned int idx,
						Elf32_Word *shndxp);

/* Validate that ADDRESS and ADDRESS+OFFSET lie in a known module
   and both within the same contiguous region for relocation purposes.
   Returns zero for success and -1 for errors.  */
extern int dwfl_validate_address (Dwfl *dwfl,
				  Dwarf_Addr address, Dwarf_Sword offset);


/*** Dwarf access functions ***/

/* Find the module containing the given address.  */
extern Dwfl_Module *dwfl_addrmodule (Dwfl *dwfl, Dwarf_Addr address);

/* Fetch the module main ELF file (where the allocated sections
   are found) for use with libelf.  If successful, fills in *BIAS
   with the difference between addresses within the loaded module
   and those in symbol tables or Dwarf information referring to it.  */
extern Elf *dwfl_module_getelf (Dwfl_Module *, GElf_Addr *bias);

/* Fetch the module's debug information for use with libdw.
   If successful, fills in *BIAS with the difference between
   addresses within the loaded module and those  to use with libdw.  */
extern Dwarf *dwfl_module_getdwarf (Dwfl_Module *, Dwarf_Addr *bias)
     __nonnull_attribute__ (2);

/* Get the libdw handle for each module.  */
extern ptrdiff_t dwfl_getdwarf (Dwfl *,
				int (*callback) (Dwfl_Module *, void **,
						 const char *, Dwarf_Addr,
						 Dwarf *, Dwarf_Addr, void *),
				void *arg, ptrdiff_t offset);

/* Look up the module containing ADDR and return its debugging information,
   loading it if necessary.  */
extern Dwarf *dwfl_addrdwarf (Dwfl *dwfl, Dwarf_Addr addr, Dwarf_Addr *bias)
     __nonnull_attribute__ (3);


/* Find the CU containing ADDR and return its DIE.  */
extern Dwarf_Die *dwfl_addrdie (Dwfl *dwfl, Dwarf_Addr addr, Dwarf_Addr *bias)
     __nonnull_attribute__ (3);
extern Dwarf_Die *dwfl_module_addrdie (Dwfl_Module *mod,
				       Dwarf_Addr addr, Dwarf_Addr *bias)
     __nonnull_attribute__ (3);

/* Iterate through the CUs, start with null for LASTCU.  */
extern Dwarf_Die *dwfl_nextcu (Dwfl *dwfl, Dwarf_Die *lastcu, Dwarf_Addr *bias)
     __nonnull_attribute__ (3);
extern Dwarf_Die *dwfl_module_nextcu (Dwfl_Module *mod,
				      Dwarf_Die *lastcu, Dwarf_Addr *bias)
     __nonnull_attribute__ (3);

/* Return the module containing the CU DIE.  */
extern Dwfl_Module *dwfl_cumodule (Dwarf_Die *cudie);


/* Cache the source line information fo the CU and return the
   number of Dwfl_Line entries it has.  */
extern int dwfl_getsrclines (Dwarf_Die *cudie, size_t *nlines);

/* Access one line number entry within the CU.  */
extern Dwfl_Line *dwfl_onesrcline (Dwarf_Die *cudie, size_t idx);

/* Get source for address.  */
extern Dwfl_Line *dwfl_module_getsrc (Dwfl_Module *mod, Dwarf_Addr addr);
extern Dwfl_Line *dwfl_getsrc (Dwfl *dwfl, Dwarf_Addr addr);

/* Get address for source.  */
extern int dwfl_module_getsrc_file (Dwfl_Module *mod,
				    const char *fname, int lineno, int column,
				    Dwfl_Line ***srcsp, size_t *nsrcs);

/* Return the module containing this line record.  */
extern Dwfl_Module *dwfl_linemodule (Dwfl_Line *line);

/* Return the source file name and fill in other information.
   Arguments may be null for unneeded fields.  */
extern const char *dwfl_lineinfo (Dwfl_Line *line, Dwarf_Addr *addr,
				  int *linep, int *colp,
				  Dwarf_Word *mtime, Dwarf_Word *length);


/* Find the symbol that ADDRESS lies inside, and return its name.  */
extern const char *dwfl_module_addrname (Dwfl_Module *mod, GElf_Addr address);



/* Return location expression to find return value given a
   DW_TAG_subprogram, DW_TAG_subroutine_type, or similar DIE describing
   function itself (whose DW_AT_type attribute describes its return type).
   The given DIE must come from the given module.  Returns -1 for errors.
   Returns zero if the function has no return value (e.g. "void" in C).
   Otherwise, *LOCOPS gets a location expression to find the return value,
   and returns the number of operations in the expression.  The pointer is
   permanently allocated at least as long as the module is live.  */
extern int dwfl_module_return_value_location (Dwfl_Module *mod,
					      Dwarf_Die *functypedie,
					      const Dwarf_Op **locops);

/* Enumerate the DWARF register numbers and their names.
   For each register, CALLBACK gets its DWARF number, a string describing
   the register set (such as "integer" or "FPU"), a prefix used in
   assembler syntax (such as "%" or "$", may be ""), and the name for the
   register (contains identifier characters only, possibly all digits).
   The REGNAME string is valid only during the callback. */
extern int dwfl_module_register_names (Dwfl_Module *mod,
				       int (*callback) (void *arg,
							int regno,
							const char *setname,
							const char *prefix,
							const char *regname),
				       void *arg);


#endif	/* libdwfl.h */
