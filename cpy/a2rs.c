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

#include "copy.h"
#include <dos.h>

union REGS inregs, outregs ;

init ()
{
    inregs.h.ah = 0 ;
    inregs.h.al = 0xe3 ;
    inregs.x.dx = 0 ;		    /* com1 */
    int86 (0x14, &inregs, &outregs) ;
}

output (c)
char c ;
{
    int ok ;

    ok = 0 ;
    while (!ok)
    {
	inregs.h.ah = 0x03 ;			/* get line status */
	inregs.x.dx = 0x0000 ;
	int86 (0x14, &inregs, &outregs) ;
	ok = (outregs.h.al & 0x30) == 0x30 ;	/* handshake lines */
    }
    inregs.h.ah = 1 ;				/* send char */
    inregs.h.al = c ;
    inregs.x.dx = 0 ;

    int86 (0x14, &inregs, &outregs) ;
}

term ()
{
}
