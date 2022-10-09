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

                                 AREUH LINKER


                               ERROR PROCESSING


error

******************************************************************************/

#include "lglobal.h"

/******************************************************************************

                                     ERROR


synopsis : void error (errno, msg)
           int errno
           char *msg
descritpion : reports an error message on listing file, if any, else on
              standard output.

******************************************************************************/

void
error (int errno, char *msg)
{
    char txt[MAXLEN+1+50], tmp[MAXLEN+1];

    switch (errno)
    {
        case ERROPN :
            sprintf (txt, "system error opening file %s", msg) ;
            break ;
        case ERRCLO :
            sprintf (txt, "system error closing file %s", msg) ;
            break ;
        case ERRWRT :
            sprintf (txt, "system error writing file %s", msg) ;
            break ;
        case ERRRD  :
            sprintf (txt, "system error reading file %s", msg) ;
            break ;
        case ERRMEM :
            strcpy (txt, "not enough memory");
            break ;
        case ERRUSA :
            strcpy (txt, "usage: ald [-p] [-l n] [-A] [-a file]") ;
            strcat (txt, " [-o object_file] file1 file2..filen") ;
            break ;
        case ERRICV :
            sprintf (txt, "incompatible version of file %s", fname[file]) ;
            break ;
        case ERRNOA :
            sprintf (txt, "file %s not an output from aas", fname[file]) ;
            break ;

        case WRNDUP :
            sprintf (txt, "duplicate label %s", msg) ;
            break ;
        case WRNURF :
            hex5 (tmp, pc) ;
            sprintf (txt, "unresolved reference at %s in file %s : %s",
                          tmp, fname[file], msg) ;
            break ;
        case WRNURL :
            sprintf (txt, "unresolved label %s in file %s", msg, fname[file]) ;
            break ;
        case WRNEXP :
            strcpy (txt, "illegal expression");
            break ;
        case WRNASC :
            strcpy (txt, "illegal ascii constant");
            break ;
        case WRNPAR :
            strcpy (txt, "mismatched parenthesis");
            break ;
        case WRNIHX :
            strcpy (txt, "illegal hexadecimal constant");
            break ;
        case WRNNUL :
            strcpy (txt, "null divisor");
            break ;
        case WRNIXP :
            strcpy (txt, "illegal exponentiation") ;
            break ;
        case WRNIBC :
            strcpy (txt, "illegal binary constant") ;
            break ;
        case WRNJVL :
            hex5 (tmp, pc) ;
            sprintf (txt, "jump or value too large at %s in file %s : %s",
                          tmp, fname[file], msg) ;
            break ;
    }
    if (errno<0)
    {
        fprintf (stderr, "ald: %s\n", txt) ;
        exit (errno) ;
    }
    else
    {
        errnb++ ;
        if (! ((passnb==1)&&(!passbis)&&(errno==WRNURL)) )
          l_print (txt) ;
    }
}       /* end of "error()"      */
