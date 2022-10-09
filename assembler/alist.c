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

			    LISTING FILE PROCESSING


l_init, l_flush, l_print, l_new_page, l_line, l_page

******************************************************************************/

#include "aglobal.h"

int l_line, l_page ;
char fstdout [] = "stdout" ;

static void l_page_header (void);

/******************************************************************************

				    L_INIT


synopsis : l_init()
description : initiates the listing mechanism, if necessary.

******************************************************************************/

void
l_init(void)
{
    char tmp [MAXLEN+1] ;

    switch (cntlist)
    {
	case 0 :
	    break ;
	case 1 :
	    fd_l = stdout ;
	    break ;
	case 2 :
	    dfl_extension (tmp, fsource, "al") ;
	    look_obj (flisting, tmp) ;
	    if ((fd_l = fopen (flisting, "w")) == NULL)
		error (ERROPN, flisting) ;
	    break ;
    }
    l_page = 0 ;
    if (cntlist) l_page_header () ;
}


/******************************************************************************

				    L_FLUSH


synospsis : void l_flush ()
description : forces a new page, then closes the listing file if necessary.

******************************************************************************/

void
l_flush(void)
{
    if (cntlist==0)	return ;
    l_new_page(0) ;
    if (cntlist == 2)
    {
	if (fclose (fd_l))  error (ERRCLO, flisting) ;
    }
}


/******************************************************************************

				    L_PRINT



synopsis : void l_print (pc, code, msg, flags)
	   saddr pc
	   char *code, *msg
	   int flags
description : prints a line on the listing file.
	      The line may contain  - program counter value	F_PC
				    - line number		F_LN
				    - generated code		F_GC
				    - text line 		F_TL
	      according the "flags" variable.
	      E.g. : to print pc and generated code, call
		     l_print(pc, gen_code, "", F_PC + F_GC)

******************************************************************************/

void
l_print (saddr address, char *code, char *msg, int flags)
{
    char line[MAXLEN+1], tmp[MAXLEN+1] ;

    if (!(cntlist))	return ;

    if (flags==F_TL)
    {
	sprintf (line, "%.79s", msg) ;
    }
    else
    {
	if (flags & F_LN) sprintf (line, "%04d ", linenb) ;     /* 5 chars */
	else strcpy (line, "     ") ;

	if (flags & F_PC)					/* 6 chars */
	{
	    hex5 (tmp, address) ;
	    strcat (line, tmp) ;
	    strcat (line, " ") ;
	}
	else strcat (line, "      ") ;

	if (flags & F_GC)					/* 9 chars */
	{
	    sprintf (tmp, "%-9.8s", code) ;
	    strcat (line, tmp) ;
	}
	else strcat (line, "         ") ;

	if (flags & F_TL)				       /* 60 chars */
	{
	    sprintf (tmp, "%.60s", msg) ;
	    strcat (line, tmp) ;
	}
    }

    if (l_line==(page_size-6))	 l_new_page (1) ;

    fprintf (fd_l, "%s\n", line) ;
    if (ferror (fd_l))
	error (ERRWRT, (cntlist == 1) ? fstdout : flisting) ;
    l_line++ ;

    if (strlen(code)>8)
	l_print (address+8, code+8, "", F_PC + F_GC) ;
}



/******************************************************************************

				  L_NEW_PAGE


synopsis : void l_new_page (flag)
	   int flag
description : forces a new page on listing file, and if flag # 0, prints a
	      header composed by page number, l_stitle and l_title.

******************************************************************************/

void
l_new_page (int flag)
{
    if (!(cntlist)) return ;

    for (; l_line<page_size; l_line++)	 fprintf (fd_l, "\n") ;
    if (ferror (fd_l))
	error (ERRWRT, (cntlist == 1) ? fstdout : flisting) ;

    if (flag) l_page_header () ;
}


static void
l_page_header (void)
{
    fprintf (fd_l, "Page %03d           %.60s\n", ++l_page, l_title) ;
    fprintf (fd_l, "AREUH ASS. V2.4    %.60s\n", l_stitle) ;
    fprintf (fd_l, "\n") ;
    l_line = 3 ;
    if (ferror (fd_l))
	error (ERRWRT, (cntlist == 1) ? fstdout : flisting) ;
}
