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

extern char *malloc () ;
extern saddr calc_expression () ;
uchar *memoire () ;

/******************************************************************************

				  FIND_LABEL

synopsis : struct symbol *find_label (label)
	   char *label
description : find address of label, return address of structure describing it.

******************************************************************************/

struct symbol *find_label (label)
char *label ;
{
    struct symbol *p ;
    int test ;

    p = h_label [label[1]]->s_next ;
    while (p)
    {
	test = strcmp (label, p->s_name) ;
	if (test==0)  break ;
	p = p->s_next ;
	if (test<0) p = (struct symbol *) NULL ;
    }
    return (p) ;
}


/******************************************************************************

				   ADD_LABEL


synopsis : void add_label (label, val, os)
	   uchar *label
	   saddr val
	   int os
description : add a label in label table and, if already defined, add it to
  multiply defined lists.

******************************************************************************/

void add_label (label, val, os)
uchar *label ;
saddr val ;
int os ;
{
    uchar error_text [MAXLEN+1] ;
    struct symbol *p, *s ;
    int b = 1 ;

    p = find_label (label) ;
    if (p)
    {
	sprintf (error_text, "%s in files %s and %s", label,
			     fname[p->s_file],
			     fname[file]) ;
	error (WRNDUP, error_text) ;	       /* duplicate label */
    }
    else
    {
	s = p = h_label [label[1]] ;
	while ((b)&&(s))
	{
	    if (strcmp (label, s->s_name)<0) b = 0 ;
	    else
	    {
		p = s ;  s = s->s_next ;
	    }
	}
	s = (struct symbol *) memoire (sizeof (struct symbol)) ;
	s->s_next = p->s_next ;
	p->s_next = s ;
	strcpy (s->s_name, label) ;
	s->s_xref = (struct xtable *) NULL ;
	p = s ;
    }
    p->s_value = val ;
    p->s_file = (uchar) file ;
    p->s_os = os ;
}


/******************************************************************************

				ADD_UNRES


synopsis : void add_unres (label, def)
	   char *label, *def
	   int file
description : during first part of pass one, if a label is undefined, add it
  to a special list. During second part, if this list is scaned. If value
  is then defined, the label will be removed. At the end of first pass, this
  queue will be the undefined labels queue, which should be empty...

******************************************************************************/

void add_unres (label, def)
uchar *label, *def ;
{
    struct unres *x ;

    x = (struct unres *) memoire (sizeof (struct unres)) ;
    strcpy (x->u_label, label) ;
    x->u_file = (uchar) file ;
    x->u_def = memoire (strlen (def) + 1) ;
    strcpy (x->u_def, def) ;
    x->u_next = head_unres ;
    head_unres = x ;
}


/******************************************************************************

				  READ_USAGE


synopsis : void read_usage (characteristic, def, fp)
	   long int *characteristic
	   char *def
	   FILE *fp
description : read an entry in third part of object file fp.

******************************************************************************/

void read_usage (characteristic, def, fp)
long int *characteristic ;
char *def ;
FILE *fp ;
{
    int i ;

    fread (characteristic, sizeof (long int), 1, fp) ;
    fread (&pc, sizeof (saddr), 1, fp) ;
    i = 0 ;
    while ((def [i] = (uchar) fgetc (fp)) != '\n') i++ ;
    def [i] = EOL ;
    if (ferror (fp)) error (ERRRD, fname [file]) ;
}


int lrange (offset, nibs)
saddr *offset ;
int nibs ;
{
    saddr Sixtine, Fiftine ;
    int r ;

    Sixtine = ((saddr) 1) << (nibs*4) ;
    Fiftine = Sixtine >> 1 ;
    r = ((*offset>=-Fiftine)&&(*offset<Fiftine)) ;
    if (*offset<0) *offset += Sixtine ;
    return (r) ;
}


/******************************************************************************

			      RESOLVE_USAGE


synopsis : void resolve_usage (characteristic, def, file, fp, code)
	   long int characteristic
	   char *def, *code
	   FILE *fp
	   int file
description : make all the work which is to be made by a linker. Resolve
  all usages.

******************************************************************************/

void resolve_usage (characteristic, def, fp, code)
saddr characteristic ;
uchar *def, *code ;
FILE *fp ;
{
    uchar hexa [MAXLEN+1] ;
    saddr type, source, dest, offset ;
    int i, j ;

    type = characteristic & (XABSL | XABSO | XRGTO | XRGSB) ;
    characteristic &= ~type ;
    dest = calc_expression (def) ;
    if (dest < (saddr) 0)
    {
	error (WRNURF, def) ;	  /* unresolved reference */
	dest = (saddr) 0 ;
    }
    switch ((int) type)
    {
	case XABSL :
	    source = 0 ;
	    break ;
	case XABSO :
	    source = 1 ;
	    break ;
	case XRGTO :
	    source = tmodule [file].m_ad + pc  ;
	    break ;
	case XRGSB :
	    source = tmodule [file].m_ad + pc  + characteristic ;
	    break ;
    }
    offset = dest - source ;
    if ((type==XRGTO)||(type==XRGSB))
    {
	if (!lrange (&offset, characteristic))
	{
	    offset = (saddr) 0 ;
	    error (WRNJVL, def) ;    /* Jump or value too large */
	}
    }
    hex6 (hexa, offset) ;
    for (i=0, j=5; i<characteristic; i++, j--)
    {
	code [pc+i] = hexa[j] ;
    }
}


/******************************************************************************

				SYMBOL_VALUE


synopsis : saddr symbol_value (label)
	   uchar *label
description : fetch a label, and return its value.

******************************************************************************/

saddr symbol_value (label)
uchar *label ;
{
    struct symbol *ad ;
    struct xtable *x, *y, *z ;

    ad = find_label (label) ;

    if ((ad)&&(passnb==2)&&xref)
    {
	x = (struct xtable *) memoire (sizeof (struct xtable)) ;
	x->x_pc = pc ;
	x->x_file = file ;
	z = y = ad->s_xref ;
	while ((y)&&(y->x_file<file))
	{
	    z = y ;
	    y = y->x_next ;
	}
	if (y==z) ad->s_xref = x ;	/* insert at begining of the list */
	else z->x_next = x ;		/* insert in middle of list */
	x->x_next = y ;
	ad->s_os = 0 ;
    }
    return ((ad) ? ad->s_value : EXP_EXT) ;
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

				 FORMAT_HEX


synopsis : void format_hex (str, val, dig)
	   uchar *str
	   saddr val
	   int dig
description : stores into str the hexadecimal string representing the dig
	      low order hex digits of val.

******************************************************************************/

format_hex (str, val, dig)
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
