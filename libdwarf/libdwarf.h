/* Interface for libdwarf.
   Copyright (C) 2000, 2001, 2002 Red Hat, Inc.
   This file is part of Red Hat elfutils.

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

#ifndef _LIBDWARF_H
#define _LIBDWARF_H 1

#include <libelf.h>
#include <stdint.h>

/* Basic data types.  */

/* Used for boolean values.  */
typedef int Dwarf_Bool;

/* Numeric values of different sizes.  */
typedef uint8_t Dwarf_Small;
typedef uint16_t Dwarf_Half;
typedef uint64_t Dwarf_Unsigned;
typedef int64_t Dwarf_Signed;

/* Offsets in the debugging sections.  */
typedef uint64_t Dwarf_Off;

/* Program counter value in the target object file.  */
typedef uint64_t Dwarf_Addr;

/* Address in the host process.  */
typedef void *Dwarf_Ptr;


/* Location record.  */
typedef struct
  {
    Dwarf_Small lr_atom;		/* Operation */
    Dwarf_Unsigned lr_number;		/* Operand */
    Dwarf_Unsigned lr_number2;		/* Possible second operand */
    Dwarf_Unsigned lr_offset;		/* Offset in location expression */
  } Dwarf_Loc;


/* Location description.  */
typedef struct
   {
     Dwarf_Addr ld_lopc;		/* Beginning of range */
     Dwarf_Addr ld_hipc;		/* End of range */
     Dwarf_Half ld_cents;		/* Number of location records */
     Dwarf_Loc *ld_s;			/* Array of location records */
  } Dwarf_Locdesc;


/* Error handler function.  */
typedef struct Dwarf_Error_s *Dwarf_Error;	/* Forward declaration.  */
typedef void (*Dwarf_Handler) (Dwarf_Error *, Dwarf_Ptr);

/* Descriptor for block of uninterpreted data.  */
typedef struct
  {
    Dwarf_Unsigned bl_len;
    Dwarf_Ptr bl_data;
  } Dwarf_Block;


/* Descriptor for libdwarf session.  */
typedef struct Dwarf_Debug_s *Dwarf_Debug;

/* Descriptor for DWARF DIE.  */
typedef struct Dwarf_Die_s *Dwarf_Die;

/* Descriptor for DWARF attribute list.  */
typedef struct Dwarf_Attribute_s *Dwarf_Attribute;

/* Descriptor for source lines.  */
typedef struct Dwarf_Line_s *Dwarf_Line;

/* Descriptor for global name.  */
typedef struct Dwarf_Global_s *Dwarf_Global;

/* Descriptor for address range.  */
typedef struct Dwarf_Arange_s *Dwarf_Arange;

/* Descriptor for common information entry.  */
typedef struct Dwarf_Cie_s *Dwarf_Cie;

/* Descriptor for frame descriptor entry.  */
typedef struct Dwarf_Fde_s *Dwarf_Fde;

/* Descriptor for abbreviations.  */
typedef struct Dwarf_Abbrev_s *Dwarf_Abbrev;


/* Return values.  */
enum
  {
    DW_DLV_NO_ENTRY = -1,	/* No error, but no entry.  */
    DW_DLV_OK = 0,		/* Success.  */
    DW_DLV_ERROR = 1,		/* Failure.  */
  };


/* Values for ACCESS parameter of 'dwarf_init' and 'dwarf_elf_init'.  */
enum
  {
    DW_DLC_READ = 0,		/* Read-only access.  */
    DW_DLC_WRITE = 1,		/* Write-only access.  */
    DW_DLC_RDWR = 2		/* Read-write access.  */
  };


/* Open file associates with FD for use with the other functions of
   this library.  Set the error handler and the parameter passed.  */
extern int dwarf_init (int fd, Dwarf_Unsigned access,
		       Dwarf_Handler errhand, Dwarf_Ptr errarg,
		       Dwarf_Debug *dbg, Dwarf_Error *errdesc);

/* Similar to `dwarf_init' but instead of a file descriptor of ELF
   descriptor is passed.  */
extern int dwarf_elf_init (Elf *elf, Dwarf_Unsigned access,
			   Dwarf_Handler errhand, Dwarf_Ptr errarg,
			   Dwarf_Debug *dbg, Dwarf_Error *errdesc);

/* Return ELF handle.  */
extern int dwarf_get_elf_init (Dwarf_Debug dbg, Elf **elf,
			       Dwarf_Error *errdesc);

/* Free resources allocated for debug handle.  */
extern int dwarf_finish (Dwarf_Debug dbg, Dwarf_Error *errdesc);


/* Return information about current and find new compile unit header.  */
extern int dwarf_next_cu_header (Dwarf_Debug dbg,
				 Dwarf_Unsigned *cu_header_length,
				 Dwarf_Half *version_stamp,
				 Dwarf_Unsigned *abbrev_offset,
				 Dwarf_Half *address_size,
				 Dwarf_Unsigned *next_cu_header,
				 Dwarf_Error *errdesc);

/* Return sibling of given DIE.  */
extern int dwarf_siblingof (Dwarf_Debug dbg, Dwarf_Die die,
			    Dwarf_Die *return_sub, Dwarf_Error *errdesc);

/* Return child of DIE.  */
extern int dwarf_child (Dwarf_Die die, Dwarf_Die *return_kid,
			Dwarf_Error *errdesc);

/* Return DIE at given offset.  */
extern int dwarf_offdie (Dwarf_Debug dbg, Dwarf_Off offset,
			 Dwarf_Die *return_die, Dwarf_Error *errdesc);


/* Return tag of DIE.  */
extern int dwarf_tag (Dwarf_Die die, Dwarf_Half *tagval, Dwarf_Error *errdesc);

/* Return offset of DIE in .debug_info section.  */
extern int dwarf_dieoffset (Dwarf_Die die, Dwarf_Off *return_offset,
			    Dwarf_Error *errdesc);

/* Return offset of DIE in compile unit data.  */
extern int dwarf_die_CU_offset (Dwarf_Die die, Dwarf_Off *return_offset,
				Dwarf_Error *errdesc);

/* Return name attribute of DIE.  */
extern int dwarf_diename (Dwarf_Die die, char **return_name,
			  Dwarf_Error *errdesc);

/* Return list of attributes for DIE.  */
extern int dwarf_attrlist (Dwarf_Die die, Dwarf_Attribute **attrbuf,
			   Dwarf_Signed *attrcount, Dwarf_Error *errdesc);

/* Determine whether DIE has attribute specified of given type.  */
extern int dwarf_hasattr (Dwarf_Die die, Dwarf_Half attr,
			  Dwarf_Bool *return_bool, Dwarf_Error *errdesc);

/* Return DIE attribute with specified of given type.  */
extern int dwarf_attr (Dwarf_Die die, Dwarf_Half attr,
		       Dwarf_Attribute *return_attr, Dwarf_Error *errdesc);

/* Return low program counter value associated with die.  */
extern int dwarf_lowpc (Dwarf_Die die, Dwarf_Addr *return_lowpc,
			Dwarf_Error *errdesc);

/* Return high program counter value associated with die.  */
extern int dwarf_highpc (Dwarf_Die die, Dwarf_Addr *return_lowpc,
			 Dwarf_Error *errdesc);

/* Return byte size value associated with die.  */
extern int dwarf_bytesize (Dwarf_Die die, Dwarf_Unsigned *return_size,
			   Dwarf_Error *errdesc);

/* Return bit size value associated with die.  */
extern int dwarf_bitsize (Dwarf_Die die, Dwarf_Unsigned *return_size,
			  Dwarf_Error *errdesc);

/* Return bit offset value associated with die.  */
extern int dwarf_bitoffset (Dwarf_Die die, Dwarf_Unsigned *return_size,
			    Dwarf_Error *errdesc);

/* Return source language associated with die.  */
extern int dwarf_srclang (Dwarf_Die die, Dwarf_Unsigned *return_lang,
			  Dwarf_Error *errdesc);

/* Return source language associated with die.  */
extern int dwarf_arrayorder (Dwarf_Die die, Dwarf_Unsigned *return_order,
			     Dwarf_Error *errdesc);


/* Determine whether attribute has given form.  */
extern int dwarf_hasform (Dwarf_Attribute attr, Dwarf_Half form,
			  Dwarf_Bool *return_hasform, Dwarf_Error *errdesc);

/* Return form of attribute.  */
extern int dwarf_whatform (Dwarf_Attribute attr, Dwarf_Half *return_form,
			   Dwarf_Error *errdesc);

/* Return code of attribute.  */
extern int dwarf_whatattr (Dwarf_Attribute attr, Dwarf_Half *return_attr,
			   Dwarf_Error *errdesc);

/*  Return compile-unit relative offset of reference associated with form.  */
extern int dwarf_formref (Dwarf_Attribute attr, Dwarf_Off *return_offset,
			  Dwarf_Error *errdesc);

/*  Return .debug_info section global offset of reference associated
    with form.  */
extern int dwarf_global_formref (Dwarf_Attribute attr,
				 Dwarf_Off *return_offset,
				 Dwarf_Error *errdesc);

/* Return address represented by attribute.  */
extern int dwarf_formaddr (Dwarf_Attribute attr, Dwarf_Addr *return_addr,
			   Dwarf_Error *errdesc);

/* Return flag represented by attribute.  */
extern int dwarf_formflag (Dwarf_Attribute attr, Dwarf_Bool *return_bool,
			   Dwarf_Error *errdesc);

/* Return unsigned constant represented by attribute.  */
extern int dwarf_formudata (Dwarf_Attribute attr, Dwarf_Unsigned *return_uval,
			    Dwarf_Error *errdesc);

/* Return signed constant represented by attribute.  */
extern int dwarf_formsdata (Dwarf_Attribute attr, Dwarf_Signed *return_uval,
			    Dwarf_Error *errdesc);

/* Return block of uninterpreted data represented by attribute.  */
extern int dwarf_formblock (Dwarf_Attribute attr, Dwarf_Block **return_block,
			    Dwarf_Error *errdesc);

/* Return string represented by attribute.  */
extern int dwarf_formstring (Dwarf_Attribute attr, char **return_string,
			     Dwarf_Error *errdesc);

/* Return location expression list.  */
extern int dwarf_loclist (Dwarf_Attribute attr, Dwarf_Locdesc **llbuf,
			  Dwarf_Signed *listlen, Dwarf_Error *errdesc);


/* Return source lines of compilation unit.  */
extern int dwarf_srclines (Dwarf_Die die, Dwarf_Line **linebuf,
			   Dwarf_Signed *linecount, Dwarf_Error *errdesc);

/* Return files used in compilation unit.  */
extern int dwarf_srcfiles (Dwarf_Die die, char ***srcfiles,
			   Dwarf_Signed *srcfilecount, Dwarf_Error *errdesc);

/* Determine whether line is the beginning of a statement.  */
extern int dwarf_linebeginstatement (Dwarf_Line line, Dwarf_Bool *return_bool,
				     Dwarf_Error *errdesc);

/* Determine whether line is marked as ending a text sequence.  */
extern int dwarf_lineendsequence (Dwarf_Line line, Dwarf_Bool *return_bool,
				  Dwarf_Error *errdesc);

/* Return source statement line number.  */
extern int dwarf_lineno (Dwarf_Line line, Dwarf_Unsigned *return_lineno,
			 Dwarf_Error *errdesc);

/* Return address associate with line.  */
extern int dwarf_lineaddr (Dwarf_Line line, Dwarf_Addr *return_lineaddr,
			   Dwarf_Error *errdesc);

/* Return column at which the statement begins.  */
extern int dwarf_lineoff (Dwarf_Line line, Dwarf_Signed *return_lineoff,
			  Dwarf_Error *errdesc);

/* Return source file for line.  */
extern int dwarf_linesrc (Dwarf_Line line, char **return_linesrc,
			  Dwarf_Error *errdesc);

/* Determine whether line is marked as beginning a basic block.  */
extern int dwarf_lineblock (Dwarf_Line line, Dwarf_Bool *return_bool,
			    Dwarf_Error *errdesc);

/* Determine whether line is marked as ending the prologue.  */
extern int dwarf_lineprologueend (Dwarf_Line line, Dwarf_Bool *return_bool,
				  Dwarf_Error *errdesc);

/* Determine whether line is marked as beginning the epilogue.  */
extern int dwarf_lineepiloguebegin (Dwarf_Line line, Dwarf_Bool *return_bool,
				    Dwarf_Error *errdesc);


/* Return list of global definitions.  */
extern int dwarf_get_globals (Dwarf_Debug dbg, Dwarf_Global **globals,
			      Dwarf_Signed *return_count,
			      Dwarf_Error *errdesc);

/* Return name for global definition.  */
extern int dwarf_globname (Dwarf_Global global, char **return_name,
			   Dwarf_Error *errdesc);

/* Return DIE offset for global definition.  */
extern int dwarf_global_die_offset (Dwarf_Global global,
				    Dwarf_Off *return_offset,
				    Dwarf_Error *errdesc);

/* Return offset of header of compile unit containing the global definition. */
extern int dwarf_global_cu_offset (Dwarf_Global global,
				   Dwarf_Off *return_offset,
				   Dwarf_Error *errdesc);

/* Return name, DIE offset, and offset of the compile unit DIE for the
   global definition.  */
extern int dwarf_global_name_offsets (Dwarf_Global global, char **return_name,
				      Dwarf_Off *die_offset,
				      Dwarf_Off *cu_offset,
				      Dwarf_Error *errdesc);


/* Find start of macro value.  */
extern char *dwarf_find_macro_value_start (char *macro_string);


/* Return string from debug string section.  */
extern int dwarf_get_str (Dwarf_Debug dbg, Dwarf_Off offset, char **string,
			  Dwarf_Signed *returned_str_len,
			  Dwarf_Error *errdesc);


/* Return list address ranges.  */
extern int dwarf_get_aranges (Dwarf_Debug dbg, Dwarf_Arange **aranges,
			      Dwarf_Signed *return_count,
			      Dwarf_Error *errdesc);

/* Find matching range for address.  */
extern int dwarf_get_arange (Dwarf_Arange *aranges,
			     Dwarf_Unsigned arange_count, Dwarf_Addr address,
			     Dwarf_Arange *return_arange,
			     Dwarf_Error *errdesc);

/* Return offset of compile unit DIE containing the range.  */
extern int dwarf_get_cu_die_offset (Dwarf_Arange arange,
				    Dwarf_Off *return_offset,
				    Dwarf_Error *errdesc);

/* Return start, length, and CU DIE offset of range.  */
extern int dwarf_get_arange_info (Dwarf_Arange arange, Dwarf_Addr *start,
				  Dwarf_Unsigned *length,
				  Dwarf_Off *cu_die_offset,
				  Dwarf_Error *errdesc);


/* Frame descriptor handling.  */

/* Get frame descriptions.  GCC version using .eh_frame.  */
extern int dwarf_get_fde_list_eh (Dwarf_Debug dbg, Dwarf_Cie **cie_data,
				  Dwarf_Signed *cie_element_count,
				  Dwarf_Fde **fde_data,
				  Dwarf_Signed *fde_element_count,
				  Dwarf_Error *errdesc);

/* Get CIE of FDE.  */
extern int dwarf_get_cie_of_fde (Dwarf_Fde fde, Dwarf_Cie *return_cie,
				 Dwarf_Error *errdesc);

/* Get information about the function range.  */
extern int dwarf_get_fde_range (Dwarf_Fde fde, Dwarf_Addr *low_pc,
				Dwarf_Unsigned *func_length,
				Dwarf_Ptr *fde_bytes,
				Dwarf_Unsigned *fde_byte_length,
				Dwarf_Off *cie_offset, Dwarf_Signed *cie_index,
				Dwarf_Off *fde_offset, Dwarf_Error *errdesc);

/* Get information about CIE.  */
extern int dwarf_get_cie_info (Dwarf_Cie cie, Dwarf_Unsigned *bytes_in_cie,
			       Dwarf_Small *version, char **augmenter,
			       Dwarf_Unsigned *code_alignment_factor,
			       Dwarf_Signed *data_alignment_factor,
			       Dwarf_Half *return_address_register,
			       Dwarf_Ptr *initial_instructions,
			       Dwarf_Unsigned *initial_instructions_length,
			       Dwarf_Error *errdesc);

/* Get frame construction instructions of FDE.  */
extern int dwarf_get_fde_instr_bytes (Dwarf_Fde fde, Dwarf_Ptr *outinstrs,
				      Dwarf_Unsigned *outlen,
				      Dwarf_Error *errdesc);

/* Get nth frame descriptions.  */
extern int dwarf_get_fde_n (Dwarf_Fde *fde_data, Dwarf_Unsigned fde_index,
			    Dwarf_Fde *returned_fde, Dwarf_Error *errdesc);

/* Find FDE for given address.  */
extern int dwarf_get_fde_at_pc (Dwarf_Fde *fde_data, Dwarf_Addr pc_of_interest,
				Dwarf_Fde *returned_fde, Dwarf_Addr *lopc,
				Dwarf_Addr *hipc, Dwarf_Error *errdesc);


/* Return location list entry.  */
extern int dwarf_get_loclist_entry (Dwarf_Debug dbg, Dwarf_Unsigned offset,
				    Dwarf_Addr *hipc_offset,
				    Dwarf_Addr *lopc_offset, Dwarf_Ptr *data,
				    Dwarf_Unsigned *entry_len,
				    Dwarf_Unsigned *next_entry,
				    Dwarf_Error *errdesc);


/* Get abbreviation record.  */
extern int dwarf_get_abbrev (Dwarf_Debug dbg,
			     Dwarf_Unsigned offset,
			     Dwarf_Abbrev *returned_abbrev,
			     Dwarf_Unsigned *length,
			     Dwarf_Unsigned *attr_count, Dwarf_Error *errdesc);

/* Get tag of abbreviation record.  */
extern int dwarf_get_abbrev_tag (Dwarf_Abbrev abbrev, Dwarf_Half *return_tag,
				 Dwarf_Error *errdesc);

/* Get code of abbreviation record.  */
extern int dwarf_get_abbrev_code (Dwarf_Abbrev abbrev,
				  Dwarf_Unsigned *return_code,
				  Dwarf_Error *errdesc);

/* Get children flag of abbreviation record.  */
extern int dwarf_get_abbrev_children_flag (Dwarf_Abbrev abbrev,
					   Dwarf_Signed *return_flag,
					   Dwarf_Error *errdesc);

/* Get attribute from abbreviation record.  */
extern int dwarf_get_abbrev_entry (Dwarf_Abbrev abbrev, Dwarf_Signed idx,
				   Dwarf_Half *attr_num, Dwarf_Signed *form,
				   Dwarf_Off *offset, Dwarf_Error *errdesc);


/* Memory handling.  */

/* Values for ALLOC_TYPE parameter of 'dwarf_dealloc'.  */
enum
  {
    DW_DLA_NONE = 0,
    DW_DLA_STRING,		/* char* */
    DW_DLA_LOC,			/* Dwarf_Loc */
    DW_DLA_LOCDESC,		/* Dwarf_Locdesc */
    DW_DLA_ELLIST,		/* Dwarf_Ellist */
    DW_DLA_BOUNDS,		/* Dwarf_Bounds */
    DW_DLA_BLOCK,		/* Dwarf_Block */
    DW_DLA_DEBUG,		/* Dwarf_Debug */
    DW_DLA_DIE,			/* Dwarf_Die */
    DW_DLA_LINE,		/* Dwarf_Line */
    DW_DLA_ATTR,		/* Dwarf_Attribute */
    DW_DLA_TYPE,		/* Dwarf_Type */
    DW_DLA_SUBSCR,		/* Dwarf_Subscr */
    DW_DLA_GLOBAL,		/* Dwarf_Global */
    DW_DLA_ERROR,		/* Dwarf_Error */
    DW_DLA_LIST,		/* a list */
    DW_DLA_LINEBUF,		/* Dwarf_Line* */
    DW_DLA_ARANGE,		/* Dwarf_Arange */
    DW_DLA_ABBREV,		/* Dwarf_Abbrev */
    DW_DLA_FRAME_OP,		/* Dwarf_Frame_Op */
    DW_DLA_CIE,			/* Dwarf_Cie */
    DW_DLA_FDE,			/* Dwarf_Fde */
    DW_DLA_LOC_BLOCK,		/* Dwarf_Loc Block */
    DW_DLA_FRAME_BLOCK,		/* Dwarf_Frame Block */
    DW_DLA_FUNC,		/* Dwarf_Func */
    DW_DLA_TYPENAME,		/* Dwarf_Type */
    DW_DLA_VAR,			/* Dwarf_Var */
    DW_DLA_WEAK,		/* Dwarf_Weak */
    DW_DLA_ADDR,		/* Dwarf_Addr sized entries */
  };

/* Deallocate memory.  */
extern void dwarf_dealloc (Dwarf_Debug dbg, Dwarf_Ptr space,
			   Dwarf_Unsigned alloc_type);


/* Determine size of address of the binary.  */
extern int dwarf_get_address_size (Dwarf_Debug dbg, Dwarf_Half *addr_size,
				   Dwarf_Error *errdesc);


/* Return error number.  */
extern Dwarf_Unsigned dwarf_errno (Dwarf_Error errdesc);

/* Return string corresponding to error.  */
extern const char *dwarf_errmsg (Dwarf_Error errdesc);


/* Set new error handler.  */
extern Dwarf_Handler dwarf_seterrhand (Dwarf_Debug dbg, Dwarf_Handler errhand);

/* Set new error handler argument.  */
extern Dwarf_Ptr dwarf_seterrarg (Dwarf_Debug dbg, Dwarf_Ptr errarg);

#endif	/* libdwarf.h */
