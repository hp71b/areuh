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

			    OPCODE PROCESSING (PART 2)


******************************************************************************/

#include "aglobal.h"
#include "agen.h"

extern void l_new_page(), l_print (), ps_line () ;
extern int ascii_len () ;
extern saddr calc_expression () ;

int ltok = 255 ;	   /* lowest token */
int htok = 0 ;		   /* highest token */
int ctok = 0 ;		   /* current token */
int ckey = 0 ;		   /* current key macro-op */


/******************************************************************************

			       FILE_HEADER

synopsis : file_header (modif, type)
	   uchar *modif, *line
description : expand a macro-op (LEX or BIN)

******************************************************************************/

file_header (modif, type)
uchar *modif, *type ;
{
    uchar line [MAXLEN+1], *pline, limit, c ;
    int i = 0, later = 0 ;

    uprc (modif) ;
    strcpy (line, "  NIBASC ") ;
    pline = line + 9 ;
    limit = *modif ;
    if ((limit!='\'')&&(limit!='\\')) error (ERRIFL, "") ; /* inv. file name */
    *pline = limit ;
    pline++ ;
    modif++ ;
    while ((i<=8)&&((c = *modif)!=limit)&&(c))
    {
	i++ ;
	if (!( ((c>='A')&&(c<='Z')) ||
	       ((later)&&(c>='0')&&(c<='9')) ) )
	    error (ERRIFL, "") ;           /* invalid file name */
	later++ ;
	*pline = c ;
	modif++ ;
	pline++ ;
    }
    if ((i==0)||(i==9)) error (ERRIFL, "") ;
    while (i<8)
    {
	*pline = ' ' ;
	pline++ ;
	i++ ;
    }
    *pline = limit ;
    *(pline+1) = EOL ;
    ps_line (line) ;

    strcpy (line, "  NIBHEX ") ;
    strcat (line, type) ;
    ps_line (line) ;

    ps_line ("  CON(2) 0") ;                /* copy code / secure code */

    for (i=1; i<=5; i++) ps_line ("  CON(2) #00") ;   /* time */

    ps_line ("  REL(5) FiLeNd") ;

    ctok = ckey = 0 ;
}


/******************************************************************************

				      BSS


synopsis : void bss (modif)
	   char *modif
description : areuh

******************************************************************************/

void bss (modif)
uchar *modif ;
{
    saddr val ;
    int i, m ;

    val = calc_expression (modif) ;
    if (val < 0L) error (ERRVMD, "BSS") ;                 /* fatal error */
    gen_len = (int) val ;
    if (passnb==2)
    {
	m = (gen_len>18) ? 18 : gen_len ;
	for (i=0; i<m; i++)
	    gen_code[i] = '0' ;
	gen_code[m] = EOL ;
    }
}


void eject ()
{
    l_new_page (1) ;
    print_this_line = 0 ;
}


void endx ()
{
    running = 0 ;
}


void list (modif)
uchar *modif ;
{
    int r = 0 ;
    uchar c ;

    uprc (modif) ;
    c = *modif++ ;
    if (c=='O')
    {
	c = *modif++ ;
	switch (c)
	{
	    case 'N' :
		r = 1 ;
		break ;
	    case 'F' :
		c = *modif++ ;
		if (c=='F')  r = 0 ;
		else r = -1 ;
		break ;
	    default :
		r = -1 ;
		break ;
	}
    }
    else r = -1 ;

    c = *modif ;
    if ((r==-1)||((c!=' ')&&(c!=EOL)&&(c!='\t')))
	error (WRNLST, "") ;   /* invalid LIST argument */
    else cntlist = r * cntlist_ref ;
}


void title (modif)
uchar *modif ;
{
    if (*l_title==EOL) strcpy (l_title, modif) ;
}


void stitle (modif)
uchar *modif ;
{
    strcpy (l_stitle, modif) ;
    eject () ;
}


void lex (modif, line)
uchar *modif, *line ;
{
    switch (passnb)
    {
	case 1 :
	    if (linenb!=1) error (ERRLEX, "") ; /* invalid macro LEX or BIN */
	    modular = 0 ;
	    gen_len = 37 ;
	    break ;
	case 2 :
	    if (cntlist) l_print (pc, gen_code, line, F_PC+F_LN+F_TL) ;
	    file_header (modif, "802E") ;
	    print_this_line = 0 ;
	    gen_len = 0 ;
	    break ;
    }
}


void id (modif, line)
uchar *modif, *line ;
{
    uchar tmp [MAXLEN+1] ;

    switch (passnb)
    {
	case 1 :
	    if (modular) error (ERRIMO, "ID") ;
	    break ;
	case 2 :
	    if (cntlist) l_print (pc, gen_code, line, F_PC+F_LN+F_TL) ;
	    sprintf (tmp, "   CON(2) %s", modif) ;
	    ps_line (tmp) ;
	    sprintf (tmp, "   CON(2) %03d", ltok) ;
	    ps_line (tmp) ;
	    sprintf (tmp, "   CON(2) %03d", htok) ;
	    ps_line (tmp) ;
	    ps_line ("   CON(5) 0") ;
	    ps_line ("   NIBHEX F") ;
	    ps_line ("   REL(4) 1+TxTbSt") ;
	    print_this_line = gen_len = 0 ;
	    break ;
    } /* du switch */
}


void msg (modif, line)
uchar *modif, *line ;
{
    uchar tmp [MAXLEN+1] ;
    saddr val ;

    switch (passnb)
    {
	case 1 :
	    if (modular) error (ERRIMO, "MSG") ;
	    break ;
	case 2 :
	    if (cntlist) l_print (pc, gen_code, line, F_PC+F_LN+F_TL) ;
	    if (((val = calc_expression (modif)) == 0L) || (val == EXP_ERR))
		ps_line ("   CON(4) 0") ;
	    else
	    {
		sprintf (tmp, "   REL(4) %s", modif) ;
		ps_line (tmp) ;
	    }
	    print_this_line = gen_len = 0 ;
    } /* du switch */
}


void poll (modif, line)
uchar *modif, *line ;
{
    uchar tmp [MAXLEN+1] ;
    saddr val ;

    switch (passnb)
    {
	case 1 :
	    if (modular) error (ERRIMO, "POLL") ;
	    break ;
	case 2 :
	    if (cntlist) l_print (pc, gen_code, line, F_PC+F_LN+F_TL) ;
	    if (((val = calc_expression (modif)) == 0L) || (val == EXP_ERR))
		ps_line ("   CON(5) 0") ;
	    else
	    {
		sprintf (tmp, "   REL(5) %s", modif) ;
		ps_line (tmp) ;
	    }
	    print_this_line = gen_len = 0 ;
    } /* du switch */
}


void entryx (modif, line)
uchar *modif, *line ;
{
    uchar tmp [MAXLEN+1] ;

    switch (passnb)
    {
	case 1 :
	    if (modular) error (ERRIMO, "ENTRY") ;
	    break ;
	case 2 :
	    if (cntlist) l_print (pc, gen_code, line, F_PC+F_LN+F_TL) ;
	    if (!ctok) ps_line ("   * * * M A I N   T A B L E * * *") ;
	    sprintf (tmp, "   CON(3) (TxEn%02d)-(TxTbSt)", ++ctok) ;
	    ps_line (tmp) ;
	    sprintf (tmp, "   REL(5) %s", modif) ;
	    ps_line (tmp) ;
	    print_this_line = gen_len = 0 ;
	    break ;
    } /* du switch */
}


void charx (modif, line)
uchar *modif, *line ;
{
    uchar tmp [MAXLEN+1] ;

    switch (passnb)
    {
	case 1 :
	    if (modular) error (ERRIMO, "CHAR") ;
	    break ;
	case 2 :
	    if (cntlist) l_print (pc, gen_code, line, F_PC+F_LN+F_TL) ;
	    sprintf (tmp, "   CON(1) %s", modif) ;
	    ps_line (tmp) ;
	    print_this_line = gen_len = 0 ;
	    break ;
    }
}


void key (modif, line)
uchar *modif, *line ;
{
    uchar tmp [MAXLEN+1] ;

    switch (passnb)
    {
	case 1 :
	    if (modular) error (ERRIMO, "KEY") ;
	    if (!ckey) ps_line (" TxTbSt") ;
	    sprintf (tmp, " TxEn%02d", ++ckey) ;
	    ps_line (tmp) ;
	    gen_len = ((ascii_len(modif))*2) + 1 ;
	    break ;
	case 2 :
	    uprc (modif) ;
	    if (cntlist) l_print (pc, gen_code, line, F_PC+F_LN+F_TL) ;
	    if (!ckey)
	    {
		ps_line ("   * * * T E X T   T A B L E * * *") ;
		ps_line (" TxTbSt") ;
	    }
	    sprintf (tmp, " TxEn%02d", ++ckey) ;
	    ps_line (tmp) ;
	    sprintf (tmp, "   CON(1) %02d", ((ascii_len(modif))*2) - 1) ;
	    ps_line (tmp) ;
	    sprintf (tmp, "   NIBASC %s", modif) ;
	    ps_line (tmp) ;
	    print_this_line = gen_len = 0 ;
	    break ;
    } /* du switch */
}


void token (modif, line)
uchar *modif, *line ;
{
    uchar tmp [MAXLEN+1] ;
    saddr tok ;

    switch (passnb)
    {
	case 1 :
	    if (modular) error (ERRIMO, "TOKEN") ;
	    tok = calc_expression (modif) ;
	    if (tok<ltok) ltok = tok ;
	    if (tok>htok) htok = tok ;
	    break ;
	case 2 :
	    if (cntlist) l_print (pc, gen_code, line, F_PC+F_LN+F_TL) ;
	    sprintf (tmp, "   CON(2) %s", modif) ;
	    ps_line (tmp) ;
	    print_this_line = gen_len = 0 ;
	    break ;
    } /* du switch */
}


void bin (modif, line)
uchar *modif, *line ;
{
    switch (passnb)
    {
	case 1 :
	    if (linenb!=1) error (ERRLEX, "") ; /* invalid macro LEX or BIN */
	    modular = 0 ;
	    gen_len = 37 ;
	    break ;
	case 2 :
	    if (cntlist) l_print (pc, gen_code, line, F_PC+F_LN+F_TL) ;
	    file_header (modif, "402E") ;
	    print_this_line = 0 ;
	    gen_len = 0 ;
	    break ;
    }
}


void chain (modif, line)
uchar *modif, *line ;
{
    uchar tmp [MAXLEN+1] ;

    switch (passnb)
    {
	case 1 :
	    if (modular) error (ERRIMO, "CHAIN") ;
	    break ;
	case 2 :
	    if (cntlist) l_print (pc, gen_code, line, F_PC+F_LN+F_TL) ;
	    if (calc_expression (modif) == EXP_ERR)
		ps_line ("   CON(5) -1") ;
	    else
	    {
		sprintf (tmp, "   REL(5) %s", modif) ;
		ps_line (tmp) ;
	    }
	    ps_line ("   CON(5) -1") ;
	    ps_line ("   NIBHEX 20") ;
	    print_this_line = gen_len = 0 ;
	    break ;
    }
}


void endtxt (line)
uchar *line ;
{
    switch (passnb)
    {
	case 1 :
	    if (modular) error (ERRIMO, "ENDTXT") ;
	    if (!ckey) ps_line (" TxTbSt") ;
/*	      if (!ckey) add_label (" TxTbSt", pc, "", LREL, 0) ; */
	    gen_len = 3 ;
	    break ;
	case 2 :
	    if (cntlist) l_print (pc, gen_code, line, F_PC+F_LN+F_TL) ;
	    if (!ckey) ps_line (" TxTbSt") ;
	    ps_line ("   NIBHEX 1FF") ;
	    print_this_line = gen_len = 0 ;
	    break ;
    } /* du switch */
}

void endifx ()
{
    if (!(in_if || in_else))
	error (WRNIIF, "") ;      /* Invalid conditional structure */
    in_if = in_else = 0 ;
    exec = 1 ;
}

void absx (modif)	/* FUTURE USE... */
uchar *modif ;
{
}

void rdsymb (modif)
uchar *modif ;
{
    uchar *pmodif ;

    if (passnb==1)
    {
	pmodif = modif ;
	while ((*pmodif!=EOL)&&(*pmodif!='\t')&&(*pmodif!=' ')) pmodif++ ;
	*pmodif = EOL ;
	load_file (modif) ;
    }
}

void elsex ()
{
    if (!in_if)
    {
	error (WRNIIF, "") ;      /* Invalid conditional structure */
	in_if = in_else = 0 ;
	exec = 1 ;
    }
    else
    {
	in_if = 0 ; in_else = 1 ;
	exec = ! exec ;
    }
}

void ifx (modif)
uchar *modif ;
{
    saddr val ;

    if (in_if || in_else)
    {
	error (WRNIIF, "") ;      /* Invalid conditional structure */
	in_if = in_else = 0 ;
	exec = 1 ;
    }
    else
    {
	val = calc_expression (modif) ;
	if (val==EXP_EXT)
	    error (WRNEXP, "") ;  /* Illegal expression */
	in_if = 1 ; in_else = 0 ;
	exec = (int) val ;
    }
}
