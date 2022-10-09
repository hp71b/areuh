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

void output (char c) ;
void init (),  term (), error () ;
int min () ;

#include "copy.h"

char outname [100] ;
FILE *fpdev ;

void init ()
{
#ifdef unix
    fpdev = stdout ;
#else
    sprintf (outname, "%s.lex", file) ;
    if ((fpdev = fopen (outname, "wb")) == NULL)
        error (ERROPN, outname) ;
#endif
}

void output (char c)
{
    putc ((int) c, fpdev) ;
    if (ferror (fpdev)) error (ERRWRT, outname) ;
}

void term ()
{
    if (fclose (fpdev))
        error (ERRCLO, outname) ;
}
