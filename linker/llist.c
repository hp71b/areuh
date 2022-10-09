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

#include "lglobal.h"

FILE *fd_l ;
int l_line = 0, l_page = 1 ;

static void l_files (void);
static void l_xref (void);



void
l_init (void)
{
    char dfl [MAXLEN+1] ;

    switch (cntlist)
    {
        case 0 :
            break ;
        case 1 :
            fd_l = stdout ;
            strcpy (flisting, "stdout") ;
            break ;
        case 2 :
            dfl_extension (dfl, "list", "al") ;
            look_obj (flisting, dfl) ;
            if (!(fd_l = fopen (flisting, "w")))
                error (ERROPN, flisting) ;
            break ;
    }
}


static void
l_new_page (int flag)
{
    if (!cntlist) return ;
    for (; l_line<page_size; l_line++) fprintf (fd_l, "\n") ;
    if (flag)
    {
        fprintf (fd_l,"AREUH LINKER V2.2 - Page %03d - File: %s\n",
                      ++l_page, flex) ;
        fprintf (fd_l,"\n") ;
        l_line = 2 ;
    }
    if (ferror (fd_l)) error (ERRWRT, flisting) ;
}


void
l_flush (void)
{
    if (!cntlist) return ;
    l_new_page (0) ;
    if (cntlist==2)
        if (fclose (fd_l)) error (ERRCLO, flisting) ;
}


void
l_print (char *line)
{
    if (!cntlist)
    {
        printf ("%s\n", line) ;
        return ;
    }

    if (l_line==page_size-6) l_new_page (1) ;
    fprintf (fd_l, "%s\n", line) ;
    l_line++ ;
    if (ferror (fd_l)) error (ERRWRT, flisting) ;
}


void
report (void)
{
    if (cntlist && errnb) l_new_page (1) ;
    if (xref) l_xref () ;
    if (cntlist && xref) l_new_page (1) ;
    if (cntlist) l_files () ;
    else if (xref && errnb)
    {
        printf ("Areuh linker : %03d errors", errnb) ;
        if (ferror (fd_l)) error (ERRWRT, flisting) ;
    }
}


static void
l_files (void)
{
    char line[MAXLEN+1], start[5+1], end[5+1], length[5+1] ;
    int i ;
    saddr val ;

    sprintf (line, "Output module : %s", flex) ;
    l_print (line) ;
    l_print ("") ;
    for (file=1; file<=nfile; file++)
    {
        sprintf (line, "Source module : %s", fname [file]) ;
        l_print (line) ;
        hex5 (start, tmodule[file].m_ad) ;
        val = (tmodule[file].m_ad==tmodule[file+1].m_ad) ? 0 : 1 ;
        hex5 (end, tmodule[file+1].m_ad - val) ;
        hex5 (length, tmodule[file+1].m_ad - tmodule[file].m_ad) ;
        sprintf (line, "%10sStart = %s, End = %s, Length = %s", 
                       "", start, end, length) ;
        l_print (line) ;
        l_print ("") ;
    }
    for (i=0; i<4; i++) l_print ("") ;
    strcpy (line, "Date : ") ;
    format_time (line+7) ;
    l_print (line) ;
    l_print ("") ;
    sprintf (line, "Errors : %03d", errnb) ;
    l_print (line) ;
    l_print ("") ;
    l_print ("") ;
    l_print ("Areuh Assembler/Linker V2.2, (c) P. David & J. Taillandier 1986  Paris, France") ;
    l_new_page (0) ;
}


static void
l_xref (void)
{
    char line [MAXLEN+1], tmp [MAXLEN+1], rel [MAXLEN+1] ;
    char format [MAXLEN+1], xformat [MAXLEN+1] ;
    struct symbol *t ;
    struct xtable *x ;
    int i ;

    sprintf (format, "%%-%ds = %%s  File : %%s", LBLLEN+1) ;
    sprintf (xformat, "%%%ds+ %%s   (Rel %%s in %%s)", LBLLEN+11) ;
    for (i=0; i<=255; i++)
    {
        t = h_label [i]->s_next ;
        while (t)
        {
            if (!t->s_os)
            {
                hex5 (tmp, t->s_value) ;
                sprintf (line, format, t->s_name, tmp, fname[(int) t->s_file]) ;
                l_print (line) ;
                x = t->s_xref ;
                while (x)
                {
                    hex5 (tmp, tmodule[(int) x->x_file].m_ad + x->x_pc) ;
                    hex5 (rel, x->x_pc) ;
                    sprintf (line, xformat, "", tmp, rel, fname[(int) x->x_file]) ;
                    l_print (line) ;
                    x = x->x_next ;
                }
            } /* du if */
            t = t->s_next ;
        } /* du while */
    } /* du for */
}
