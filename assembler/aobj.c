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

                                 AREUH ASSEMBLER

                              OBJECT FILE UTILITIES


o_init (), o_print (), dump_linker_infos ()

******************************************************************************/

#include "aglobal.h"

#define fputl(lg,fd)   fwrite((char *)(&(lg)),sizeof(long int),1,fd)

long int zero = 0L ;
long int magic ;


/******************************************************************************

                                    O_INIT


synopsis : void o_init ()
description : open object file and, if modular assembly, mark information for
  linker (address of second part of file, and size of code in nibs).

******************************************************************************/

void o_init ()
{
    uchar dfl [MAXLEN+1] ;

    if (modular) dfl_extension (dfl, fsource, "ao") ;
    else strcpy (dfl, "lex") ;

    look_obj (fobject, dfl) ;                        /* ensure name validity */
        
    if (modular) fd_o = fopen (fobject, WAO_MODE) ;     /* untranslated mode */
    else         fd_o =fopen (fobject, "w") ;           /* text mode */
    if (fd_o==NULL)
        error (ERROPN, fobject) ;      /* error opening file */
    if (modular)
    {
        magic = AO_MAGIC ;
        fputl (magic, fd_o) ;
        fputl (zero, fd_o) ;
        fputl (zero, fd_o) ;
    }
    else
    {
        magic = AL_MAGIC ;
        fputl (magic, fd_o) ;
    }
    if (ferror (fd_o)) error (ERRWRT, fobject) ;
}


/******************************************************************************

                                   O_PRINT


synopsis : void o_print (str, len)
           uchar *str
           int len
description :  burp

******************************************************************************/

void o_print (str, len)
uchar *str ;
int len ;
{
    int i ;
    
    if (len)
    {
        if (len>18) for (i=0; i<len; i++) putc ((int) '0', fd_o) ;
        else        for (i=0; i<len; i++) putc ((int) str[i], fd_o) ;
    }
    if (ferror (fd_o)) error (ERRWRT, fobject) ;
}


/******************************************************************************

                               DUMP_LINKER_INFOS


synopsis : void dump_linker_infos ()
description : append to objet file external public definitions (symbolic or
  numeric) and external references usages.

******************************************************************************/

void dump_linker_infos ()
{
    int i ;                       /* index in h_label table */
    struct symbol *pl ;           /* points into a label list */
    long int p2, p3 ;             /* addresses of second and third parts */
    long int nl = 0, nu = 0;      /* # labels (part 2), # used (part 3) */
    long int l ;                  /* auxiliar */
    struct xused *h ;

    p2 = ftell (fd_o) ;           /* begin of second part */
    fputl (zero, fd_o) ;          /* marker for address of third part */
    fputl (zero, fd_o) ;          /* number of labels in second  part */
    for (i=0; i<256; i++)
    {
        pl = h_label[i]->s_next ;
        while (pl)
        {
            if ((pl->s_name[0]=='=')&&(pl->s_value!=LBL_EXT)
                                    &&(pl->s_value!=LBL_IVL))
            {
                nl++ ;
                fprintf (fd_o, "%s\n", pl->s_name) ;
                fputl (pl->s_value, fd_o) ;
                if (pl->s_value<0L)
                    fprintf (fd_o, "%s\n", pl->s_def) ;
                else fputc (pl->s_type, fd_o) ;
            }
            pl = pl->s_next ;
        }
    }
    p3 = ftell (fd_o) ;           /* begin of third part */
    fputl (zero, fd_o) ;          /* # used in third part */
    while (headxu)
    {
        nu++ ;
        l = (long int) headxu->u_characteristic ;
        fputl (l, fd_o) ;
        fputl (headxu->u_pc, fd_o) ;
        fprintf (fd_o, "%s\n", headxu->u_expression) ;
        h = headxu ;
        headxu = headxu->u_next ;
        free ((char *) h) ;
    }

    fseek (fd_o, zero, 0) ;
    fputl (magic, fd_o) ;
    fputl (p2, fd_o) ;            /* begin of second part */
    fputl (pc, fd_o) ;            /* # nibs in the code */

    fseek (fd_o, p2, 0) ;
    fputl (p3, fd_o) ;            /* begin of third part */
    fputl (nl, fd_o) ;            /* # labels defined in this module */

    fseek (fd_o, p3, 0) ;
    fputl (nu, fd_o) ;            /* # unresolved references here */

    if (ferror (fd_o)) error (ERRWRT, fobject) ;
}
