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

                                TITAN ASSEMBLER

                        INPUT STREAM READING & PARSING



read_line, parse_line, next_token, stcp

******************************************************************************/

#include "aglobal.h"



/******************************************************************************

                                   READ_LINE


synopsis : int read_line (fd, line)
           FILE *fd
           uchar *line
description : read_line reads a line from the source file, or stdin.
note : read_line returns -1 if EOF reached, 0 otherwise.

******************************************************************************/

int read_line (fd, line)
FILE *fd ;
uchar line[] ;
{
    int c, i = -1 ;

    do
    {
        c = getc (fd) ;
        if (i<MAXLEN)
            line[++i] = (uchar) c ;
    }
    while ((c!=EOF)&&(c!='\n')) ;
    line[i] = EOL ;
    if (c==EOF)
    {
        if (ferror (fd))  error (ERRRD, fsource) ;    /* stdin doesn't fail */
    }
    else c = 0 ;

    linenb++ ;
    return (c) ;
}


/******************************************************************************

                                  PARSE_LINE


synopsis : void parse_line (line, label, mnemo, modif)
           uchar *line, *label, *mnemo, *modif
description : parse_line breaks the line read from the source file in three
              components :
                - label : max LBLLEN or LBLLEN+1 characters
                - mnemonic : max 6 characters
                - modifier : not limited (in fact, MAXLEN characters)
note : parse_line doesn't return a result, the line is always valid.

******************************************************************************/

void parse_line(line, label, mnemo, modif)
uchar *line, *label, *mnemo, *modif ;
{
    int i=0, j ;

    *label = EOL ;
    *mnemo = EOL ;
    *modif = EOL ;

/* label parse */


/*               no label      comment       null line     tab *
 *               |             |             |             |   *
 *               v             v             v             v   */

    if ((*line!=' ')&&(*line!='*')&&(*line!=EOL)&&(*line!='\t'))
        i = stcp (label, line, 0, (*line=='=') ? LBLLEN+1 : LBLLEN) ;
    else if ((*line!='\t')&&(*line!='*')&&(*line!=EOL)&&
             (line[1]!=' ')&&(line[1]!='*')&&(line[1]!=EOL)&&(line[1]!='\t'))
        i = stcp (label, line, 1, (line[1]=='=') ? LBLLEN+1 : LBLLEN) ;
        

/* mnemonic parse */

    i = next_token (line, i) ;
    if (i<0) return ;
    if (line[i]=='*') return ;
    i = stcp (mnemo, line, i, 6) ;
    for (j=0; j<6; j++) mnemo[j] -= ((mnemo[j]<'a')||(mnemo[j]>'z')) ? 0 : 32 ;

/* modifier parse */

    i = next_token (line, i) ;
    if (i<0) return ;
    strcpy (modif, line+i) ;
}


/******************************************************************************

                                  NEXT_TOKEN


synopsis : int next_token (line, i)
           uchar *line
           int i
descrption : next_token looks the line, from the i-th position, for the next
             non-blank character or end of line. If end-of-line is reached,
             a negative value (-1) is returned to caller. Otherwise, the
             position of character is returned, which permits to reassign
             the line index (i=next_token(line, i)).

******************************************************************************/

int next_token (line, i)
uchar *line ;
int i ;
{
    while ((line[i]!=EOL)&&((line[i]==' ')||(line[i]=='\t'))) i++ ;
    return ((line[i]) ? i : -1);
}


/******************************************************************************

                                     STCP


synopsis : int stcp (str1, str2, i, lmax)
           uchar *str1, *str2
           int i, lmax
description : stcp is an alternate name for "string_copy". In fact, stcp copies
              str2 (from i-th position) into str1 (from 0-th position) until a
              blank character of end-of-line is reached. If lmax (max length of
              str1) is is reached, the rest of the field is ignored.
              The returned value is the new line index value.
note : we assume that stcp works well. No error value is returned.

replace with "strncpy" in C library for future releases

******************************************************************************/

int stcp (str1, str2, i, lmax)
uchar *str1, *str2;
int i, lmax;
{
    int j=0;
    do
    {
        if (j<lmax) str1[j++] = str2[i];
    }
    while ((str2[++i]!=' ')&&(str2[i]!='\t')&&(str2[i]!=EOL));
    str1[j] = EOL;
    return (i);
}
