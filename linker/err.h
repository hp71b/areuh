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

/******************************************************************************

                           AREUH ASSEMBLER / LINKER


                          ERROR NUMBERS DECLARATIONS

******************************************************************************/




/* FATAL ERRORS */

#define ERROPN  -1                                       /* A L */
    /* system error opening file */
#define ERRCLO  -2                                       /* A L */
    /* system error closing file */
#define ERRREW  -3                                       /* A */
    /* system error on file at start of pass two */
#define ERRWRT  -4                                       /* A L */
    /* system error writing file */
#define ERRRD   -5                                       /* A L */
    /* system error reading file */
#define ERRMEM  -6                                       /* A L */
    /* not enough memory */

#define ERRLEX  -10                                      /* A */
    /* invalid macro pseudo-op LEX or BIN */
#define ERRPGS  -11                                      /* A */
    /* invalid page size */
#define ERRFLN  -12                                      /* A */
    /* restricted label FiLeNd exists */
#define ERRIFL  -13                                      /* A L */
    /* invalid file name */
#define ERRIMO  -14                                      /* A */
    /* invalid macro-op xx in modular assembling */
#define ERRVMD  -15                                      /* A */
    /* value must be defined for xx */

#define ERRUSA  -20                                      /* A L */
    /* usage: ass [ [-l] source_file ] */
    /* usage: ald ... */

#define ERRNOA  -30                                      /* L */
    /* file not output from aas */
#define ERRICV  -31                                      /* L */
    /* incompatible version */



/* NON FATAL ERRORS */

#define WRNEQU  10                                       /* A */
    /* cannot resolve equate */
#define WRNDUP  11                                       /* A L */
    /* duplicate label */
#define WRNLBL  12                                       /* A */
    /* illegal label */
#define WRNULB  13                                       /* A */
    /* unrecognized label */
#define WRNURF  14                                       /* L */
    /* unresolved reference */
#define WRNURL  15                                       /* L */
    /* unresolved label */

#define WRNEXP  20                                       /* A */
    /* illegal expression */
#define WRNASC  21                                       /* A */
    /* illegal ascii constant */
#define WRNPAR  22                                       /* A */
    /* mismatched parenthesis */
#define WRNIHX  23                                       /* A */
    /* illegal hexadecimal constant */
#define WRNNUL  24                                       /* A */
    /* null divisor */
#define WRNIXP  25                                       /* A */
    /* illegal exponentiation */
#define WRNIBC  26                                       /* A */
    /* illegal binary constant */
#define WRNENA  27                                       /* A */
    /* external references not allowed */

#define WRNYES  30                                       /* A */
    /* GOYES or RTNYES required */
#define WRNIDP  31                                       /* A */
    /* illegal dp arithmetic value */
#define WRNIPP  32                                       /* A */
    /* illegal pointer position */
#define WRNISB  33                                       /* A */
    /* illegal status bit */
#define WRNTFR  34                                       /* A */
    /* illegal transfer value */
#define WRNIWS  35                                       /* A */
    /* illegal word select */
#define WRNLST  36                                       /* A */
    /* invalid LIST argument */
#define WRNJVL  37                                       /* A L */
    /* jump or value too large */
#define WRNMLB  38                                       /* A */
    /* missing label */
#define WRNTST  39                                       /* A */
    /* needs previous test instruction */
#define WRNNHX  40                                       /* A */
    /* non hexadecimal digits present */
#define WRNTMA  41                                       /* A */
    /* too many ascii characters present */
#define WRNTMH  42                                       /* A */
    /* too many hexadecimal digits present */
#define WRNOPC  43                                       /* A */
    /* unknown opcode */

#define WRNIIF  50                                       /* A */
    /* invalid conditional structure */

extern void error() ;
