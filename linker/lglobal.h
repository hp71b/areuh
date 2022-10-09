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

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"

/******************************************************************************
  MODULE TABLE
******************************************************************************/

#define MAXMOD 80                      /* maximum number of modules */

struct module                          /* module descriptor */
{
    long int m_part3 ;                 /* address of utilisations part */
    saddr m_ad ;                       /* Saturn address of start of code */
} ;

extern struct module tmodule [] ;      /* module descriptors table */


/******************************************************************************
  UNRESOLVED LABELS LIST
******************************************************************************/

struct unres                           /* unresolved label element */
{
    char u_label [LBLLEN+2] ;          /* label name (with leading '=') */
    char u_file ;                      /* file where declared */
    char *u_def ;                      /* definition address */
    struct unres *u_next ;             /* next element */
} ;

extern struct unres *head_unres ;      /* unresolved label list */


/******************************************************************************
  CROSS-REFERENCE LIST
******************************************************************************/

struct xtable
{
    saddr x_pc ;                       /* relative addr. where symb. is used */
    char x_file ;                      /* file where symbol is used */
    struct xtable *x_next ;            /* link */
} ;


/******************************************************************************
  LABEL TABLE
******************************************************************************/

struct symbol
{
    char s_name [LBLLEN+2] ;           /* label name */
    saddr s_value ;                    /* label value */
    char s_file ;                      /* last file where label is declared */
    char s_os ;                        /* 1 is O.S. entry point not used */
    struct xtable *s_xref ;            /* head of xref list */
    struct symbol *s_next ;            /* next label */
} ;

extern struct symbol *h_label [] ;     /* pseudo-hash table */


/******************************************************************************
  CODE
******************************************************************************/

extern char *code ;

extern char *fname[] ;

extern char flex [], flisting [] ;
extern FILE *fplex ;
extern saddr pc ;
extern int nfile, file ;
extern int cntlist, page_size, xref, passnb, errnb, passbis ;

extern char hp71ep [] ;

/******************************************************************************
  FUNCTIONS
******************************************************************************/

// exp.c
extern saddr calc_expression (char *) ;

// lerror.c
extern void error (int errno, char *msg);

// linit.c
extern void init (void) ;
extern void between (void) ;
extern void term (void) ;

// llist.c
extern void l_init (void) ;
extern void l_print (char *);
extern void l_flush (void);
extern void report (void) ;

// lmain.c

// lpass.c
extern void pass1 (void) ;
extern void pass2 (void) ;

// lutil.c
extern void add_label (char *, saddr, int);
extern void add_unres (char *, char *);
extern void read_usage (int *, char *, FILE *);
extern void resolve_usage (saddr, char *, char *) ;
extern saddr symbol_value(char *) ;
extern char *memoire (int) ;
extern void format_hex (char *, saddr, int) ;

// mdep.c
extern void dfl_extension (char *, char *, char *);
extern void look_obj (char *, char *);
