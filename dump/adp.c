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

#include <stdlib.h>

#include "common.h"

#define fgetl(lg,fp)  fread(&(lg),sizeof(long int),1,fp)

void format_hex () ;

int main (int argc, char *argv [])
{
    FILE *fp ;
    char line [MAXLEN + 1], hexvar[MAXLEN+1], type ;
    saddr magic, pc, p2, p3, nu, nl, characteristic, value ;

    int i, j ;

    if (argc!=2)
    {
	fprintf (stderr, "usage: adp file\n") ;
	exit (1) ;
    }

    fp = fopen (argv[1], RAO_MODE) ;
    if (fp==NULL)
    {
	fprintf (stderr, "adp: error opening %s\n", argv[1]) ;
	exit (1) ;
    }

    printf ("file %s : ", argv[1]) ;
    fgetl (magic, fp) ;
    if ((magic<AOF_MAGIC)||(magic>AO_MAGIC))
    {
	fprintf (stderr, "adp: %s is not an Areuh object file\n", argv [1]) ;
	exit (1) ;
    }
    printf ("version %ld\n", magic - AOF_MAGIC) ;
    if (magic!=AO_MAGIC)
    {
	fprintf (stderr, "Wrong version\n") ;
	exit (1) ;
    }
    fgetl (p2, fp) ;
    fgetl (pc, fp) ;

    printf ("code: length (in nibbles) = %ld\n", pc) ;

    fseek (fp, p2, 0) ;
    fgetl (p3, fp) ;
    fgetl (nl, fp) ;

    printf ("public definitions : number = %ld\n", nl) ;
    for (i=1; i<=nl; i++)
    {
	printf ("%4d:", i) ;
	j = 0 ;
	while ((line [j] = (char) getc (fp)) != '\n') j++ ;
	line [j]= EOL ;
	printf (" %-13s, val = ", line) ;     /* LBLLEN + 1 */
	fgetl (value, fp) ;
	if (value >= (saddr) 0)
	{
	    hex5 (hexvar, value) ;
	    printf ("%s, ", hexvar) ;
	    type = getc (fp) ;
	    switch (type)
	    {
		case LABS : printf ("Abs") ; break ;
		case LREL : printf ("Rel") ; break ;
		case LUDF : printf ("Udf") ; break ;
		default : printf ("Type aie aie aie") ; break ;
	    }
	}
	else if (value == LBL_UDF)
	{
	    printf ("-UDF-") ;
	}
	else if (value == LBL_IVL)
	{
	    printf ("-IVL-") ;
	}
	else if (value == LBL_EXT)
	{
	    printf ("-EXT-") ;
	}
	else if (value == LBL_XEQ)
	{
	    printf ("-XEQ-") ;
	    fscanf (fp, "%s\n", line) ;
	    printf (", definition = %s", line) ;
	}
	else		    /* (value == LBL_SEQ) */
	{
	    printf ("-SEQ-") ;
	}
	printf ("\n") ;
    }

    fgetl (nu, fp) ;
    printf ("references not resolved: number = %ld\n", nu) ;
    for (i=1; i<=nu; i++)
    {
	fgetl (characteristic, fp) ;
	fgetl (pc, fp) ;
	fscanf (fp, "%s\n", line) ;
	printf ("%4d:", i) ;
	hex5 (hexvar, pc) ;
	printf (" pc = %s", hexvar) ;
	printf (", type = ") ;
	switch ((int) (characteristic & 0xf0))
	{
	    case XABSL : printf ("XABSL") ; break ;
	    case XABSO : printf ("XABSO") ; break ;
	    case XRGTO : printf ("XRGTO") ; break ;
	    case XRGSB : printf ("XRGSB") ; break ;
	}
	printf (", length = %1ld", characteristic & 0xf) ;
	printf (", val = %s\n", line) ;
    }
}


/* function called indirectly, via hex5 */

void format_hex (char *str, saddr val, int dig)
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
