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

				PASS PROCESSING


pass

******************************************************************************/

#include "aglobal.h"
#include "agen.h"

extern struct mnemo_desc *find_mnemo() ;
extern int read_line();
extern void parse_line(), ps_label(), process_mnemo(),
	    l_new_page(), l_print(), o_print() ;
extern struct symbol *add_label() ;

void ps_line() ;


/******************************************************************************

				     PASS


synopsis : void pass()
description :

******************************************************************************/

void pass()
{
    uchar line[MAXLEN+1];
    int c;

    pc = 0 ;
    prev_test = 0 ;
    running = 1 ;
    exec = 1 ; in_if = in_else = 0 ;

    while (running)
    {
	c = read_line (fd_s, line) ;
	error_this_line = 0 ;
	if (c) running = 0 ;
	else ps_line (line) ;
    }

    if (passnb==1)
    {
	add_label("FiLeNd", pc, "", LREL, 0) ;
    }
}


/******************************************************************************

				  PS_LINE


synopsis : void ps_line (line)
	   uchar *line
description : parses the line read from the input (breaks the line into three
	      components), processes the label if necessary, processes the
	      mnemonic if necessary, and does the listing and object file
	      actualization, if pass one.

******************************************************************************/

void ps_line (line)
uchar *line ;
{
    uchar label[LBLLEN+2], mnemo[7], modif[MAXLEN+1] ;
    struct mnemo_desc *ad ;

    parse_line (line, label, mnemo, modif) ;

    if (!exec)
    {
	if ( (strcmp(mnemo, "IF"))&&
	     (strcmp(mnemo, "ELSE"))&&
	     (strcmp(mnemo, "ENDIF"))&&
	     (strcmp(mnemo, "END")) )
	    return ;	  /* Neither IF nor ELSE nor ENDIF */
    }
    if (*label)
	ps_label (label, mnemo, modif) ;

    *gen_code = EOL ;
    gen_len = 0 ;

    if (*mnemo)
    {
	ad = find_mnemo (mnemo) ;
	if (ad)
	{
	    gen_len = ad->m_len ;
	    switch (passnb)
	    {
		case 1 :
		    if (!((ad->m_flag)&F_LSET))
			 ps_mnemo (line, modif, ad) ;
		    break ;
		case 2 :
		    strcpy (gen_code, ad->m_code) ;
		    ps_mnemo (line, modif, ad) ;
		    if ((prev_test)&&(!(ad->m_flag & F_GOYS)))
			error (WRNYES, "") ;     /* GOYES or RTNYES required */
		    break ;
	    }
	}
	else
	{
	    error (WRNOPC, "") ;       /* unknown oopcode */
	    if (prev_test)
		error (WRNYES, "") ;     /* GOYES or RTNYES required */
	}
    }
    else ad = (struct mnemo_desc *) NULL ;

    if (passnb==2)
    {
	if ((cntlist)&&(print_this_line++))
	    l_print (pc, gen_code, line, F_PC+F_LN+F_GC+F_TL) ;
	o_print (gen_code, gen_len) ;
	if (ad) prev_test = ((ad->m_flag)&F_RETY) ? 1 : 0 ;
    }

    pc += gen_len ;
}
