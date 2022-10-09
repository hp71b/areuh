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

                                TITAN ASSEMBLER


                               ERROR  PROCESSING


error

******************************************************************************/

#include "aglobal.h"

/******************************************************************************

                                     ERROR


synopsis : void error (errnb, msg)
           int errnb
           char *msg
descritpion : reports an error message on listing file, if any, else on
              standard output.

******************************************************************************/

void error (int errnb, char *msg)
{
    char txt[MAXLEN+1], tmp[MAXLEN+1+2];

    if ((errnb<0)||(passnb==2))
    {
        switch (errnb)
        {
            case ERROPN :
                sprintf (txt, "system error opening file %s",msg) ;
                break ;
            case ERRCLO :
                sprintf (txt, "system error closing file %s",msg) ;
                break ;
            case ERRREW :
                sprintf (txt,"system error on file %s at start of pass 2",msg);
                break ;
            case ERRWRT :
                sprintf (txt, "system error writing file %s", msg) ;
                break ;
            case ERRRD :
                sprintf (txt, "system error reading file %s", msg) ;
                break ;
            case ERRMEM :
                strcpy (txt, "not enough memory");
                break ;
            case ERRLEX :
                strcpy (txt, "invalid pseudo-op LEX or BIN");
                break ;
            case ERRPGS :
                strcpy (txt, "invalid page size");
                break ;
            case ERRFLN :
                strcpy (txt, "restricted label FiLeNd exists");
                break ;
            case ERRIFL :
                strcpy (txt, "invalid file name") ;
                break ;
            case ERRIMO :
                sprintf (txt,"invalid macro-op %s in modular assembling",msg);
                break ;
            case ERRVMD :
                sprintf (txt,"value must be defined for %s",msg);
                break ;
            case ERRUSA :
                strcpy (txt, "usage: aas [-p] [-l n] [-A] [-a file]") ;
                strcat (txt, " [-o object_file]  [file1 ... filen]") ;
                break ;

            case WRNEQU :
                strcpy (txt, "cannot resolve equate");
                break ;
            case WRNDUP :
                strcpy (txt, "duplicate label");
                break ;
            case WRNLBL :
                strcpy (txt, "illegal label");
                break ;
            case WRNULB :
                strcpy (txt, "unrecognized label");
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
            case WRNENA :
                strcpy (txt, "external references not allowed") ;
                break ;
            case WRNYES :
                strcpy (txt, "GOYES or RTNYES required");
                break ;
            case WRNIDP :
                strcpy (txt, "illegal dp arithmetic value");
                break ;
            case WRNIPP :
                strcpy (txt, "illegal pointer position");
                break ;
            case WRNISB :
                strcpy (txt, "illegal status bit");
                break ;
            case WRNTFR :
                strcpy (txt, "illegal transfer value");
                break ;
            case WRNIWS :
                strcpy (txt, "illegal word select");
                break ;
            case WRNLST :
                strcpy (txt, "invalid LIST argument");
                break ;
            case WRNJVL :
                strcpy (txt, "jump or value too large");
                break ;
            case WRNMLB :
                strcpy (txt, "missing label");
                break ;
            case WRNTST :
                strcpy (txt, "needs previous test instruction");
                break ;
            case WRNNHX :
                strcpy (txt, "non hexadecimal digits present");
                break ;
            case WRNTMA :
                strcpy (txt, "too much ascii characters present");
                break ;
            case WRNTMH :
                strcpy (txt, "too much hexadecimal digits present");
                break ;
            case WRNOPC :
                strcpy (txt, "unknown opcode");
                break ;
            case WRNIIF :
                strcpy (txt, "invalid conditional structure") ;
                break ;
        }
        if (errnb<0)                              /* fatal error */
        {
            fprintf(stderr, "aas: %s\n", txt) ;
            exit (errnb) ;
        }
        else
        {
            if (cntlist)
            {
                sprintf (tmp, "* %s", txt) ;
                if (error_this_line++) l_print ((saddr)0, "", "", F_TL) ;
                l_print ((saddr)0, "", tmp, F_TL) ;
            }
            else printf ("Line %d : %s\n", linenb, txt);
            errcnt++ ;           /* we are in the second pass case */
        }
    }   /* end of "if" statement */
}       /* end of "error()"      */
