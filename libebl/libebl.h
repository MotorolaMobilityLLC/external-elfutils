/* Interface for libebl.
   Copyright (C) 2000, 2001, 2002, 2004, 2005 Red Hat, Inc.

   This program is Open Source software; you can redistribute it and/or
   modify it under the terms of the Open Software License version 1.0 as
   published by the Open Source Initiative.

   You should have received a copy of the Open Software License along
   with this program; if not, you may obtain a copy of the Open Software
   License version 1.0 from http://www.opensource.org/licenses/osl.php or
   by writing the Open Source Initiative c/o Lawrence Rosen, Esq.,
   3001 King Ranch Road, Ukiah, CA 95482.   */

#ifndef _LIBEBL_H
#define _LIBEBL_H 1

#include <gelf.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <elf-knowledge.h>


/* Opaque type for the handle.  */
typedef struct ebl Ebl;


/* Get backend handle for object associated with ELF handle.  */
extern Ebl *ebl_openbackend (Elf *elf);
/* Similar but without underlying ELF file.  */
extern Ebl *ebl_openbackend_machine (GElf_Half machine);
/* Similar but with emulation name given.  */
extern Ebl *ebl_openbackend_emulation (const char *emulation);

/* Free resources allocated for backend handle.  */
extern void ebl_closebackend (Ebl *bh);


/* Information about the descriptor.  */

/* Get ELF machine.  */
extern int ebl_get_elfmachine (Ebl *ebl) __attribute__ ((__pure__));

/* Get ELF class.  */
extern int ebl_get_elfclass (Ebl *ebl) __attribute__ ((__pure__));

/* Get ELF data encoding.  */
extern int ebl_get_elfdata (Ebl *ebl) __attribute__ ((__pure__));


/* Function to call the callback functions including default ELF
   handling.  */

/* Return backend name.  */
extern const char *ebl_backend_name (Ebl *ebl);

/* Return relocation type name.  */
extern const char *ebl_object_type_name (Ebl *ebl, int object,
					 char *buf, size_t len);

/* Return relocation type name.  */
extern const char *ebl_reloc_type_name (Ebl *ebl, int reloc,
					char *buf, size_t len);

/* Check relocation type.  */
extern bool ebl_reloc_type_check (Ebl *ebl, int reloc);

/* Check relocation type use.  */
extern bool ebl_reloc_valid_use (Ebl *ebl, int reloc);

/* Check if relocation type is for simple absolute relocations.
   Return ELF_T_{BYTE,HALF,SWORD,SXWORD} for a simple type, else ELF_T_NUM.  */
extern Elf_Type ebl_reloc_simple_type (Ebl *ebl, int reloc);

/* Return true if the symbol type is that referencing the GOT.  E.g.,
   R_386_GOTPC.  */
extern bool ebl_gotpc_reloc_check (Ebl *ebl, int reloc);

/* Return segment type name.  */
extern const char *ebl_segment_type_name (Ebl *ebl, int segment,
					  char *buf, size_t len);

/* Return section type name.  */
extern const char *ebl_section_type_name (Ebl *ebl, int section,
					  char *buf, size_t len);

/* Return section name.  */
extern const char *ebl_section_name (Ebl *ebl, int section, int xsection,
				     char *buf, size_t len,
				     const char *scnnames[], size_t shnum);

/* Return machine flag names.  */
extern const char *ebl_machine_flag_name (Ebl *ebl, GElf_Word flags,
					  char *buf, size_t len);

/* Check whether machine flag is valid.  */
extern bool ebl_machine_flag_check (Ebl *ebl, GElf_Word flags);

/* Return symbol type name.  */
extern const char *ebl_symbol_type_name (Ebl *ebl, int symbol,
					 char *buf, size_t len);

/* Return symbol binding name.  */
extern const char *ebl_symbol_binding_name (Ebl *ebl, int binding,
					    char *buf, size_t len);

/* Return dynamic tag name.  */
extern const char *ebl_dynamic_tag_name (Ebl *ebl, int64_t tag,
					 char *buf, size_t len);

/* Check dynamic tag.  */
extern bool ebl_dynamic_tag_check (Ebl *ebl, int64_t tag);

/* Check whether given symbol's st_value and st_size are OK despite failing
   normal checks.  */
extern bool ebl_check_special_symbol (Ebl *ebl, GElf_Ehdr *ehdr,
				      const GElf_Sym *sym, const char *name,
				      const GElf_Shdr *destshdr);

/* Return combined section header flags value.  */
extern GElf_Word ebl_sh_flags_combine (Ebl *ebl, GElf_Word flags1,
				       GElf_Word flags2);

/* Return symbolic representation of OS ABI.  */
extern const char *ebl_osabi_name (Ebl *ebl, int osabi, char *buf, size_t len);


/* Return name of the note section type for a core file.  */
extern const char *ebl_core_note_type_name (Ebl *ebl, uint32_t type, char *buf,
					    size_t len);

/* Return name of the note section type for an object file.  */
extern const char *ebl_object_note_type_name (Ebl *ebl, uint32_t type,
					      char *buf, size_t len);

/* Print information about core note if available.  */
extern void ebl_core_note (Ebl *ebl, const char *name, uint32_t type,
			   uint32_t descsz, const char *desc);

/* Print information about object note if available.  */
extern void ebl_object_note (Ebl *ebl, const char *name, uint32_t type,
			     uint32_t descsz, const char *desc);

/* Check section name for being that of a debug informatino section.  */
extern bool ebl_debugscn_p (Ebl *ebl, const char *name);

/* Check whether given relocation is a copy relocation.  */
extern bool ebl_copy_reloc_p (Ebl *ebl, int reloc);

/* Check whether section should be stripped.  */
extern bool ebl_section_strip_p (Ebl *ebl, const GElf_Ehdr *ehdr,
				 const GElf_Shdr *shdr, const char *name,
				 bool remove_comment, bool only_remove_debug);

/* Check if backend uses a bss PLT in this file.  */
extern bool ebl_bss_plt_p (Ebl *ebl, GElf_Ehdr *ehdr);


/* ELF string table handling.  */
struct Ebl_Strtab;
struct Ebl_Strent;

/* Create new ELF string table object in memory.  */
extern struct Ebl_Strtab *ebl_strtabinit (bool nullstr);

/* Free resources allocated for ELF string table ST.  */
extern void ebl_strtabfree (struct Ebl_Strtab *st);

/* Add string STR (length LEN is != 0) to ELF string table ST.  */
extern struct Ebl_Strent *ebl_strtabadd (struct Ebl_Strtab *st,
					 const char *str, size_t len);

/* Finalize string table ST and store size and memory location information
   in DATA.  */
extern void ebl_strtabfinalize (struct Ebl_Strtab *st, Elf_Data *data);

/* Get offset in string table for string associated with SE.  */
extern size_t ebl_strtaboffset (struct Ebl_Strent *se);

/* Return the string associated with SE.  */
extern const char *ebl_string (struct Ebl_Strent *se);


/* ELF wide char string table handling.  */
struct Ebl_WStrtab;
struct Ebl_WStrent;

/* Create new ELF wide char string table object in memory.  */
extern struct Ebl_WStrtab *ebl_wstrtabinit (bool nullstr);

/* Free resources allocated for ELF wide char string table ST.  */
extern void ebl_wstrtabfree (struct Ebl_WStrtab *st);

/* Add string STR (length LEN is != 0) to ELF string table ST.  */
extern struct Ebl_WStrent *ebl_wstrtabadd (struct Ebl_WStrtab *st,
					   const wchar_t *str, size_t len);

/* Finalize string table ST and store size and memory location information
   in DATA.  */
extern void ebl_wstrtabfinalize (struct Ebl_WStrtab *st, Elf_Data *data);

/* Get offset in wide char string table for string associated with SE.  */
extern size_t ebl_wstrtaboffset (struct Ebl_WStrent *se);


/* Generic string table handling.  */
struct Ebl_GStrtab;
struct Ebl_GStrent;

/* Create new string table object in memory.  */
extern struct Ebl_GStrtab *ebl_gstrtabinit (unsigned int width, bool nullstr);

/* Free resources allocated for string table ST.  */
extern void ebl_gstrtabfree (struct Ebl_GStrtab *st);

/* Add string STR (length LEN is != 0) to string table ST.  */
extern struct Ebl_GStrent *ebl_gstrtabadd (struct Ebl_GStrtab *st,
					   const char *str, size_t len);

/* Finalize string table ST and store size and memory location information
   in DATA.  */
extern void ebl_gstrtabfinalize (struct Ebl_GStrtab *st, Elf_Data *data);

/* Get offset in wide char string table for string associated with SE.  */
extern size_t ebl_gstrtaboffset (struct Ebl_GStrent *se);

#endif	/* libebl.h */
