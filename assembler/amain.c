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

                                 MAIN FONCTION


main, prompt

******************************************************************************/

#include "aglobal.h"

struct symbol *h_label[256] ;

struct xused *headxu ;

uchar fsource [MAXLEN+1], flisting [MAXLEN+1], fobject [MAXLEN+1] ;
FILE *fd_s, *fd_l, *fd_o ;

int cntlist, cntlist_ref ;
int page_size ;
uchar l_title[MAXLEN+1], l_stitle[MAXLEN+1];
int errcnt, error_this_line ;
int print_this_line ;
int xref ;
int running ;
int passnb ;
int linenb ;

saddr pc ;
int gen_len ;
uchar gen_code[19] ;
int prev_test ;
int exec, in_if, in_else ;

int modular, linker ;


extern void init(), between(), term(), pass() ;
extern int read_line() ;


/******************************************************************************

                                     MAIN




synopsis : aas  [-p]  [-l n] [-A] [-a afile] [-o objfile]  [file1 [... filen]]

description : Aas is a cross assembler which runs on UNIX based machines.
              Aas assembles the named file, or the standard input if no file
              is specified. The optional arguments -a or -A may be used to
              obtain an assembly listing. If -A is used, listing goes to
              standard output. If -a is used, the listing goes to afile.
              -l option specifies page length.
              The output of assembly is left in the filoe objfile; if that is
              omitted, then the output is left in a file "lex".

******************************************************************************/

main (argc, argv)
int argc ;
char *argv[] ;
{
    int  r = 0, c, i ;
    int errflg = 0 ;
    int errrep = 0 ;
    char saveo [MAXLEN+1], savel [MAXLEN+1] ;

    extern char *optarg ;
    extern int optind, opterr ;


    opterr = 0 ;

/* par defaut, on a : */
    xref = 0 ;                      /* pas de cross ref */
    page_size = 0 ;                 /* taille de page non fournie */
    cntlist_ref = 0 ;               /* pas de listing */
    strcpy (fobject, "") ;          /* fichier objet := "" */
    strcpy (fsource, "") ;          /* fichier source := "" */
    strcpy (flisting, "") ;         /* fichier listing := "" */

/* analyse des options */
    while ((c = getopt(argc, argv, "xa:Apl:o:")) != EOF)
        switch (c)
        {
            case 'x' :                               /* cross-ref */
                xref = 1 ;
                break ;
            case 'a' :                               /* listing sur fichier */
                strcpy (flisting, optarg) ;
                cntlist_ref = 2 ;
                break ;
            case 'A' :                               /* listing sur stdout */
                cntlist_ref = 1 ;
                break ;
            case 'p' :                               /* prompt */
                r++ ;                                /* r := 1 */
                break ;
            case 'l' :                               /* page length */
                sscanf (optarg, "%d", &page_size) ;  /* page_size := -l <n> */
                break ;
            case 'o' :                               /* object file */
                strcpy (fobject, optarg) ;
                break ;
            case '?' :                               /* non reconnu */
                errflg++ ;
                break ;
        }
/* si on a demande "-p", ou si pas d'argument : interface interactive */
    if ((r)||(argc==1)) prompt () ;
/* si option non reconnue ou si pas de nom de source, alors erreur */
    if (errflg || ((optind==argc)&&(fsource[0]==EOL)))
        error (ERRUSA, "") ;

/* page_size = 72 par defaut */
    if (page_size==0)         page_size = 72 ;

    if (*fsource)               /* got source file name with prompt() */
    {
        init() ;
        pass() ;
        between() ;
        pass() ;
        term() ;
        errrep = errcnt ;
    }
    else
    {
        for (i=optind; i<argc; i++)
        {
            strcpy (saveo, fobject) ;
            strcpy (savel, flisting) ;
            strcpy (fsource, argv [i]) ;
            if (optind != argc-1)  printf ("%s:\n", fsource) ;
            init() ;
            pass() ;
            between() ;
            pass() ;
            term() ;
            strcpy (fobject, saveo) ;
            strcpy (flisting, savel) ;
            errrep += errcnt ;
        }
    }
    exit (errrep) ;                  /* permet de signaler les erreurs */
}


/******************************************************************************

                                    PROMPT


synopsis : prompt()
description : if "-p" option is used, prompts the user for files to be used.

******************************************************************************/

prompt ()
{
    uchar line[MAXLEN+1] ;

    while (fsource[0]==EOL)
    {
        printf ("Source  file : ") ;
        read_line (stdin, fsource) ;
    }
    if (fobject[0]==EOL)
    {
        printf ("Object  file : ") ;
        read_line (stdin, fobject) ;
    }
    if (cntlist_ref==0)
    {
        printf ("Listing file : ") ;
        read_line (stdin, flisting) ;
        cntlist_ref = (flisting[0]) ? 2 : 0 ;
    }
    if (cntlist_ref)
    {
        if (xref==0)
        {
            printf ("Cross reference (y/n) : ") ;
            read_line (stdin, line) ;
            if ((*line=='y')||(*line=='Y')||(*line=='o')||(*line=='O'))
                xref = 1 ;
        }
        if (page_size==0)
        {
            printf ("Page length : ") ;
            read_line (stdin, line) ;
            sscanf (line, "%d", &page_size) ;
        }
    }
}
