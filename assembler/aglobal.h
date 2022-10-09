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
    char s_name[LBLLEN+2] ;              /* symbol name */
    saddr s_value ;                       /* symbol value */
    char s_type ;                        /* type (LABS, LREL or LUDF) */
    char s_os ;                          /* 1 if O.S. entry point not used */
    char *s_def ;                        /* external definition if any */
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
    char m_class ;                       /* opcode class (0..43) */
    char *m_text ;                        /* mnemonic */
    char m_flag ;                        /* flags */
    char m_len ;                         /* generated code length */
    char *m_code ;                        /* generated code */
    char m_a ;                           /* variable a */
    char m_b ;                           /* variable b */
} ;

extern struct mnemo_desc mnemo_table[] ;  /* the opcode table */

extern int h_opcode[] ;                   /* hash table for opcodes */


/******************************************************************************
  EXTERNAL REFERENCES NOT RESOLVED
******************************************************************************/

struct xused
{
    char u_characteristic ;              /* type and number of nibs */
    saddr u_pc ;                          /* relative address  */
    char *u_expression ;                 /* expression representation */
    struct xused *u_next ;                /* next element in the xused queue */
} ;

extern struct xused *headxu ;             /* head of previous queue */


/******************************************************************************
  GLOBAL VARIABLES
******************************************************************************/

/* files */

extern char fsource[], flisting[], fobject[] ; /* file names */
extern FILE *fd_s, *fd_l, *fd_o ;         /* and associated streams */

/* listing management */

extern int cntlist, cntlist_ref ;         /* type of output (no/stdout/file) */
extern int page_size ;                    /* size of listing page */
extern char l_title[], l_stitle[] ;      /* var. used by (S)TITLE opcodes */
extern int errcnt, error_this_line ;      /* error management */
extern int print_this_line ;              /* 0 if line must not be printed */
extern int xref ;                         /* 1 if cross reference table */
extern int running ;                      /* 0 if END opcode reached */
extern int passnb ;                       /* current pass number */
extern int linenb ;                       /* current line number */
           

/* code management */

extern saddr pc ;                         /* current program counter */
extern int gen_len ;                      /* current opcode code length */
extern char gen_code[] ;                 /* current opcode code */
extern int prev_test ;                    /* previous opcode was a test */
extern int exec, in_if, in_else ;         /* conditionnal assembly */


/* informations to linker */

extern char *xlabel ;                     /* label synonym */
extern char extexp[] ;                    /* external expression */
extern int modular ;                      /* 0 if LEX encoutered */

/******************************************************************************
  FUNCTIONS
******************************************************************************/

// aerror.c
extern void error(int errnb, char *msg);

// ainput.c
extern int read_line(FILE *fd, char line[]) ;
extern void parse_line(char *line, char *label, char *mnemo, char *modif);

// alabel.c
extern void s_init(void) ;
extern void ps_label (char *label, char *mnemo, char *modif);
extern struct symbol *add_label(char *label, saddr val, char *exp, char type, char os);
extern saddr symbol_value(char *label) ;

// alist.c
extern void l_flush(void) ;
extern void l_init(void) ;
extern void l_new_page(int flag) ;
extern void l_print (saddr address, char *code, char *msg, int flags);

// amain.c

// amnemo.c
extern void ps_mnemo (char *line, char *modif, struct mnemo_desc *ad);
extern struct mnemo_desc *find_mnemo(char *mnemo) ;
extern char hex (int digit);
extern int dec (char digit);

// aobj.c
extern void o_init(void) ;
extern void o_print(char *str, int len) ;
extern void dump_linker_infos (void) ;

// aopc1.c
extern void regarith (struct mnemo_desc *ad, char *modif);
extern void reglogic (char *modif);
extern void regtest(struct mnemo_desc *ad, char *modif);
extern void branches (struct mnemo_desc *ad, char *modif);
extern void ptrtest (char *modif);
extern void rtnyes(void) ;
extern void stattest (char *modif);
extern void setptr (char *modif);
extern void setstat (char *modif);
extern void dparith (char *modif);
extern void datatrans (char *modif);
extern void nibhex (char *modif);
extern void lchex (char *modif);
extern void dxhex (char *modif);
extern int ascii_len (char *modif);
extern void nibasc (char *modif);
extern void lcasc (char *modif);

// aopc2.c
extern void bss (char *modif);
extern void eject (void);
extern void endx (void);
extern void list (char *modif);
extern void title (char *modif);
extern void stitle (char *modif);
extern void lex (char *modif, char *line);
extern void id (char *modif, char *line);
extern void msg (char *modif, char *line);
extern void poll (char *modif, char *line);
extern void entryx (char *modif, char *line);
extern void charx (char *modif, char *line);
extern void key (char *modif, char *line);
extern void token (char *modif, char *line);
extern void bin (char *modif, char *line);
extern void chain (char *modif, char *line);
extern void endtxt (char *line);
extern void endifx (void);
extern void rdsymb (char *modif);
extern void elsex (void);
extern void ifx (char *modif);

// apass.c
extern void pass(void) ;
extern void ps_line (char *line);

// areport.c
extern void print_ref(void) ;

// autil.c
extern void init(void) ;
extern void between(void) ;
extern void term(void) ;
extern char *memoire(int size) ;
extern void uprc (char *str) ;
extern void format_hex (char *str, saddr val, int dig);

// exp.c
extern saddr calc_expression(char *exp) ;

// mdep.c
extern void look_obj (char *fname, char *dfl);
extern void dfl_extension (char* obj, char *src, char *ext) ;
extern void format_time (char *str) ;
extern void load_file (char *file) ;

// tabgrp.c

// tabopc.c
