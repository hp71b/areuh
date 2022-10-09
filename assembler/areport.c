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

                            END OF ASSEMBLY REPORT



print_ref, print_status, print_label, print_xref

******************************************************************************/

#include "aglobal.h"

extern void l_print (), l_new_page () ;
extern void format_hex ();

char format [15], formateq [16], xformat [MAXLEN], xf[15] ;

void print_status (), print_label (), print_xref () ;

/******************************************************************************

                                   PRINT_REF

synopsis : void print_ref () 
description : at the end of assembly (end of second pass), programmer want some
  informations on the program. This includes :
  - list of labels with their value, and cross reference
  - error count
  - time and date
  - that's all folks

******************************************************************************/

void print_ref ()
{
    sprintf (format, " %%-%ds %%s", LBLLEN) ;
    sprintf (formateq, "%%-%ds %%s", LBLLEN+1) ;
    sprintf (xf, "%%%ds+ ", LBLLEN+18) ;
    sprintf (xformat, xf, "") ;

    if ((cntlist) || (xref))
        print_label() ;

    if (cntlist) print_status() ;
    else
    {
        if (errcnt)
            printf ("aas: %d errors in file %s\n", errcnt, fsource) ;
    }
}


/******************************************************************************

                                 PRINT_STATUS


synopsis : void print_status ()
description : reports some status lines after the assembly

******************************************************************************/

void print_status()
{
    char line[MAXLEN+1], tmp[MAXLEN+1] ;

    l_print (0L, "", "", F_TL) ;
    l_print (0L, "", "", F_TL) ;
    l_print (0L, "", "", F_TL) ;
    l_print (0L, "", "", F_TL) ;
    l_print (0L, "", "", F_TL) ;
    l_print (0L, "", "", F_TL) ;

    sprintf (line, "Source  : %s", fsource) ;
    l_print (0L, "", line, F_TL) ;
    l_print (0L, "", "", F_TL) ;

    sprintf (line, "Object  : %s", fobject) ;
    l_print (0L, "", line, F_TL) ;
    l_print (0L, "", "", F_TL) ;

    strcpy (line, "Listing : ") ;
    if (cntlist == 1) strcat (line, "stdout") ;
    else strcat (line, flisting) ;
    l_print (0L, "", line, F_TL) ;
    l_print (0L, "", "", F_TL) ;

    format_time (tmp) ;
    sprintf (line, "Date    : %s", tmp) ;
    l_print (0L, "", line, F_TL) ;
    l_print (0L, "", "", F_TL) ;

    sprintf (line, "Errors  : %03d", errcnt) ;
    l_print (0L, "", line, F_TL) ;
    l_print (0L, "", "", F_TL) ;

    l_print (0L, "", "", F_TL) ;
    l_print (0L, "", "", F_TL) ;
    l_print (0L, "", "Areuh Assembler/Linker V2.4, (c) P. David & J. Taillandier 1986 Paris, France", F_TL) ;
}


/******************************************************************************

                                  PRINT_LABEL

synopsis : void print_label()
description : prints a complete list of labels encoutered in the assembly.

******************************************************************************/

void print_label ()
{
    struct symbol *t ;
    struct xtable *x ;
    char line[MAXLEN+1], tmp[MAXLEN+1] ;
    int i ;

    strcpy (l_stitle, "**** SYMBOL TABLE ****") ;
    l_new_page (1) ;

    for (i=0; i<256; i++)
    {
        t = h_label[i]->s_next ;
        while (t)
        {
            if (!t->s_os)
            {
                if (t->s_value >= (saddr) 0)
                {
                        hex5 (tmp, t->s_value) ;
                }
                else if (t->s_value == LBL_UDF)
                {
                        strcpy (tmp, "Undef") ;
                }
                else if (t->s_value == LBL_EXT)
                {
                        strcpy (tmp, "Extrn") ;
                }
                else /* (t->s_value == LBL_SEQ) || (t->s_value == LBL_XEQ) */
                {
                        strcpy (tmp, "Unkwn") ;
                }

                if (*(t->s_name)=='=')
                     sprintf (line, formateq, t->s_name, tmp) ;
                else sprintf (line, format, t->s_name, tmp) ;
                if (xref)
                {
                    switch (t->s_type)
                    {
                        case LUDF : strcat (line, " Ukn") ; break ;
                        case LABS : strcat (line, " Abs") ; break ;
                        case LREL : strcat (line, " Rel") ; break ;
                    }
                    if ((t->s_value!=LBL_UDF)&&(t->s_value!=LBL_EXT))
                        sprintf (tmp, "%s  %04d - ", line, t->s_decl) ;
                    else sprintf (tmp, "%s       - ", line) ;
                    x = t->s_xref ;
                    if (x)
                    {
                        while (x->x_next)  x = x->x_next ;
                        print_xref (x, tmp) ;
                    }
                    else
                    {
                        if (cntlist) l_print (0L, "", tmp, F_TL) ;
                        else printf ("%s\n", tmp) ;
                    }
                }
                else l_print (0L, "", line, F_TL) ;
            } /* if not (O.S. entry point not used) */
            t = t->s_next ;
        } /* while */
    } /* for */
}


/******************************************************************************

                                   PRINT_XREF


synopsis : void print_xref (x, line)
           struct xtable *x
           char *line
description : prints the complete cross-reference list backward from the end.
              x is the end of xref list, line is the line to be printed first.

******************************************************************************/

void print_xref (struct xtable *x, char *line)
{
    int col = LBLLEN+21 ;        /* column number */
    char tmp[MAXLEN+1], strnum[MAXLEN+1] ;

    strcpy (tmp, line) ;
    while (x)
    {
        if (col>75) 
        {
            if (cntlist) l_print (0L, "", tmp, F_TL) ;
            else printf ("%s\n", tmp) ;
            col = LBLLEN+21 ;
            strcpy (tmp, xformat) ;
        }
        sprintf (strnum, " %04d", x->x_line) ;
        strcat (tmp, strnum) ;
        col += 5 ;
        x = x->x_prev ;
    }
    if (cntlist) l_print (0L, "", tmp, F_TL) ;
    else printf ("%s\n", tmp) ;
}
