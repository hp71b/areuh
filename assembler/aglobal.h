/*
 * Authors :
 *   Pierre DAVID (pda@masi.ibp.fr or pda@frunip62.bitnet)
 *   Janick TAILLANDIER
 *
 * This program can be freely used or distributed as long as this
 * note is kept.
 *
 * This program is provided "as is".
 */

#include "common.h"                       /* common to assembler & linker */

/* flag for listing output */

#define F_PC 0x1                          /* print pc */
#define F_LN 0x2                          /* print line number */
#define F_GC 0x4                          /* print generated code */
#define F_TL 0x8                          /* print text line */


/******************************************************************************
  CROSS REFERENCE LIST
******************************************************************************/

struct xtable
{
    sint x_line ;                         /* line where symbol is used */
    struct xtable *x_next, *x_prev ;      /* link */
} ;


/******************************************************************************
  SYMBOL TABLE
******************************************************************************/

struct symbol                             /* one symbol */
{
    uchar s_name[LBLLEN+2] ;              /* symbol name */
    saddr s_value ;                       /* symbol value */
    uchar s_type ;                        /* type (LABS, LREL or LUDF) */
    uchar s_os ;                          /* 1 if O.S. entry point not used */
    uchar *s_def ;                        /* external definition if any */
    sint s_decl ;                         /* line where declared */
    struct xtable *s_xref ;               /* head of xref list */
    struct symbol *s_next ;               /* next symbol */
} ;

extern struct symbol *h_label[] ;         /* speed table for fast access */


/******************************************************************************
  OPCODE TABLE
******************************************************************************/

struct mnemo_desc                         /* one table element */
{
    uchar m_class ;                       /* opcode class (0..43) */
    char *m_text ;                        /* mnemonic */
    uchar m_flag ;                        /* flags */
    uchar m_len ;                         /* generated code length */
    char *m_code ;                        /* generated code */
    uchar m_a ;                           /* variable a */
    uchar m_b ;                           /* variable b */
} ;

extern struct mnemo_desc mnemo_table[] ;  /* the opcode table */

extern int h_opcode[] ;                   /* hash table for opcodes */


/******************************************************************************
  EXTERNAL REFERENCES NOT RESOLVED
******************************************************************************/

struct xused
{
    uchar u_characteristic ;              /* type and number of nibs */
    saddr u_pc ;                          /* relative address  */
    uchar *u_expression ;                 /* expression representation */
    struct xused *u_next ;                /* next element in the xused queue */
} ;

extern struct xused *headxu ;             /* head of previous queue */


/******************************************************************************
  GLOBAL VARIABLES
******************************************************************************/

/* files */

extern uchar fsource[], flisting[], fobject[] ; /* file names */
extern FILE *fd_s, *fd_l, *fd_o ;         /* and associated streams */

/* listing management */

extern int cntlist, cntlist_ref ;         /* type of output (no/stdout/file) */
extern int page_size ;                    /* size of listing page */
extern uchar l_title[], l_stitle[] ;      /* var. used by (S)TITLE opcodes */
extern int errcnt, error_this_line ;      /* error management */
extern int print_this_line ;              /* 0 if line must not be printed */
extern int xref ;                         /* 1 if cross reference table */
extern int running ;                      /* 0 if END opcode reached */
extern int passnb ;                       /* current pass number */
extern int linenb ;                       /* current line number */
           

/* code management */

extern saddr pc ;                         /* current program counter */
extern int gen_len ;                      /* current opcode code length */
extern uchar gen_code[] ;                 /* current opcode code */
extern int prev_test ;                    /* previous opcode was a test */
extern int exec, in_if, in_else ;         /* conditionnal assembly */


/* informations to linker */

extern uchar *xlabel ;                    /* label synonym */
extern uchar extexp[] ;                   /* external expression */
extern int modular ;                      /* 0 if LEX encoutered */
