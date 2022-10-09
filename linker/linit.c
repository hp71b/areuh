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

static void free_mem (void);

void
init(void)
{
    struct symbol *s ;
    int i ;

    head_unres = (struct unres *) NULL ;

    s = (struct symbol *) memoire (256*sizeof(struct symbol)) ;
    for (i=0; i<256; i++)
    {
        strcpy (s->s_name, "") ;
        s->s_value = 0L ;
        s->s_file = 0 ;
        s->s_os = 1 ;
        s->s_xref = (struct xtable *) NULL ;
        s->s_next = (struct symbol *) NULL ;
        h_label [i] = s++ ;
    }
    l_init () ;
    file = errnb = passbis = 0 ;
    load_file (HP71EP) ;
    passnb = 1 ;
}

void
between (void)
{
    long int magic = AL_MAGIC ;
    char dfl [MAXLEN+1] ;

    strcpy (dfl, "lex") ;
    look_obj (flex, dfl) ;
    if (!(fplex = fopen (flex, "w")))
        error (ERROPN, flex) ;
    fwrite (&magic, sizeof (long int), 1, fplex) ;
    if (ferror (fplex)) error (ERRWRT, flex) ;
    passnb = 2 ;
}

void
term (void)
{
    if (fclose (fplex))
        error (ERRCLO, flex) ;
    if (cntlist || xref) report () ;
    l_flush () ;

    free_mem () ;
}


static void
free_mem (void)
{
    struct symbol *s1, *s2 ;
    struct xtable *x1, *x2 ;
    int i ;

    for (i=0; i<=255; i++)
    {
        s1 = h_label[i]->s_next ;
        while (s1)
        {
            if (xref)
            {
                x1 = s1->s_xref ;
                while (x1)
                {
                    x2 = x1->x_next ;
                    free ((char *) x1) ;
                    x1 = x2 ;
                }
            }
            s2 = s1->s_next ;
            free ((char *) s1) ;
            s1 = s2 ;
        }
    }
    free ((char *) h_label [0]) ;

    for (i=0; i<=nfile+1; i++)
        free ((char *) fname [i]) ;
}
