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

                              MNEMONIC PROCESSING


ps_mnemo, find_mnemo, hex, dec

******************************************************************************/

#include "aglobal.h"

extern void regtest(), regarith(), reglogic(), branches(), rtnyes(), ptrtest(),
            stattest(), setptr(), setstat(), dparith(), datatrans(), nibhex(),
            lchex(), dxhex(), nibasc(), lcasc(),

            bss(), eject(), endx(), list(), title(), stitle(), lex(), id(),
            msg(), poll(), entryx(), charx(), key(), token(), bin(), chain(),
            endtxt(), endifx(), rdsymb(), elsex(), ifx() ;


/******************************************************************************

                                 PROCESS_MNEMO


synopsis : ps_mnemo (line, modif, ad)
           char *line, *modif
           struct mnemo_desc *ad 
description : pass control to the appropriate routine, to process the opcode.

******************************************************************************/

void ps_mnemo (char *line, char *modif, struct mnemo_desc *ad)
{
    switch (ad->m_class)
    {
        case 1 :
            regtest(ad, modif) ;
            break ;
        case 2 :
            regarith(ad, modif) ;
            break ;
        case 3 :
            reglogic(modif) ;
            break ;
        case 4 :
            branches(ad, modif) ;
            break ;
        case 7 :
            rtnyes() ;
            break ;
        case 8 :
            ptrtest(modif) ;
            break ;
        case 9 :
            stattest(modif) ;
            break ;
        case 10 :
            setptr(modif) ;
            break ;
        case 11 :
            setstat(modif) ;
            break ;
        case 12 :
            dparith(modif) ;
            break ;
        case 13 :
            datatrans(modif) ;
            break ;
        case 14 :
            nibhex(modif) ;
            break ;
        case 15 :
            lchex(modif) ;
            break ;
        case 16 :
            dxhex(modif) ;
            break ;
        case 17 :
            nibasc(modif) ;
            break ;
        case 18 :
            lcasc(modif) ;
            break ;
        case 19 :
            bss(modif) ;
            break ;
        case 20 :
            eject() ;
            break ;
        case 21 :
            endx() ;
            break ;
        case 23 :
            list(modif) ;
            break ;
        case 24 :
            title(modif) ;
            break ;
        case 25 :
            stitle(modif) ;
            break ;
        case 28 :
            lex(modif, line) ;
            break ;
        case 29 :
            id(modif, line) ;
            break ;
        case 30 :
            msg(modif, line) ;
            break ;
        case 31 :
            poll(modif, line) ;
            break ;
        case 32 :
            entryx(modif, line) ;
            break ;
        case 33 :
            charx(modif, line) ;
            break ;
        case 34 :
            key(modif, line) ;
            break ;
        case 35 :
            token(modif, line) ;
            break ;
        case 36 :
            bin(modif, line) ;
            break ;
        case 38 :
            chain(modif, line) ;
            break ;
        case 39 :
            endtxt(line) ;
            break ;
        case 40 :
            endifx() ;
            break ;
        case 41 :
            /* absx(modif) ; */
            break ;
        case 42 :
            rdsymb(modif) ;
            break ;
        case 43 :
            elsex() ;
            break ;
        case 44 :
            ifx(modif) ;
            break ;
    }
}


/******************************************************************************

                                   FIND_MNEMO


synopsis : struct mnemo_desc *find_mnemo (mnemo)
           char *mnemo
description : finds index of a mnemonic in mnemo_table, and returns it, -1
              if not found.

******************************************************************************/

struct mnemo_desc *find_mnemo (char * mnemo)
{
    int i, m, b = 1 ;
    long int h = 0 ;

    for (i=0; i<6; i++)
        h = (h<<3) + ((b) ? ((b=(int)mnemo[i])&7) : 0) ;
    h %= 43 ;
    i = h_opcode[h] ; m = h_opcode[++h] ;
    while (i<m)
    {
        if (strcmp(mnemo, mnemo_table[i].m_text))  i++ ;
        else break ;
    }
    return ((i==m) ? (struct mnemo_desc *)NULL : &mnemo_table[i]) ;
}


/******************************************************************************

                                   HEX


synopsis : char hex (digit)
           int digit
description : returns the hexadecimal representation of integer 'digit' (in
              ['0'..'9','A'..'Z']).

******************************************************************************/

char hex (int digit)
{
    return ((char) ( (digit<=9) ? digit+48 : digit+55) ) ;
}


/*****************************************************************************

                                   DEC


synopsis : int dec (digit)
           char digit
description : returns the decimal equivalent of hexadecimal character 'digit'.

*****************************************************************************/

int dec (char digit)
{
    return ((digit<'A') ? (int) digit - 48 : (int) digit - 55 ) ;
}
