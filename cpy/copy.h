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

#include <stdio.h>

long int con () ;

typedef unsigned char uchar ;

typedef unsigned long int  int32 ;
typedef unsigned short int int16 ;

#define ERRUSA	       1		/* usage */
#define ERROPN	       2		/* cannot open */
#define ERRNAL	       3		/* not a Lex */
#define ERRCLO	       4		/* cannot close */
#define ERRRD	       5		/* error reading */
#define ERRWRT	       6		/* error writing */
#define ERRLEN	       7		/* bad REL(5) FiLeNd field */
#define ERRINT	       8		/* interface error */

uchar	*file ;

uchar	name [10] ;
FILE	*fp ;
int16	ftype ;
int32	lnib, lbyte, lsect ;

uchar	*pgm ;				/* name of current program */

main (argc, argv)
int argc ;
char *argv[] ;
{
    long int magic ;
    int32 l ;
    uchar tab [4] ;
    int i, j, m ;
    int mod ;

    pgm = (uchar *) argv [0] ;		/* program name */

    switch (argc)
    {
	case 1 :
	    file = (uchar *) "lex" ;	/* default name */
	    break ;
	case 2 :
	    file = (uchar *) argv [1] ;
	    break ;
	default :
	    error (ERRUSA, "") ;
	    break ;
    }

    fp = fopen (file, "rb") ;		/* binary file for MS-DOS machines */
    if (fp == NULL)
	error (ERROPN, file) ;

/* get Lex file header */

    fread ((char *) &magic, sizeof (long int), 1 , fp) ;
    if (magic!=0x1b080100)
	error (ERRNAL, file) ;		/* not a Lex file */

    for (i=0; i<8; i++)
	name [i] = (uchar) con (2) ;
    name [8] = name [9] = ' ' ;

    ftype = (int32) con (4) ;		/* file type */

    skip (12) ; 			/* skip date and time */

    lnib = (int32)con (5) - (int32)5 ;	/* get length */
    lsect = (lnib + 511) / 512 ;

    if (ferror (fp))			/* read error */
	error (ERRRD, file) ;

/* initialize output channel */

    init () ;

/* header output */

    for (i=0; i<10; i++)		/* file name */
	output ((uchar) name [i]) ;
    output ((uchar) (ftype / 256)) ;	/* file type */
    output ((uchar) (ftype % 256)) ;
    for (i=1; i<=4; i++)		/* start sector */
	output ((uchar) 0) ;
    l = lsect ;
    for (i=3; i>=0; i--)		/* length in sectors */
    {
	tab [i] = (uchar) l & 0xff ;
	l >>= 8 ;
    }
    for (i=0; i<4; i++)
	output (tab [i]) ;
    for (i=0; i<6; i++)			/* date and time */
	output ((uchar) 0) ;
    output ((uchar) 0x80) ;		/* 80 */
    output ((uchar) 0x01) ;		/* 01 */
    l = lnib ;
    for (i=0; i<4; i++) 		/* length in nibbles */
    {
	output ((uchar) (l & (int32) 0xff)) ;
	l >>= 8 ;
    }

/* data output */

    lbyte = (lnib+1) >> 1 ;
    mod = lbyte % 256 ;
    for (i=1; i<=lsect; i++)
    {
	m = (i == lsect && mod != 0) ? min (256, mod) : 256 ;
	for (j=0; j<m ; j++)
	    output ((uchar) con (2)) ;
    }
    for (j=m; j<256; j++)
	output ((uchar) 0) ;

/* end */

    if (getc (fp) != EOF)		/* bad file len */
	error (ERRLEN, file) ;
    term () ;

    exit (0) ;
}

long int con (n)
int n ;
{
    int i ;
    long int res ;
    uchar buf [1024] ;
    int first ;

    res = 0 ;
    first = 1 ;
    for (i=0; i<n; i++)
    {
	buf [i] = (uchar) getc (fp) ;
	if (first && feof (fp))
	    error (ERRLEN, "") ;
	first = 0 ;
    }
    if (ferror (fp))
	error (ERRRD, file) ;
    for (i=n-1; i>=0; i--)
	res = (res << 4) | hex (buf [i]) ;
    return res ;
}

skip (n)
int n ;
{
    uchar buf [1024] ;

    fread (buf, n, 1, fp) ;
    if (ferror (fp))
	error (ERRRD, file) ;
    if (feof (fp))
	error (ERRLEN, "") ;
}

int min (a, b)
int a, b ;
{
    return a <= b ? a : b ;
}

int hex (c)
uchar c ;
{
    return (c >= 'A' && c <= 'F') ? c - 'A' + 10 : c - '0' ;
}

error (code, str)
int code ;
uchar *str ;
{
    switch (code)
    {
	case ERRUSA :	    /* usage */
	    fprintf (stderr, "usage: %s [file]\n", pgm) ;
	    break ;
	case ERROPN :	    /* cannot open */
	    fprintf (stderr, "%s: cannot open %s\n", pgm, str) ;
	    break ;
	case ERRNAL :	    /* not a Lex */
	    fprintf (stderr, "%s: %s is not a lex file\n", pgm, str) ;
	    break ;
	case ERRCLO :	    /* cannot close */
	    fprintf (stderr, "%s: cannot close %s\n", pgm, str) ;
	    break ;
	case ERRRD  :	    /* error reading */
	    fprintf (stderr, "%s: error reading %s\n", pgm, str) ;
	    break ;
	case ERRWRT :	    /* error writing */
	    fprintf (stderr, "%s: error writing %s\n", pgm, str) ;
	    break ;
	case ERRLEN :	    /* bad REL(5) FiLeNd field */
	    fprintf (stderr, "%s: bad REL(5) FiLeNd field\n", pgm) ;
	    break ;
	case ERRINT :	    /* interface error */
	    fprintf (stderr, "%s: interface error\n", pgm) ;
	    break ;
	default :
	    fprintf (stderr, "%s: internal error\n", pgm) ;
	    break ;
    }
    exit (1) ;
}
