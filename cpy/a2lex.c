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

#include "copy.h"

char outname [100] ;
FILE *fpdev ;

init ()
{
#ifdef unix
    fpdev = stdout ;
#else
    sprintf (outname, "%s.lex", file) ;
    if ((fpdev = fopen (outname, "wb")) == NULL)
        error (ERROPN, outname) ;
#endif
}

output (c)
uchar c ;
{
    putc ((int) c, fpdev) ;
    if (ferror (fpdev)) error (ERRWRT, outname) ;
}

term ()
{
    if (fclose (fpdev))
        error (ERRCLO, outname) ;
}
