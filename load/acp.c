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
#include <time.h>

typedef unsigned short uint16 ;
typedef unsigned long uint32 ;

struct disc_hdr
{
    uint16  mlfi ;			/* Lif Id (0x8000) */
    char   l_label [6] ;		/* volume label */
    uint32 l_dstart ;			/* directory start record (usually 2) */
    uint16 l_sys3000 ;			/* system 3000 (0x1000) */
    uint16 l_spare1 ;			/* 0x0000 */
    uint32 l_dirlen ;			/* directory length */
    uint16 l_version ;			/* 0x0001 */
    uint16 l_spare2 ;			/* 0x0000 */
    uint32 trkpersurf ;			/* tracks per surface */
    uint32 surfpermed ;			/* number of surfaces */
    uint32 sectpertrk ;			/* records per track */
    char   l_date [6] ;			/* date & time */
    char   l_spare [213] ;		/* padding */
} ;

struct lif_filent
{
    char    l_filnam [10] ;		/* file name */
    uint16  l_filtyp ;			/* file type */
    uint32  l_startsec ;		/* start record */
    uint32  l_fillen ;			/* file length */
    char    l_create_time [6] ;		/* date & time created */
    uint16  l_flagwd ;			/* volume flag/number 0x8001 */
    uint32  l_startexec ;		/* implementation */
} ;

FILE *fp, *fplif, *fpw ;
char filename[10] ;
int type ;
long int dir_sec, data_sec ;
struct lif_filent dir_entry, areuh_dir ;
struct disc_hdr header ;
long int a = 0, b = 0, eod = 0 ;

#define DISK "/dev/dsk/floppy"

main (argc, argv)
int argc ;
char *argv[] ;
{
    long int magic ;

    if (argc!=2)
    {
        fprintf (stderr, "usage: acp file\n") ;
        exit (1) ;
    }

    if (!(fp=fopen (argv[1], "r")))
    {
        fprintf (stderr, "acp: cannot open file %s\n", argv[1]) ;
        exit (1) ;
    }
    fread (&magic, sizeof (long int), 1 , fp) ;
    if (magic!=0x1b080100)
    {
        fprintf (stderr, "acp: %s not a lex file\n", argv[1]) ;
        exit (1) ;
    }
    if (!(fplif=fopen (DISK, "r")))
    {
        fprintf (stderr, "acp: cannot open %s for read\n", DISK) ;
        exit (1) ;
    }
    if (!read_lif ())
    {
        fprintf (stderr, "acp: not a LIF disk\n") ;
        exit (1) ;
    }
    read_file_infos () ;
    search_dir () ;
    if (!(fpw=fopen (DISK, "w")))
    {
        fprintf (stderr, "acp: cannot open %s for write\n", DISK) ;
        exit (1) ;
    }
    write_data () ;
    write_dir () ;
    if (fclose (fplif))
    {
        fprintf (stderr, "acp: cannot close %s opened for read\n", DISK) ;
        exit (1) ;
    }
    if (fclose (fpw))
    {
        fprintf (stderr, "acp: cannot close %s opened for write\n", DISK) ;
        exit (1) ;
    }
    if (fclose (fp))
    {
        fprintf (stderr, "acp: cannot close file %s\n", argv[1]) ;
        exit (1) ;
    }
    exit (0) ;
}

skip (n)
int n ;
{
    char c ;
    int i ;

    for (i=1; i<=n; i++) fread (&c, 1, 1, fp) ;
}

con (pvar, n)
unsigned int *pvar ;
int n ;
{
    int i ;
    unsigned char areuh [256] ;

    fread (areuh, n, 1, fp) ;
    *pvar = 0 ;
    for (i=n-1; i>=0; i--)
    {
        *pvar = (*pvar * 16) + hex(areuh[i]) ;
    }
}

read_file_infos ()
{
    int i ;
    unsigned int c ;
    int length, length_sect ;

    for (i=0; i<8; i++)
    {
        con (&c, 2) ;
        dir_entry.l_filnam[i] = (unsigned char) c ;
    }
    dir_entry.l_filnam[8] = ' ' ;
    dir_entry.l_filnam[9] = ' ' ;
    con (&c, 4) ;
    dir_entry.l_filtyp = (unsigned short) c ;
    skip (12) ;
    con (&length, 5) ;
    length_sect = (length -= 5 ) ;

    read_date (dir_entry.l_create_time) ;

    dir_entry.l_startexec = 0 ;
    for (i=0; i<4; i++)
    {  
        c = length&0xff ;
        dir_entry.l_startexec = (((unsigned int) dir_entry.l_startexec)*0x100)
            + c ;
        length /= 256 ;
    }
 
    dir_entry.l_fillen = length_sect / 512 + ((length_sect%512) ? 1 : 0) ;
    dir_entry.l_flagwd = (unsigned short int) 0x8001 ;
}

read_date (str)
char *str ;
{
    int i ;
    long clock ;
    struct tm *ptm ;

    clock = time ((long *) 0) ;
    ptm = localtime (&clock) ;
    str [0] = bcd (ptm->tm_year) ;
    str [1] = bcd (ptm->tm_mon + 1) ;
    str [2] = bcd (ptm->tm_mday) ;
    str [3] = bcd (ptm->tm_hour) ;
    str [4] = bcd (ptm->tm_min) ;
    str [5] = '\0' ;
}

int bcd (n)
int n ;
{
    return (((n / 10) * 16) + (n % 10)) ;
}

int read_lif ()
{
    fread (&header, sizeof (header), 1, fplif) ;
    if (((unsigned short)header.mlfi)!=0x8000) return (0) ;
    return (1) ;
}

search_dir()
{
    struct lif_filent cur_dir_entry ;
    long int compteur = 1, sod, sodn, limite ;

    sod = header.l_dstart  + header.l_dirlen ;
    limite = ((unsigned) header.l_dirlen)*8 ;
    fseek (fplif, header.l_dstart*0x100, 0) ;
    fread (&cur_dir_entry, sizeof(cur_dir_entry), 1, fplif) ;
    while ((cur_dir_entry.l_filtyp != 0xffff)&&(compteur<=limite))
    {
        if ((cur_dir_entry.l_filtyp == 0) &&
            (cur_dir_entry.l_fillen >= dir_entry.l_fillen) && !a ) 
        {
            a = ftell (fplif) - 32 ;
            sodn = cur_dir_entry.l_startsec ;
        }
        else if (strncmp (cur_dir_entry.l_filnam, dir_entry.l_filnam, 10) == 0)
        {
            b = ftell (fplif) - 32 ;
            memcpy ((char *) &areuh_dir, (char *) &cur_dir_entry, 32) ;
        }
        sod = cur_dir_entry.l_startsec + cur_dir_entry.l_fillen ;
        fread (&cur_dir_entry, sizeof(cur_dir_entry), 1, fplif) ;
        compteur++ ;
    }
    if ((b) && (areuh_dir.l_fillen >= dir_entry.l_fillen))
    {
        a = b ;
        b = 0 ;
        sodn = areuh_dir.l_startsec ;
    }
    else if (!a)
    {
        if (compteur <= limite)
        {
            eod = ftell (fplif) ;
            a = eod - 32 ;
            sodn = sod ;
            if (sodn+((unsigned) dir_entry.l_fillen)>=
                                          ((unsigned) header.trkpersurf) *
                                          ((unsigned) header.surfpermed) *
                                          ((unsigned) header.sectpertrk))
            {
                fprintf (stderr, "acp: no room in data area\n") ;
                exit (1) ;
            }
        }
        else
        {
            fprintf (stderr, "acp: no room in directory area\n") ;
            exit (1) ;
        }
    }
    dir_entry.l_startsec = sodn ;
}

write_data ()
{
    int c = 0, d ;

    fseek (fpw, ((unsigned long) dir_entry.l_startsec)*0x100, 0) ;
    while ((d=getc (fp))!=EOF)
    {
        if (c)
        {
            fputc (hex(d)*16+hex(c), fpw) ;
            c = 0 ;
        }
        else c = d ;
    }
    if (c) fputc (hex(c), fpw) ;
}

int hex (c)
int c ;
{
    return (((c>='A')&&(c<='F'))?c-'A'+10:c-'0') ;
}

write_dir ()
{
    long int *plong, i ;

    update (a, &dir_entry) ;
    if (b)
    {
        areuh_dir.l_filtyp = 0 ;
        update (b, &areuh_dir) ;
    }
    if (eod)
    {
        plong = (long int *) &areuh_dir ;
        for (i=1; i<=8; i++)
        {
            *plong = -1 ;
            plong++ ;
        }
        update (eod, &areuh_dir) ;
    }
}

update (x, pdir)
long int x ;
struct lif_filent *pdir ;
{
    struct lif_filent sector[8] ;

/*
fclose(fpw) ; fpw = fopen (DISK, "w") ;
fclose(fplif) ; fplif = fopen (DISK, "r") ;
    fseek (fplif, x, 0) ;
    fread (sector, sizeof (sector), 1, fplif) ;
    write (sector, sizeof (sector), 1, fpw) ;
    memcpy (&sector[(x/32) % 8], pdir, 32) ;
    fseek (fpw, x, 0) ;
    fwrite (sector, sizeof (sector), 1, fpw) ;
*/

    fseek (fpw, x, 0) ;
    fwrite (pdir, sizeof *pdir, 1, fpw) ;
}
