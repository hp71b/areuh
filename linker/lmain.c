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

saddr pc ;
int nfile, file ;
uchar flex [MAXLEN+1], flisting [MAXLEN+1] ;
FILE *fplex ;
struct module tmodule[MAXMOD] ;
struct unres *head_unres ;
struct symbol *h_label[256] ;
uchar *code ;
uchar *fname[MAXMOD+2] ;
int page_size, cntlist, xref, passnb, errnb ;
int passbis = 0 ;

void prompt (), read_line (), make_name () ;
extern uchar *memoire () ;

main (argc, argv)
int argc ;
uchar *argv[] ;
{
    int r = 0, c, i ;
    int errflg = 0 ;

    extern char *optarg ;
    extern int optind, opterr ;

    opterr = 0 ;

    xref = 0 ;
    page_size = 0 ;
    strcpy (flex, "") ;
    strcpy (flisting, "") ;

    fname[0] = memoire (sizeof(HP71EP)+1) ;
    strcpy (fname[0], HP71EP) ;
    nfile = 0 ;

    while ((c = getopt (argc, argv, "xa:Apl:o:")) != EOF)
        switch (c)
        {
            case 'x' :
                xref = 1 ;
                break ;
            case 'a' :
                strcpy (flisting, optarg) ;
                cntlist = 2 ;
                break ;
            case 'A' :
                cntlist = 1 ;
                break ;
            case 'p' :
                r++ ;
                break ;
            case 'l' :
                sscanf (optarg, "%d", &page_size) ;
                break ;
            case 'o' :
                strcpy (flex, optarg) ;
                break ;
            case '?' :
                errflg++ ;
                break ;
        }
        
    if ((r)||(argc==1)) prompt () ;
    if (errflg || ((optind==argc)&&(nfile==0)))
        error (ERRUSA, "") ;
    for (i=optind; i<argc; i++)
        make_name (nfile+i-optind+1, argv [i]) ;
    nfile += argc - optind ;

    if (*flex==EOL) strcpy (flex, "lex") ;

    if (page_size==0) page_size = 72 ;

    fname [nfile + 1] = memoire (sizeof("eof.ep")+1) ;
    strcpy (fname[nfile + 1], "eof.ep") ;

    init () ;
    pass1 () ;
    between () ;
    pass2 () ;
    term () ;
    exit (errnb) ;
}


/******************************************************************************

                                    PROMPT


synopsis : void prompt ()
description : if "-p" option is used, or if no parameters are passed to "ald",
  asks the user for missing arguments.

******************************************************************************/

void prompt ()
{
    uchar line [MAXLEN+1] ;

    do
    {
        printf ("Input module : ") ;
        read_line (stdin, line) ;
        if (*line) make_name (++nfile, line) ;
    } while (*line) ;
    if (*flex==EOL)
    {
        printf ("Object file : ") ;
        read_line (stdin, flex) ;
    }
    if (cntlist==0)
    {
        printf ("Listing file : ") ;
        read_line (stdin, flisting) ;
        if (*flisting)
        {
            cntlist = 2 ;
            xref = 1 ;
        }
        else cntlist = 0 ;
    }
    if (cntlist)
    {
        if (page_size==0)
        {
            printf ("Page length : ") ;
            read_line (stdin, line) ;
            sscanf (line, "%d", &page_size) ;
        }
    }
}

void read_line (fp, line)
FILE *fp ;
uchar *line ;
{
    int c, i = -1 ;

    do
    {
        c = getc (fp) ;
        if (i<MAXLEN) line[++i] = c ;
    } while ((c!=EOF)&&(c!='\n')) ; 
    line[i] = EOL ;
    if (ferror (fp)) error (ERRRD, "stdin") ; /* il se trouve que c'est stdin */
}


/******************************************************************************

                                   MAKE_NAME


synopsis : void make_name (i, file)
           int i
           uchar *file
description : parse file name, and store file name with proper extension in
  fname table.

******************************************************************************/

void make_name (i, file)
int i ;
uchar *file ;
{
    uchar *pfile ;

    pfile = file ;
    while ((*pfile)&&(*pfile!='.')) pfile++ ;
    fname [i] = memoire (strlen(file) + ((*pfile) ? 1 : 4) ) ;
    strcpy (fname[i], file) ;
    if (!(*pfile)) strcat (fname[i], ".ao") ;
}
