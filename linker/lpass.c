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


extern void add_label (), add_unres () ;
extern long int calc_expression () ;
extern uchar *memoire () ;


/******************************************************************************

				   PASS1


synopsis : pass1 ()
description : processes the first pass on the object files. Open each file,
  read part one (code) and two (public label definitions), and close it.

******************************************************************************/

pass1 ()
{
    tmodule [1].m_ad = (saddr) 0 ;
    for (file=1; file<=nfile; file++)	ps_part12 () ;
	    /* still undefined labels ? */
    file = nfile + 1 ;
    add_label ("=FiLeNd", tmodule[file].m_ad, 0) ;
    if (head_unres)    pass1_bis () ;	    /* yes, try to resolve */
}


/******************************************************************************

				  PS_PART12


synopsis : ps_part12 ()
description : in first pass, process parts one and two of an object file, and
  store all necessary informations in tmodule array.
  name is the file name, i is the entry in tmodule array.

******************************************************************************/

ps_part12 ()
{
    FILE *fp ;
    uchar label [LBLLEN+2], def [MAXLEN+1], type ;
    saddr size, val ;
    long int part2, nl, magic ;
    int j, k ;

    if (!(fp = fopen (fname[file], RAO_MODE)))
	error (ERROPN, fname[file]) ;	  /* error opening file */
    fgetl (magic, fp) ;
    if ((magic<AOF_MAGIC)||(magic>AO_MAGIC))
	error (ERRNOA, fname[file]) ;	  /* not output of aas */
    if (magic!=AO_MAGIC)
	error (ERRICV, fname[file]) ;	  /* incompatible version */

    fgetl (part2, fp) ;
    fgetl (size, fp) ;			   /* length of code */
    tmodule[file+1].m_ad = tmodule[file].m_ad + size ;

    fseek (fp, part2, 0) ;
    fgetl (tmodule[file].m_part3, fp) ;
    fgetl (nl, fp) ;

    if (ferror (fp)) error (ERRRD, fname [file]) ;

    for (j=1; j<=nl; j++)
    {
	k = 0 ;
	while ((label [k] = (uchar) fgetc (fp)) != '\n') k++ ;
	label [k] = EOL ;
	fgetl (val, fp) ;
	if (ferror (fp)) error (ERRRD, fname [file]) ;
	if (val<0L)
	{
	    k = 0 ;
	    while ((def [k] = (uchar) fgetc (fp)) != '\n') k++ ;
	    def [k] = EOL ;
	    val = calc_expression (def) ;
	}
	else
	{
	    type = (uchar) getc (fp) ;
	    switch (type)
	    {
		case LABS :
		    break ;
		case LREL :
		    val += tmodule[file].m_ad ;
		    break ;
		case LUDF :
		default :
		    fprintf (stderr, "Internal error. Aie aie aie. LUDF\n") ;
		    exit (1) ;
	    }
	}
	if (val>=0L) add_label (label, val, 0) ;
	else add_unres (label, def) ;
    }
    if (fclose (fp)) error (ERRCLO, fname[file]) ;   /* error closing file */
}


/******************************************************************************

				PASS1_BIS


synopsis : pass1_bis ()
description : at the end of first pass, if still undefined labels, try to
  resolve all definitions. If not succesful, exit on error.

******************************************************************************/

pass1_bis ()
{
    struct unres *xdef ;
    saddr val ;

    passbis = 1 ;     /* signal to error message driver */

    while (head_unres)
    {
	file = head_unres->u_file ;
	val = calc_expression (head_unres->u_def) ;

	if (val<0) error (WRNURL, head_unres->u_label) ; /* unresolved label */
	else add_label (head_unres->u_label, val, 0) ;

	xdef = head_unres ;
	head_unres = head_unres->u_next ;
        free ((char *) xdef->u_def) ;
	free ((char *) xdef) ;
    }

    passbis = 0 ;
}


/******************************************************************************

				PASS2


synopsis : void pass2 ()
description : during pass two, we open each file, we read the code into a
  string variable properly dimensioned. Then, for each external reference
  usage, we store the proper value into the proper place (we hope so !).
  At last, we append the code to the final lex file. That's grehate.

******************************************************************************/

void pass2 ()
{
    FILE *fp ;
    uchar def [MAXLEN+1], *code ;
    int j ;
    long int size, bidon, nu, characteristic ;

    for (file=1; file<=nfile; file++)
    {
	fp = fopen (fname[file], RAO_MODE) ;
	if (!fp) error (ERROPN, fname[file]) ;
	fgetl (bidon, fp) ;
	fgetl (bidon, fp) ;
	fgetl (size, fp) ;
	code = memoire (size) ;
	fread ((char *) code, (int) size, 1, fp) ;

	fseek (fp, tmodule[file].m_part3, 0) ;
	fgetl (nu, fp) ;

	if (ferror (fp)) error (ERRRD, fname [file]) ;

	for (j=1; j<=nu; j++)
	{
	    read_usage (&characteristic, def, fp) ;
	    resolve_usage (characteristic, def, fp, code) ;
	}

	fwrite (code, (int) size, 1, fplex) ;
	free ((char *) code) ;
	if (ferror (fplex)) error (ERRWRT, flex) ;
	if (fclose (fp)) error (ERRCLO, fname[file]) ;
    }
}
