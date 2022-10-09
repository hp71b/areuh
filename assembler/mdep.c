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

      M A C H I N E    D E P E N D E N C I E S

******************************************************************************/

#include "flag.h"

#if ASSEMBLER
#include "aglobal.h"
#else
#include "lglobal.h"
#endif

extern struct symbol *add_label() ;

#if HPUX

void format_time (char *str)
{
    long int l ;

    extern long int time () ;
    extern char *ctime () ;

    l = time (0L) ;
    strcpy (str, ctime (&l)) ;
    str [strlen (str) - 1] = EOL ;
}

char *tab[] = { "",
		"/usr/lib/",
		"/usr/local/lib/",
		"/lib/",
		"/hp71/lib/",
		"/local/lib/",
		0 } ;

void load_file (file)
char *file ;
{
    int i = 0 ;
    char name [MAXLEN+1] ;
    saddr val ;
    FILE *fp ;

    fp = (FILE *) NULL ;
    while ((tab[i])&&(!fp))
    {
	sprintf (name, "%s%s", tab[i++], file) ;
	fp = fopen (name, "r") ;
    }
    if (!fp) error (ERROPN, file) ;

#if LINKER
    file = 0 ;
#endif
    while (fscanf (fp, "%s\n%lX\n", name, &val) != EOF)
    {
#if ASSEMBLER
	add_label (name, val, "", LABS, 1) ;
#else
	add_label (name, val, 1) ;
#endif
    }
    if (ferror (fp)) error (ERRWRT, file) ;
    if (fclose (fp)) error (ERRCLO, file) ;
}

#include <sys/types.h>
#include <sys/stat.h>

/* look for object file (object code or listing file)
 *   if non existent, ok
 *   if exists and not directory, ok
 *   if exists and directory, then append "<dir>/<default>" */

void look_obj (char *fname, char *dfl)
{
    struct stat buf ;

    if (*fname == EOL)		   /* if fname == "" then default it */
	strcpy (fname, dfl) ;

    if (!stat (fname, &buf))	   /* file exists. Is it a directory ? */
    {
	if ((buf.st_mode & S_IFMT) == S_IFDIR)
	    sprintf (fname, "%s/%s", fname, dfl) ;
    }
}

/* build a default file name, based on "source" basename and a given
 * default extension.
 */

void dfl_extension (char *object, char *source, char *extension)
{
    char *pname ;

    strcpy (object, source) ;
    pname = object ;
    while ((*pname)&&(*pname!='.')) pname++ ;
    if (*pname==EOL) *pname = '.' ;
    strcpy (pname+1, extension) ;
}

#endif	   /* HPUX */


#if ATARI_LATTICE

char skipvar ;

void format_time (str)
char *str ;
{
    strcpy (str, "Areuh Tagada Bouzouh bouzouh areuh areuh... et toc !") ;
}

char *optarg ;
int optind = 0, opterr = 0 ;

int getopt (argc, argv, optstr)
int argc ;
char *argv[], *optstr ;
{
    char *o, car ;
    static char *index = 0 ;
    extern char *strchr () ;

    if ((index==(char *)0)||(*(index+1)==0))
    {
	if (++optind>argc-1) return (EOF) ;
	index = argv[optind] ;
	if (*index!='-') return (EOF) ;
    }
    car = *(++index) ;	      /* state 6 */
    if (!(o = strchr (optstr, car))) return ('?') ;
    if (*(o+1)!=':')     return ((int) car) ;
    if (*(index+1)) optarg = index+1 ;
    else
    {
	if (++optind>argc-1) return (EOF) ;
	else optarg = argv[optind] ;
    }
    index = (char *) 0 ;
    return ((int) car) ;
}

char *tab[] = { "",
		"A:",
		"A:\\TABLE\\",
		0 } ;

void load_file (file)
char *file ;
{
    int i = 0 ;
    char name [MAXLEN+1] ;
    saddr val ;
    FILE *fp ;

    fp = (FILE *) NULL ;
    while ((tab[i])&&(!fp))
    {
	sprintf (name, "%s%s", tab[i++], file) ;
	fp = fopen (name, "r") ;
    }
    if (!fp) error (ERROPN, file) ;

#if LINKER
    file = 0 ;
#endif
    while (fscanf (fp, "%s\n%x\n", name, &val) != EOF)
    {
#if ASSEMBLER
	add_label (name, val, "", LABS, 1) ;
#else
	add_label (name, val, 1) ;
#endif
    }
    if (fclose (fp)) error (ERRCLO, file) ;
}

#endif	  /* ATARI_LATTICE */


#if PC_MSC

char skipvar ;

void format_time (str)
char *str ;
{
    long int l ;

    extern long int time () ;
    extern char *ctime () ;

    time (&l) ;
    strcpy (str, ctime (&l)) ;
    str [strlen (str) - 1] = EOL ;
}

char *optarg ;
int optind = 0, opterr = 0 ;

int getopt (argc, argv, optstr)
int argc ;
char *argv[], *optstr ;
{
    char *o, car ;
    static char *index = 0 ;
    extern char *strchr () ;

    if ((index==(char *)0)||(*(index+1)==0))
    {
	if (++optind>argc-1) return (EOF) ;
	index = argv[optind] ;
	if (*index!='-') return (EOF) ;
    }
    car = *(++index) ;	      /* state 6 */
    if (!(o = strchr (optstr, car))) return ('?') ;
    if (*(o+1)!=':')     return ((int) car) ;
    if (*(index+1)) optarg = index+1 ;
    else
    {
	if (++optind>argc-1) return (EOF) ;
	else optarg = argv[optind] ;
    }
    index = (char *) 0 ;
    return ((int) car) ;
}

char *tab[] = { "",
		"c:",
		"c:\\hp71\\",
		"c:\\lib\\hp71\\",
		"c:\\lib\\",
		"c:\\areuh\\lib\\",
		0 } ;

void load_file (file)
char *file ;
{
    int i = 0 ;
    char name [MAXLEN+1] ;
    saddr val ;
    FILE *fp ;

    fp = (FILE *) NULL ;
    while ((tab[i])&&(!fp))
    {
	sprintf (name, "%s%s", tab[i++], file) ;
	fp = fopen (name, "r") ;
    }
    if (!fp) error (ERROPN, file) ;

#if LINKER
    file = 0 ;
#endif
    while (fscanf (fp, "%s\n%X\n", name, &val) != EOF)
    {
#if ASSEMBLER
	add_label (name, val, "", LABS, 1) ;
#else
	add_label (name, val, 1) ;
#endif
    }
    if (fclose (fp)) error (ERRCLO, file) ;
}

#include <sys\types.h>
#include <sys\stat.h>

/* look for object file (object code or listing file)
 *   if non existent, ok
 *   if exists and not directory, ok
 *   if exists and directory, then append "<dir>/<default>" */

look_obj (fname, dfl)
char *fname, *dfl ;
{
    struct stat buf ;

    if (*fname == EOL)		   /* if fname == "" then default it */
	strcpy (fname, dfl) ;

    if (!stat (fname, &buf))	   /* file exists. Is it a directory ? */
    {
	if ((buf.st_mode & S_IFMT) == S_IFDIR)
	    sprintf (fname, "%s\\%s", fname, dfl) ;
    }
}

/* build a default file name, based on "source" basename and a given
 * default extension.
 */

dfl_extension (object, source, extension)
char *object, *source, *extension ;
{
    char *pname ;

    strcpy (object, source) ;
    pname = object ;
    while ((*pname)&&(*pname!='.')) pname++ ;
    if (*pname==EOL) *pname = '.' ;
    strcpy (pname+1, extension) ;
}

#endif	  /* PC_MSC */

#if MACOS

void format_time (char *str)
{
    long int l ;

    extern long int time () ;
    extern char *ctime () ;

    l = time (0L) ;
    strcpy (str, ctime (&l)) ;
    str [strlen (str) - 1] = EOL ;
}

char *tab[] = { "",
		"/usr/local/share/",
		0 } ;

void load_file (file)
char *file ;
{
    int i = 0 ;
    char name [MAXLEN+1] ;
    saddr val ;
    FILE *fp ;

    fp = (FILE *) NULL ;
    while ((tab[i])&&(!fp))
    {
	sprintf (name, "%s%s", tab[i++], file) ;
	fp = fopen (name, "r") ;
    }
    if (!fp) error (ERROPN, file) ;

#if LINKER
    file = 0 ;
#endif
    while (fscanf (fp, "%s\n%lX\n", name, &val) != EOF)
    {
#if ASSEMBLER
	add_label (name, val, "", LABS, 1) ;
#else
	add_label (name, val, 1) ;
#endif
    }
    if (ferror (fp)) error (ERRWRT, file) ;
    if (fclose (fp)) error (ERRCLO, file) ;
}

#include <sys/types.h>
#include <sys/stat.h>

/* look for object file (object code or listing file)
 *   if non existent, ok
 *   if exists and not directory, ok
 *   if exists and directory, then append "<dir>/<default>" */

void look_obj (char *fname, char *dfl)
{
    struct stat buf ;

    if (*fname == EOL)		   /* if fname == "" then default it */
	strcpy (fname, dfl) ;

    if (!stat (fname, &buf))	   /* file exists. Is it a directory ? */
    {
	if ((buf.st_mode & S_IFMT) == S_IFDIR)
	    sprintf (fname, "%s/%s", fname, dfl) ;
    }
}

/* build a default file name, based on "source" basename and a given
 * default extension.
 */

void dfl_extension (char *object, char *source, char *extension)
{
    char *pname ;

    strcpy (object, source) ;
    pname = object ;
    while ((*pname)&&(*pname!='.')) pname++ ;
    if (*pname==EOL) *pname = '.' ;
    strcpy (pname+1, extension) ;
}

#endif	   /* MACOS */

#if FREEBSD

void format_time (char *str)
{
    long int l ;

    extern long int time () ;
    extern char *ctime () ;

    l = time (0L) ;
    strcpy (str, ctime (&l)) ;
    str [strlen (str) - 1] = EOL ;
}

char *tab[] = { "",
		"/usr/lib/",
		"/usr/local/lib/",
		"/lib/",
		"/hp71/lib/",
		"/local/lib/",
		0 } ;

void load_file (file)
char *file ;
{
    int i = 0 ;
    char name [MAXLEN+1] ;
    saddr val ;
    FILE *fp ;

    fp = (FILE *) NULL ;
    while ((tab[i])&&(!fp))
    {
	sprintf (name, "%s%s", tab[i++], file) ;
	fp = fopen (name, "r") ;
    }
    if (!fp) error (ERROPN, file) ;

#if LINKER
    file = 0 ;
#endif
    while (fscanf (fp, "%s\n%lX\n", name, &val) != EOF)
    {
#if ASSEMBLER
	add_label (name, val, "", LABS, 1) ;
#else
	add_label (name, val, 1) ;
#endif
    }
    if (ferror (fp)) error (ERRWRT, file) ;
    if (fclose (fp)) error (ERRCLO, file) ;
}

#include <sys/types.h>
#include <sys/stat.h>

/* look for object file (object code or listing file)
 *   if non existent, ok
 *   if exists and not directory, ok
 *   if exists and directory, then append "<dir>/<default>" */

void look_obj (char *fname, char *dfl)
{
    struct stat buf ;

    if (*fname == EOL)		   /* if fname == "" then default it */
	strcpy (fname, dfl) ;

    if (!stat (fname, &buf))	   /* file exists. Is it a directory ? */
    {
	if ((buf.st_mode & S_IFMT) == S_IFDIR)
	    sprintf (fname, "%s/%s", fname, dfl) ;
    }
}

/* build a default file name, based on "source" basename and a given
 * default extension.
 */

void dfl_extension (char *object, char *source, char *extension)
{
    char *pname ;

    strcpy (object, source) ;
    pname = object ;
    while ((*pname)&&(*pname!='.')) pname++ ;
    if (*pname==EOL) *pname = '.' ;
    strcpy (pname+1, extension) ;
}

#endif	   /* FREEBSD */
