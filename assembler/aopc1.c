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

			    OPCODE PROCESSING (PART 1)


******************************************************************************/

#include "aglobal.h"
#include "agen.h"

extern void format_hex (char *, saddr, int) ;

#define UPRC(c)   ((((c)>='a')&&((c)<='z')) ? (c)-32 : (c))

extern char *memoire() ;
extern saddr calc_expression () ;
extern char hex () ;


/******************************************************************************

				  ADD_XUSED


synopsis : void add_xused (type, pc, len, exp)
	   int type, len
	   saddr pc
	   char *exp
description : adds the parameters about the use of an expression containing
	      external references.

******************************************************************************/

void add_xused (int type, saddr addr, int len, char*exp)
{
    struct xused *xu ;

    xu = (struct xused *) memoire (sizeof (struct xused)) ;
    if (xu==(struct xused *) NULL)
	error (ERRMEM, "") ;
    xu->u_characteristic = (char) (type + len) ;
    xu->u_pc = addr ;
    xu->u_expression = memoire (strlen (exp) + 1) ;
    if (xu->u_expression==(char *)NULL)
	error (ERRMEM, "") ;
    strcpy (xu->u_expression, exp) ;
    xu->u_next = headxu ;     /* queue it */
    headxu = xu ;	      /* before the previous head */
}


/******************************************************************************

				  FIELD_SELECT


synopsis : field_select (modif)
	   char *modif
description : to be described, in a (far) future...

******************************************************************************/

int field_select (char *modif)
{
    int r = 0 ;
    char c ;

    switch (UPRC(*modif))
    {
	case 'P' :
	    r = 1 ;
	    break ;
	case 'W' :
	    if (UPRC(modif[1])=='P')
	    {
		r = 2 ;
		modif++ ;
	    }
	    else r = 8 ;
	    break ;
	case 'X' :
	    if (UPRC(modif[1])=='S')
	    {
		r = 3 ;
		modif++ ;
	    }
	    else r = 4 ;
	    break ;
	case 'S' :
	    r = 5 ;
	    break ;
	case 'M' :
	    r = 6 ;
	    break ;
	case 'B' :
	    r = fs_B ;
	    break ;
	case 'A' :
	    r = fs_A ;
	    break ;
    }
    c = *(++modif) ;
    if ((c!=' ')&&(c!=EOL)&&(c!='\t')) r = 0 ;
    return (r) ;
}


void regtest(struct mnemo_desc *ad, char *modif)
{
    int r ;

    if ((r=field_select(modif)))
    {
	if (r==fs_A)
	{
	    gen_code[1] = hex (r + ad->m_a) ;
	}
	else
	{
	    gen_code[0] = '9' ;
	    gen_code[1] = hex ((ad->m_a==2) ? r+7 : r-1) ;
	}
    }
    else error (WRNIWS, "") ;
}


void regarith (struct mnemo_desc *ad, char *modif)
{
    int r ;

    if ((r=field_select(modif)))
    {
	/* gen_len is 3 by default */
	if (r==fs_A)   gen_len = 2 ;

	if (passnb==2)
	{
	    if (r==fs_A)
	    {
		gen_code[0] = hex (ad->m_a + 11) ;
		gen_code[1] = gen_code[2] ;
		gen_code[2] = EOL ;
	    }
	    else
	    {
		gen_code[0] = hex(((ad->m_a==ar_C)||(ad->m_a==ar_D))?10:11) ;
		gen_code[1] = hex(((ad->m_a==ar_C)||(ad->m_a==ar_E))?r-1:r+7) ;
	    }
	}
    }
    else error (WRNIWS, "") ;
}


void reglogic (char *modif)
{
    int r ;

    if ((r=field_select(modif)))
	gen_code[2] = (r==fs_A) ? 'F' : hex(r-1) ;
    else error (WRNIWS, "") ;
}


int range (saddr org, saddr dest, int nibs, saddr *offset)
{
    saddr Sixtine, Fiftine ;
    int r ;

    Sixtine = ((saddr) 1) << (nibs*4) ;
    Fiftine = Sixtine >> 1 ;
    *offset = dest - org ;
    r = ((*offset >= -Fiftine) && (*offset < Fiftine)) ;
    if (*offset<0)   (*offset) += Sixtine ;	   /* -128 = 128 ; -1 = 255 */
    return (r) ;
}


void branches (struct mnemo_desc *ad, char *modif)
{
    saddr val, offset, pc_bis ;
    int fits, i, j ;
    char hex_var[MAXLEN+1] ;

    if ((ad->m_flag & F_GOYS) && (!(prev_test)))
	error (WRNTST, "") ;       /* needs previous test instruction */
    else
    {
	val = calc_expression (modif) ;
	if (val >= 0L)
	{
	    if (ad->m_flag & F_ABSL)
	    {
		fits = 1 ;
		offset = val ;
	    }
	    else
	    {
		if (ad->m_flag & F_GSUB) pc_bis = pc + (long int) gen_len ;
		else pc_bis = pc + (long int) (ad->m_a) - 1L ;
		fits = range (pc_bis, val, ad->m_b, &offset) ;
	    }

	    if (!(fits))
	    {
		offset = 0L ;
		error (WRNJVL, "") ;       /* Jump or value too large */
	    }

	    hex6 (hex_var, offset) ;
	    for (i=1, j=5; i<=ad->m_b; i++, j--)
	    {
		gen_code[ad->m_a + i - 2] = hex_var[j] ;
	    }
	    gen_code[gen_len] = EOL ;
	}
	else	       /* EXP_ERR or EXP_EXT */
	{
	    if (val == EXP_EXT)
	    {
		switch (ad->m_flag & (F_GSUB | F_ABSL))
		{
		    case F_GSUB :
			i = XRGSB ;
			break ;
		    case F_ABSL :
		    case F_ABSL | F_GSUB :
			i = XABSL ;
			break ;
		    default :
			i = XRGTO ;
			break ;
		}
		add_xused (i, pc + (long int) ad->m_a - 1L, ad->m_b, extexp) ;
	    }
	    for (i=0; i<ad->m_b; i++) gen_code [ad->m_a - 1 + i] = '0' ;
	    gen_code [gen_len] = EOL ;
	}
    }
}


void rtnyes()
{
    if (!(prev_test))  error (WRNTST, "") ;   /* needs previous test instr. */
}


void ptrtest (char *modif)
{
    saddr val ;

    val = calc_expression (modif) ;
    if (val == EXP_EXT)
    {
	add_xused (XABSL, pc + 2L, 1, extexp) ;
	val = (saddr) 0 ;			/* FALLS INTO FOLLOWING */
    }
    if (val == EXP_ERR)
	val = (saddr) 0 ;			/* FALLS INTO FOLLOWING */
    if (val>15L)
	error (WRNIPP, "") ;      /* illegal pointer position */
    else gen_code[2] = hex ((int) val) ;
}


void stattest (char *modif)
{
    saddr val ;

    val = calc_expression (modif) ;
    if (val == EXP_EXT)
    {
	add_xused (XABSL, pc + 2L, 1, extexp) ;
	val = (saddr) 0 ;			/* FALLS INTO FOLLOWING */
    }
    if (val == EXP_ERR)
	    val = (saddr) 0 ;			    /* FALLS INTO FOLLOWING */
    if (val>15L)
	error (WRNISB, "") ;      /* illegal status bit */
    else gen_code[2] = hex ((int) val) ;
}


void setptr (char *modif)
{
    saddr val ;

    val = calc_expression (modif) ;
    if (val == EXP_EXT)
    {
	add_xused (XABSL, pc + (long int) gen_len - 1L, 1, extexp) ;
	val = (saddr) 0 ;
    }						/* FALLS INTO FOLLOWING */
    if (val == EXP_ERR)
	val = (saddr) 0 ;			/* FALLS INTO FOLLOWING */
    if (val>15L)
	error (WRNIPP, "") ;      /* illegal pointer position */
    else gen_code[gen_len-1] = hex ((int) val) ;
}


void setstat (char *modif)
{
    saddr val ;

    val = calc_expression (modif) ;
    if (val == EXP_EXT)
    {
	add_xused (XABSL, pc + 2L, 1, extexp) ;
	val = (saddr) 0 ;
    }						/* FALLS INTO FOLLOWING */
    if (val == EXP_ERR)
	val = (saddr) 0 ;			/* FALLS INTO FOLLOWING */
    if (val>15L)
	error (WRNISB, "") ;      /* illegal status bit */
    else gen_code[2] = hex ((int) val) ;
}


void dparith (char *modif)
{
    saddr val ;

    val = calc_expression (modif) ;
    if (val >= 0L)
    {
	if ((val<1L)||(val>16L))
	    error (WRNIDP, "") ;      /* illegal dp arithmetic value */
	else gen_code[2] = hex (val - 1L) ;
    }
    else if (val == EXP_EXT)
    {
	add_xused (XABSO, pc + 2L, 1, extexp) ;
	val = (saddr) 1 ;
    }
    else		 /* (val == EXP_ERR) */
    {
	val = (saddr) 1 ;
    }
}


void datatrans (char *modif)
{
    saddr val ;
    int r ;

    r=field_select (modif) ;
    if ((r==fs_A)||(r==fs_B))  gen_len = 3 ;
    if (passnb == 2)
    {
	switch (r)
	{
	    case 0 :	  /* expression instead of field selector */
		val = calc_expression (modif) ;
		if (val >= 0L)
		{
		    if ((val<1L)||(val>16))
			error (WRNTFR, "") ;  /* illegal transfer value */
		    else
		    {
			gen_code[2] = hex (dec(gen_code[2])+8) ;
			gen_code[3] = hex ((int) val - 1L) ;
		    }
		}
		else if (val == EXP_EXT)
		{
		    add_xused (XABSO, pc + 3L, 1, extexp) ;
		    gen_code[2] = hex (dec(gen_code[2])+8) ;  /* ajout */
		    val = 1L ;
		}
		else		 /* (val == EXP_EXT) */
		{
		    val = 1L ;
		}
		break ;
	    case fs_A :
		gen_len = 3 ;
		gen_code[1] = '4' ;
		gen_code[3] = EOL ;
		break ;
	    case fs_B :
		gen_len = 3 ;
		gen_code[1] = '4' ;
		gen_code[2] = hex ( dec(gen_code[2]) + 8) ;
		gen_code[3] = EOL ;
		break ;
	    default :
		gen_code[3] = hex (r-1) ;
		break ;
	}
    }
}


int hex_len (char *modif)	  /* aCLCHX : AS5 : ED094 */
{
    int i = 0 ;

    if (*modif=='#') modif++ ;
    while (((*modif>='0')&&(*modif<='9'))||
	   ((*modif>='A')&&(*modif<='F'))||
	   ((*modif>='a')&&(*modif<='f')))
    {
	modif++ ;
	i++ ;
    }
    return ((i>16) ? 16 : i) ;
}


void check_last_hex (char digit)
{
    if ((digit!=EOL)&&(digit!='\t')&&(digit!=' '))
    {
	if ((digit<'0')||
	    ((digit>'9')&&(digit<'A'))||
	    ((digit>'F')&&(digit<'a'))||
	    (digit>'f'))
	    error (WRNTMH, "") ;   /* too many hex digit present    */
	else error (WRNNHX, "") ;  /* non hexadecimal digit present */
    }
}


void nibhex (char *modif)
{
    int i ;

    gen_len = hex_len (modif) ;
    if (passnb==2)
    {
	if (*modif=='#') modif++ ;
	for (i=0; i<gen_len; i++)
	{
	    if (modif[i]>='a') gen_code[i] = modif[i] - 32 ;
	    else	       gen_code[i] = modif[i] ;
	}
	gen_code[gen_len] = EOL ;
	check_last_hex (modif[gen_len]) ;
    }
}


void lchex (char *modif)
{
    int i, j ;

    gen_len = hex_len (modif) ;
    if (passnb==2)
    {
	if (*modif=='#') modif++ ;
	if (gen_len)
	{
	    gen_code[1] = hex (gen_len - 1) ;
	    for (i=0, j=gen_len-1; i<gen_len; i++, j--)
	    {
		if (modif[j]>='a') gen_code[2+i] = modif[j] - 32 ;
		else		   gen_code[2+i] = modif[j] ;
	    }
	    gen_code[gen_len + 2] = EOL ;
	    check_last_hex (modif[gen_len]) ;
	}
	else
	{
	    gen_code[0] = EOL ;
	    error (WRNNHX, "") ; /* non hex present (in fact, there is no) */
	}
    }

    gen_len = (gen_len) ? gen_len+2 : 0 ;
}


void dxhex (char *modif)
{
    int i, j, r ;

    r = hex_len (modif) ;
    if (passnb==2)
    {
	if ((r!=2)&&(r!=4)&&(r!=5))
	    error (WRNIHX, "") ;  /* illegal hex const (number of digits) */
	else
	{
	    if (*modif=='#') modif++ ;
	    /* if 2, no change to the opcode */
	    /* if 4, add 1, if 5 add two     */
	    if (r!=2) gen_code[1] = hex (dec(gen_code[1]) + r - 3) ;
	    for (i=0, j=r-1; i<r; i++, j--)
	    {
		if (modif[j]>='a') gen_code[2+i] = modif[j] - 32 ;
		else		   gen_code[2+i] = modif[j] ;
	    }
	    gen_code[r+2] = EOL ;
	    check_last_hex (modif[r]) ;
	}
    }
    gen_len = ((r==4)||(r==5)) ? r+2 : 4 ;
}


int ascii_len (char *modif)
{
    char limit ;
    int i = 0 ;

    switch (*modif)
    {
	case '\'' :
	case '\\' :
	    limit = *modif++ ;
	    while ((*modif!=limit)&&(*modif))
	    {
		modif++ ;
		i++ ;
	    }
	    break ;
    }
    return ((i>8) ? 8 : i) ;
}


void nibasc (char *modif)
{
    char limit ;
    int i ;

    gen_len = ascii_len (modif) ;
    if (passnb==2)
    {
	limit = *modif ;
	for (i=1; i<=gen_len; i++)
	{
	    gen_code[2*(i-1)] = hex (((int) modif[i]) % 16) ;
	    gen_code[2*i-1] = hex (((int) modif[i]) / 16) ;
	}
	gen_code[2*gen_len] = EOL ;
	switch (modif[gen_len + 1])
	{
	    case '\'' :
	    case '\\' :
		if (modif[gen_len + 1]!=limit)
		    error  (WRNTMA, "") ; /* too many ascii chars present */
		break ;
	    case EOL :
		error (WRNASC, "") ;      /* illegal ascii constant       */
		break ;
	    default :
		error (WRNTMA, "") ;      /* too many ascii chars present */
		break ;
	}
    }
    gen_len *= 2 ;
}


void lcasc (char *modif)
{
    char limit ;
    int i ;

    gen_len = ascii_len (modif) ;
    if (passnb==2)
    {
	limit = *modif ;
	for (i=1; i<=gen_len; i++)
	{
	    gen_code[2*i]   = hex (((int) modif[gen_len - i + 1]) % 16) ;
	    gen_code[2*i+1] = hex (((int) modif[gen_len - i + 1]) / 16) ;
	}
	gen_code[2+2*gen_len] = EOL ;
	switch (modif[gen_len + 1])
	{
	    case '\'' :
	    case '\\' :
		if (modif[gen_len + 1]!=limit)
		    error  (WRNTMA, "") ; /* too many ascii chars present */
		break ;
	    case EOL :
		error (WRNASC, "") ;      /* illegal ascii constant       */
		break ;
	    default :
		error (WRNTMA, "") ;      /* too many ascii chars present */
		break ;
	}
	gen_code[1] = hex (2*gen_len - 1) ;
    }
    gen_len = (gen_len) ? 2+2*gen_len : 0 ;
}
