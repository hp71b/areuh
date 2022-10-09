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
#include <string.h>

#include <stdio.h>		    /* Pour emmerder J.T. */

#define EOL '\0'		    /* End Of Line */
#define NSTR ""			    /* null string */
#define SPEC_CHAR '\\'		    /* special character */
#define MAXLEN 500		    /* input line max length */
#define MSGLEN 48		    /* message max length */
#define LBLMAX 7		    /* length of a sasm label */

#define BBMAX	64		    /* max number of building blocks */
#define MGMAX	255		    /* max number of messages	     */

#define ERRUSA	1		    /* usage: msg in_file out_file [n] */
#define ERRINP	2		    /* invalid input line */
#define ERRIVL	3		    /* invalid building block */
#define ERRTMB	4		    /* too many blocks */
#define ERRTMM	5		    /* too many messages */
#define ERRNSB	6		    /* no sufficient space for blocks */
#define ERRFST	7		    /* no valid first message */


#define VALUE(p) ((p)&((token)0xff))/* value of a token */
#define MFBB (token) 0x100	    /* MainFrame Building Block */
#define LLBB (token) 0x200	    /* LocaL Building Block */
#define ASCH (token) 0x400	    /* AScii CHaracter */
#define INST (token) 0x800	    /* INSerTion mark */

typedef short int token ;	    /* token = (type, value)  */

/* ascii representations of messages / building blocks */

char mftab[17][MAXLEN+1] ;	    /* mainframe building blocks */
char bbtab[BBMAX][MAXLEN+1] ;	    /* local building blocks table */
char mgtab[MGMAX][MAXLEN+1] ;	    /* message table */

/* tokenised representations for local messages / building blocks */
token bbtok[BBMAX][MAXLEN+1] ;	    /* local building blocks */
token mgtok[MGMAX][MAXLEN+1] ;	    /* messages */

/* length of local messages / building blocks */
short int bblen[BBMAX] ;
short int mglen[MGMAX] ;

/* symbolic labels for messages */
char lbltab[MGMAX][LBLMAX+1] ;

int mbase = 0 ;	  /* base for message numerotation */
int bbase ;	  /* base for building block numerotation */

int mdep ;	  /* relative message number (relative to mbase) */
int bdep ;	  /* idem for building blocks */

char line [MAXLEN+1] ;		    /* input line */
char *pline ;

int ln = 0 ;			    /* line number */

int ind16 ;			    /* msg # whose length is 16, 32, 48... */
#define INMG 0x100		    /* msg in mgtab */
#define INBB 0x200		    /* msg in bbtab */

static token read_block (void) ;
static void init (void) ;
static void pass1 (void) ;
static void between (void) ;
static void pass2 (void) ;
static void terminator () ;
static void header () ;
static void output_ascii () ;
static void build () ;
static int read_line (void);
static void print_msg (char tab [][MAXLEN+1], token tok [][MAXLEN+1], short int len [], int ind);
static int erreur (int) ;
static int find_block (char *areuh, char tab[][MAXLEN+1], int imax);

/******************************************************************************
				     MAIN

purpose : main program. see called procedures for information.
******************************************************************************/
int
main (int argc, char *argv [])
{
    if (argc>2) erreur(ERRUSA) ;
    else if (argc==2) mbase = atoi(argv[1]) ;

    init () ;
    pass1 () ;
    between () ;
    pass2 () ;
    exit (0) ;
}


/******************************************************************************
				     INIT

purpose : initializes "mainframe building blocks" list.
******************************************************************************/
static void
init (void)
{
    strcpy (mftab [0], "Illegal ") ;
    strcpy (mftab [1], " Expected") ;
    strcpy (mftab [2], " Not Found") ;
    strcpy (mftab [3], "Context") ;
    strcpy (mftab [4], "File") ;
    strcpy (mftab [5], " w/o ") ;
    strcpy (mftab [6], "Invalid ") ;
    strcpy (mftab [7], "Stat") ;
    strcpy (mftab [8], "Too ") ;
    strcpy (mftab [9], ": Align then ENDLN") ;
    strcpy (mftab[10], "Transform") ;
    strcpy (mftab[11], "Inf") ;
    strcpy (mftab[12], " Input") ;
    strcpy (mftab[13], " Ovfl") ;
    strcpy (mftab[14], "Pull") ;
    strcpy (mftab[15], "") ;		 /* insert message : ### of ### */
    strcpy (mftab[16], " Protect") ;
}


/******************************************************************************
				READ_LINE

synopsis : int read_line ()
description : reads a line from stdin
note : returns -1 if EOF reached, 0 otherwise
******************************************************************************/
static int
read_line (void)
{
    int c, i = -1 ;

    ln++ ;		    /* new line to be read */
    do
    {
	c = getchar () ;
	if (i<MAXLEN) line[++i] = (char) c ;
    }
    while ((c!=EOF)&&(c!='\n')) ;
    line [i] = EOL ;
    return ( (c==EOF) ? -1 : 0) ;
}


/******************************************************************************
				 BUILD_LABEL

purpose : scans the line, isolate the label if exits, otherwise generates it.
******************************************************************************/
static void
build_label (void)
{
    int i = 0 ;

    while ((*pline!=':')&&(*pline!=EOL))
    {
	if (i<LBLMAX) lbltab [mdep][i++] = *pline ;
	pline++ ;
    }
    if (*pline==EOL) erreur (ERRINP) ;
    pline++ ;
    if (i) lbltab [mdep][i] = EOL ;
    else sprintf (lbltab[mdep], "MSG%d", mbase + mdep) ;
}


/******************************************************************************
			     GET_CODE

synopsis : get_code ()
purpose : parses a number if possible (three digits < 256), and modify input
  line and its pointer (line/pline). If not possible, returns pline unchanged.
******************************************************************************/
static void
get_code (void)
{
    register char *pp ;
    register int code ;

    pp = pline ;
    code = (*(pp++)-'0') * 100 ;
    if ((*pp<'0') || (*pp>'9')) return ;
    code += (*(pp++)-'0') * 10 ;
    if ((*pp<'0') || (*pp>'9')) return ;
    code += (*pp)-'0' ;
    if (code>255) return ;
    pline = pp ;
    *pline = (char)code ;
    return ;
}


/******************************************************************************
			       PASS1

synopsis : pass1 ()
purpose : reads stdin and, for each line, parses it, creates local building
  blocks if any, and tokenises the message.
******************************************************************************/
static void
pass1 (void)
{
    int itok, ncar, iascii ;
    token bb ;

    mdep = 0 ;
    bdep = 0 ;
    while (read_line () == 0)
    {
	pline = line ;
	build_label () ;
	mglen [mdep] = 4 + 1 ;	 /* 4 for (blk len, msg #), 1 for blk term. */
	itok = 0 ;
	ncar = 0 ;
	iascii = 0 ;
	while (*pline)
	{
	    switch (*pline)
	    {
		case '^' :		/* insert text cell */
		    if (*(pline+1)==' ')
		    {
			pline++ ;	   /* trailing space */
			mgtok [mdep][itok++] = INST + 0xf3 ;
		    }
		    else
			mgtok [mdep][itok++] = INST + 0xf2 ;
		    mgtab [mdep][iascii++] = '^' ;
		    mglen [mdep] += 2 ;
		    ncar = 0 ;
		    break ;
		case '[' :
		    pline++ ;
		    bb = read_block () ;     /* bb  is a full token */
		    mgtok [mdep][itok++] = bb ;
		    mgtab [mdep][iascii] = EOL ;
		    strcat (mgtab[mdep], (bb&MFBB) ? mftab [VALUE(bb)-230]
						   : bbtab [VALUE(bb)] ) ;
		    iascii = strlen (mgtab[mdep]) ;
		    mglen [mdep] += 3 ;
		    ncar = 0 ;
		    break ;
		case SPEC_CHAR :
		    pline++ ;
		    if (*pline==EOL) break ;
		    if ((*pline=='0')||(*pline=='1')||(*pline=='2'))
			get_code () ;
		    // comment added for gcc:
		    // FALLTHRU
		default :
		    if (ncar == 0 ) mglen[mdep] ++ ;
		    ncar++ ;
		    mgtab [mdep][iascii++] = *pline ;
		    mgtok [mdep][itok++] = ASCH + (unsigned char) *pline ;
		    mglen [mdep] += 2 ;
		    if (ncar == 12) mglen [mdep] ++ ; /* cell > 11 char */
		    else if (ncar == 17)
		    {
			ncar = 1 ;
			mglen [mdep] ++ ;
		    }
		    break ;
	    }
	    pline++ ;
	}
	mgtab[mdep][iascii] = EOL ;
	mdep++ ;
	if (mdep>MGMAX) erreur (ERRTMM) ;
    }
}


/******************************************************************************
			      READ_BLOCK

synopsis : token read_block ()
purpose : reads a block in input line, finds it if already defined. Otherwise
  stores it in block tables. Returns a complete token.
******************************************************************************/
token
read_block (void)
{
    int iascii = 0 ;
    int ncar = 0 ;
    int x, itok = 0 ;

    bblen [bdep] = 5 ;
    while ((*pline)&&(*pline!=']'))
    {
	if (*pline==SPEC_CHAR)
	{
	    pline++ ;
	    if (*pline==EOL) break ;
	    if ((*pline=='0')||(*pline=='1')||(*pline=='2'))
		get_code () ;
	}
	if (*pline==EOL) erreur (ERRIVL) ;
	if (ncar==0) bblen [bdep] ++ ;
	ncar++ ;
	bbtab [bdep][iascii++] = *pline ;
	bbtok [bdep][itok++] = ASCH + *pline ;
	bblen [bdep] += 2 ;
	if (ncar == 12) bblen [bdep]++ ;
	else if (ncar == 17)
	{
	    ncar = 1 ;
	    bblen [bdep]++ ;
	}
	pline++ ;
    }
    if (*pline==EOL) erreur (ERRIVL) ;
    bbtab [bdep][iascii] = EOL ;
    bbtok [bdep][itok] = (token) 0 ;
    x = find_block (bbtab[bdep], mftab, 16) ;
    if (x>=0) return ((token)(MFBB + x + 230)) ;
    x = find_block (bbtab[bdep], bbtab, bdep-1) ;
    if (x>=0) return ((token)(LLBB + x)) ;
    if (bdep>=BBMAX) erreur (ERRTMB) ;
    return ((token)(LLBB + bdep++)) ;
}


/******************************************************************************
				 FIND_BLOCK

synopsis : int find_block (areuh, tab, imax)
	   char *areuh ;
	   char tab [][MAXLEN+1] ;
	   int imax ;
purpose : searches in table tab (mainframe / local building blocks) for 
  areuh building block. If found, return its index, otherwise -1.
******************************************************************************/
static int
find_block (char *areuh, char tab[][MAXLEN+1], int imax)
{
    int i ;

    for (i=0; i<=imax; i++)
	if (strcmp (tab[i], areuh)==0) return (i) ;
    return (-1) ;
}


/******************************************************************************
				   FIND_16

synopsis : int find_16 (xxlen, dep)
	   short int xxlen[] ;
	   int dep ;
description : returns the index of a message whose length is a multiple of 16,
  otherwise -1.
******************************************************************************/
static int
find_16 (short int xxlen [], int dep)
{
    int i = 0 ;

    while (i<dep)
	if (xxlen[i] % 16) i++ ;
	else break ;
    return ((i==dep) ? -1 : i) ;
}


/******************************************************************************
				   BETWEEN

synopsis : between ()
purpose : find a message length multiple of 16. If no one, then disp an error
  message and build a comment.
  Caclulate base for building blocks.
******************************************************************************/
static void
between (void)
{
    ind16 = find_16 (mglen, mdep) ;
    if (ind16>=0)
	ind16 += INMG ;
    else
    {
	ind16 = find_16 (bblen, bdep) ;
	if (ind16>=0)
	    ind16 += INBB ;
	else
	{
	    ind16 = 0 ;
	    printf ("**************************\n") ;
	    printf ("* E R R O R        ! ! ! *\n") ;
	    printf ("*                        *\n") ;
	    printf ("* no valid first message *\n") ;
	    printf ("**************************\n") ;
	}
    }
    if (mbase + mdep + bdep - 1 <= 255)
	bbase = mbase + mdep ;
    else
    {
	if (bdep < mbase)
	    bbase = 0 ;
	else erreur (ERRNSB) ; /* no sufficient space for blocks */
    }
}


/******************************************************************************
				     PASS2

synopsis : pass2 ()
purpose : sends output to stdout.
******************************************************************************/
static void
pass2 (void)
{
    int index ;

    header () ;

    index = ind16 & ~(INMG | INBB) ;
    switch (ind16 & (INMG | INBB))
    {
	case INMG :
	    print_msg (mgtab, mgtok, mglen, index) ;
	    break ;
	case INBB :
	    print_msg (bbtab, bbtok, bblen, index) ;
	    break ;
/*	default :    there is no valid first message */
    }

    if (ind16 & INMG)
    {
	build (mgtab, mgtok, mglen, 0, index - 1) ;
	build (mgtab, mgtok, mglen, index + 1, mdep - 1) ;
    }
    else build (mgtab, mgtok, mglen, 0, mdep - 1) ;

    if (ind16 & INBB)
    {
	build (bbtab, bbtok, bblen, 0, index - 1) ;
	build (bbtab, bbtok, bblen, index + 1, bdep - 1) ;
    }
    else build (bbtab, bbtok, bblen, 0, bdep - 1) ;

    terminator () ;
}


/******************************************************************************
				   PRINT_MSG

synopsis : print_msg (tab, tok, len, ind)
	   char tab [][MAXLEN+1] ;
	   token tok [][MAXLEN+1] ;
	   short int len [] ;
	   int ind ;
purpose : outputs message ind in table (tab, tok, len).
******************************************************************************/
static void
print_msg (char tab [][MAXLEN+1],
		token tok [][MAXLEN+1],
		short int len [],
		int ind)
{
    token c ;
    int msg, j, k ;
    char areuh [MAXLEN] ;

    msg = ((tab==mgtab) ? 1 : 0) ;
    printf ("\n") ;
    printf ("* %s\n", tab[ind]) ;	/* message text */
    printf ("%8sCON(2) %d\n", NSTR, len[ind]) ;	      /* length */

    printf ("%8sCON(2) ", NSTR) ;
    if (msg) printf ("%-7s", lbltab [ind]) ;
    else printf ("BB%-3d  ", bbase + ind ) ;
    printf ("  Message # %d\n", ((msg) ? mbase : bbase) + ind) ;

    j = 0 ;
    while (tok[ind][j])
    {
	switch ((tok[ind][j]) & (MFBB | LLBB | ASCH | INST) )
	{
	    case (int)MFBB :
		printf ("%8sCON(1) 14\n", NSTR) ;
		printf ("%8sCON(2) %d\n", NSTR, VALUE(tok[ind][j])) ;
		j++ ;
		break ;
	    case (int)LLBB :
		printf ("%8sCON(1) 13\n", NSTR) ;
		printf ("%8sCON(2) BB%d\n", NSTR, bbase + VALUE(tok[ind][j])) ;
		j++ ;
		break ;
	    case (int)INST :
		printf ("%8sNIBHEX %2x\n", NSTR, VALUE(tok[ind][j])) ;
		j++ ;
		break ;
	    case (int)ASCH :
		k = 0 ;
		while (((c=tok[ind][j+k])&ASCH) && (k<16))
		    areuh [k++] = VALUE (c) ;
		areuh [k] = EOL ;
		if (k<=8)
		{
		    printf ("%8sCON(1) %d\n", NSTR, k-1) ;
		    output_ascii (areuh, k) ;
		}
		else if (k<=11)
		{
		    printf ("%8sCON(1) %d\n", NSTR, k-1) ;
		    output_ascii (areuh, 8) ;
		    output_ascii (areuh+8, k-8) ;
		}
		else
		{
		    printf ("%8sCON(1) 11\n", NSTR) ;
		    printf ("%8sCON(1) %d\n", NSTR, k-1) ;
		    output_ascii (areuh, 8) ;
		    output_ascii (areuh+8, k-8) ;
		}
		j += k ;
	}
    }
    printf ("%8sCON(1) 12\n", NSTR) ;
}


/******************************************************************************
				  OUTPUT_ASCII

synopsis : output_ascii (str, len)
	   char *str ;
	   int len ;
purpose : outputs a string in sasm format (NIBASC / CON()), depending on the
  ascii value of characters.
******************************************************************************/
static void
output_ascii (char *str, int len)
{
    char buf[MAXLEN] ;
    char *pbuf ;

    pbuf = buf ;
    while (len--)
    {
	if ((*str >= 0x20) && (*str <= 0x7e) && (*str != '\''))
	{
	    *pbuf = *str ;
	    pbuf++ ;
	}
	else
	{
	    if (pbuf != buf)
	    {
		*pbuf = EOL ;
		printf ("%8sNIBASC '%s'\n", NSTR, buf) ;
		pbuf = buf ;
	    }
	    printf ("%8sCON(2) %d\n", NSTR, *str) ;
	}
	str++ ;
    }
    if (pbuf != buf)
    {
	*pbuf = EOL ;
	printf ("%8sNIBASC '%s'\n", NSTR, buf) ;
    }
}


/******************************************************************************
				    BUILD

synospis : build (tab, tok, len, n1, n2)
	   char tab [][MAXLEN+1] ;
	   token tok [][MAXLEN+1] ;
	   short int len [] ;
	   int n1, n2 ;
purpose : build a list of messages.
******************************************************************************/
static void
build (char tab[][MAXLEN+1],
            token tok[][MAXLEN+1],
	    short int len[],
	    int n1, int n2)
{
    int i ;
 
    for (i=n1; i<=n2; i++)
	print_msg (tab, tok, len, i) ;
}


/******************************************************************************
			       HEADER

synopsis : header ()
purpose : print table header, cad EQU table, lowest and highest msg #.
******************************************************************************/
static void
header (void)
{
    int i ;

    printf ("MBASE   EQU    %d\n", mbase) ;
    for (i=0; i<mdep; i++)
	printf ("%-7s EQU    (MBASE)+%-3d  %s\n", lbltab[i], i, mgtab[i]) ;

    if (bdep)
    {
	printf ("\n") ;
	for (i=0; i<bdep; i++)
	    printf ("BB%-3d   EQU    %-11d  %s\n",
		bbase + i, bbase + i, mgtab[i]) ;
    }

    printf ("\n") ;
    printf ("=MSGTBL\n") ;
    printf ("%8sCON(2) (MBASE)+0    Lowest message #\n", NSTR) ;
    printf ("%8sCON(2) (MBASE)+%-3d  Highest message #\n", NSTR, mdep-1) ;
}


/******************************************************************************
			       TERMINATOR

synospsis : terminator ()
purpose : print "NIBHEX FF"
******************************************************************************/
static void
terminator (void)
{
    printf ("\n") ;
    printf ("%8sNIBHEX FF       Table terminator\n", NSTR);
    if (ind16==0) erreur (ERRFST) ;
}


/******************************************************************************
				ERREUR

synopsis : erreur (errn)
purpose : declenche une erreur de numero errn
******************************************************************************/
static int
erreur (int errn)
{
    fprintf (stderr, "amg: error in line %d, ", ln) ;
    switch (errn)
    {
	case ERRUSA :
	    fprintf (stderr, "usage: amg [n]\n") ;
	    break ;
	case ERRINP :
	    fprintf (stderr, "invalid source line\n") ;
	    break ;
	case ERRIVL :
	    fprintf (stderr, "invalid building block\n") ;
	    break ;
	case ERRTMB :
	    fprintf (stderr, "too many blocks\n") ;
	    break ;
	case ERRTMM :
	    fprintf (stderr, "too many messages\n") ;
	    break ;
	case ERRNSB :
	    fprintf (stderr, "not enough space for blocks\n") ;
	    break ;
	case ERRFST :
	    fprintf (stderr, "no valid first message\n") ;
	    break ;
    }
    exit(2) ;
}
