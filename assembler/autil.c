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

			       GENERAL UTILITIES


hard_init, soft_init, between, term, memoire

******************************************************************************/


#include "aglobal.h"

void free_mem () ;

extern void o_init() ;
extern void dump_linker_infos () ;
extern void l_init(), l_flush() ;
extern void s_init() ;
extern void i_tab0(), i_tab1(), i_tab2(), i_tab3(), i_tab4(), i_tab5() ;
extern void print_ref() ;

extern char *malloc () ;

/******************************************************************************

				   INIT


synopsis : void init()
description : initiates the pass one. Opens source file, initializes the symbol
	      list. Called for each file assembled.

******************************************************************************/

void init ()
{
    uchar file [MAXLEN+1], *psource, *pfile ;

    if (!try_source (fsource))	     /* if can't open succesfully */
    {
	pfile = file ; psource = fsource ;
	while ((*pfile = *psource) && (*pfile != '.'))
        {
	    pfile++ ;
            psource++ ;
        }
	if (*pfile == EOL)
	{
	    strcpy (pfile, ".as") ;
	    if (try_source (file)) strcpy (fsource, file) ;
	}
    }
    if (fd_s==NULL) error (ERROPN, fsource) ;

    s_init() ;                                  /* symbol table init */

    passnb = 1 ;
    linenb = 0 ;
    cntlist = 0 ;
    modular = 1 ;
    *l_title = *l_stitle = EOL ;
}


/******************************************************************************

				    BETWEEN


synopsis : void between()
description : terminates the passe one, and prepares pass two.
	      Restore file source pointer, opens object file.

******************************************************************************/

void between()
{
    int r ;

    r =  fseek (fd_s,0L,0);
    if (r)  error (ERRREW, fsource) ;
    o_init() ;
    cntlist = cntlist_ref ;
    l_init() ;
    linenb = 0 ;
    passnb = 2 ;
    print_this_line = 1 ;
    errcnt = 0 ;
    headxu = (struct xused *) NULL ;
}

/******************************************************************************

				     TERM


synopsis : void term ()
description : ends the pass two, and thus the assembly.
	      Closes source file, and flushes object files.

******************************************************************************/

void term ()
{
    if (fclose (fd_s)) error (ERRCLO, fsource) ;
    if (modular) dump_linker_infos () ;
    if (fclose (fd_o)) error (ERRCLO, fobject) ;
    print_ref () ;
    l_flush () ;

    free_mem () ;
}


int try_source (file)
uchar *file ;
{
    long int magic ;

    fd_s = fopen (file, "r") ;
    if (fd_s)
    {
	fread (&magic, sizeof (long int), 1, fd_s) ;
	if (((magic>=ALF_MAGIC)&&(magic<=AL_MAGIC))||
	    ((magic>=AOF_MAGIC)&&(magic<=AO_MAGIC)))
	{
	    fclose (fd_s) ;
	    fd_s = (FILE *) NULL ;
	}
	else fseek (fd_s, 0L, 0) ;
    }
    return ((int) fd_s) ;
}


/******************************************************************************

				  MEMOIRE


synopsis : uchar *memoire (size)
	   int size
description : get memory from heap using malloc. It is just a layer above
	      malloc, including a test.

******************************************************************************/

uchar *memoire (size)
int size ;
{
    uchar *x ;

    if ((x = (uchar *) malloc (size)) == NULL)
	error (ERRMEM, "") ;
    return (x) ;
}


/******************************************************************************

				    UPRC


synopsis : void uprc (str)
	   uchar *str
description : uppercase a string

******************************************************************************/

void uprc (str)
uchar *str ;
{
    while (*str)
    {
	if ((*str >= 'a') && (*str <= 'z'))
	    *str -= 32 ;
	str++ ;
    }
}


/******************************************************************************

				 FORMAT_HEX


synopsis : void format_hex (str, val, dig)
	   uchar *str
           saddr val
           int dig
description : stores into str the hexadecimal string representing the dig
              low order hex digits of val.

******************************************************************************/

void format_hex (str, val, dig)
uchar *str ;
saddr val ;
int dig ;
{
    register int i, h ;

    for (i=dig-1; i>=0; i--)
    {
	h = (int) (val & ((saddr) 0xf)) ;
	str [i] = h + ((h < 10) ? '0' : 'A' - 10) ;
	val >>= 4 ;
    }
    str [dig] = EOL ;
}


/******************************************************************************

				 FREE_MEM


synopsis : void free_mem ()
description : frees all dynamically allocated memory during both passes.

******************************************************************************/

void free_mem ()
{
    struct xused *u ;
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
            if (s1->s_def)  free ((char *) s1->s_def) ;
            s2 = s1->s_next ;
            free ((char *) s1) ;
            s1 = s2 ;
        }
    }
    free ((char *) h_label [0]) ;
}
