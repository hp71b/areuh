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

#include <stdio.h>


#define MAXLEN 256
#define EOL    '\0'
#define LBLLEN 12

/******************************************************************************
 TYPE DEFINITIONS
******************************************************************************/

typedef long int saddr ;	      /* Saturne address. At least 32 bits   */
typedef unsigned char uchar ;	      /* unsigned characters, for accents    */
typedef short int sint ;	      /* 16 bits */

/******************************************************************************
 MAGIC NUMBERS
******************************************************************************/

#define  AL_MAGIC 0x1b080100	      /* ... areuh_lex */
#define  AO_MAGIC 0x1b0d0100	      /* ... areuh_output of assembler */
#define ALF_MAGIC 0x1b080100	      /* first version of object file format */
#define AOF_MAGIC 0x1b0d0100	      /* first version of object file format */


/******************************************************************************
 USAGE TYPES IN .ao FILES
******************************************************************************/

#define XABSL 0x10     /* absolute reference */
#define XABSO 0x20     /* absolute reference, with one bias */
#define XRGTO 0x40     /* relative reference, goto type */
#define XRGSB 0x80     /* relative reference, gosub type */


/******************************************************************************
 LABEL VALUES IN .ao FILES
******************************************************************************/

#define LBL_UDF -1L
    /* label not declared (implicit or explicit) <=> only used */
#define LBL_IVL -2L
    /* invalid label (invalid expression for an EQU during pass one) */
#define LBL_EXT -3L
    /* external label not defined here (or yet) */
#define LBL_XEQ -4L
    /* global label which is defined here, with an external reference */
#define LBL_SEQ -5L
    /* local label which is defined with an external reference */


/******************************************************************************
 LABEL TYPES IN .ao FILES
******************************************************************************/

#define LUDF 0
    /* undefined type (external label) */
#define LABS 1
    /* absolute local label (declared explicitly, with constants) */
#define LREL 2
    /* relative local label (declared implicitly, or explicitly with at least
       one relative) */


/******************************************************************************
 VALUES RETURNED BY EXPRESSION EVALUATOR
******************************************************************************/

#define EXP_ERR -1L
#define EXP_EXT -2L

extern int relabs ;
extern uchar extexp[] ;

#include "err.h"
extern void error () ;

#define hex5(str,val) format_hex (str, val, 5) ;
#define hex6(str,val) format_hex (str, val, 6) ;

/******************************************************************************
 MACHINE DEPENDANCIES
******************************************************************************/

#define HPUX		     1
#define ATARI_LATTICE	     0
#define PC_MSC		     0		/* PC (beuark, Microsoft C) */

#if HPUX
extern void format_time(), load_file() ;
#define HP71EP "hp71.ep"
#define RAO_MODE "r"
#define WAO_MODE "w"
#define skip(fp)
#endif

#if ATARI_LATTICE
extern void format_time(), load_file() ;
#define HP71EP "hp71.ep"
#define RAO_MODE "rb"
#define WAO_MODE "wb"
extern char skipvar ;
#define skip(fp) fread(&skipvar,1,1,fp) ;
#endif

#if PC_MSC
extern void format_time(), load_file() ;
#define HP71EP "hp71.ep"
#define RAO_MODE "rb"
#define WAO_MODE "wb"
extern char skipvar ;
#define skip(fp) fread(&skipvar,1,1,fp) ;
#endif
