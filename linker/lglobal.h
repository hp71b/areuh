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

#include "common.h"

#define fgetl(lg,fp) fread(&(lg),sizeof(long int),1,fp)

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
    uchar u_label [LBLLEN+2] ;         /* label name (with leading '=') */
    uchar u_file ;                     /* file where declared */
    uchar *u_def ;                     /* definition address */
    struct unres *u_next ;             /* next element */
} ;

extern struct unres *head_unres ;      /* unresolved label list */


/******************************************************************************
  CROSS-REFERENCE LIST
******************************************************************************/

struct xtable
{
    saddr x_pc ;                       /* relative addr. where symb. is used */
    uchar x_file ;                     /* file where symbol is used */
    struct xtable *x_next ;            /* link */
} ;


/******************************************************************************
  LABEL TABLE
******************************************************************************/

struct symbol
{
    uchar s_name [LBLLEN+2] ;          /* label name */
    saddr s_value ;                    /* label value */
    uchar s_file ;                     /* last file where label is declared */
    uchar s_os ;                       /* 1 is O.S. entry point not used */
    struct xtable *s_xref ;            /* head of xref list */
    struct symbol *s_next ;            /* next label */
} ;

extern struct symbol *h_label [] ;     /* pseudo-hash table */


/******************************************************************************
  CODE
******************************************************************************/

extern uchar *code ;

extern uchar *fname[] ;

extern uchar flex [], flisting [] ;
extern FILE *fplex ;
extern saddr pc ;
extern nfile, file ;
extern int cntlist, page_size, xref, passnb, errnb, passbis ;

extern uchar hp71ep [] ;
