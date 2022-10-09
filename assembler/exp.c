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

                             EXPRESSION EVALUATION


calc_expression, reduce_E, reduce_T, reduce_F, reduce_B, reduce_X,
reduce_P, dec_value, hex_value, bin_value, ascii_value, label_value, apply,
trunc, next_char, append_extexp

******************************************************************************/

#include "flag.h"

#if ASSEMBLER
#include "aglobal.h"
#else
#include "lglobal.h"
#endif

char extexp [4*MAXLEN] ;
char *pexp, *pextexp ;
char *xlabel ;
int relabs ;

static saddr reduce_E(void) ;
static saddr reduce_T(void) ;
static saddr reduce_F(void) ;
static saddr reduce_B(void) ;
static saddr reduce_X(void) ;
static saddr reduce_P(void) ;
static saddr dec_value(void) ;
static saddr hex_value(void) ;
static saddr bin_value(void) ;
static saddr ascii_value(char) ;
static saddr label_value(void) ;
static saddr truncate24(saddr) ;
static saddr apply (saddr val1, char op, saddr val2, char relabs1, char relabs2) ;
static void next_char(void);
static void append_extexp(char *) ;


/******************************************************************************

                               CALC_EXPRESSION


synopsis : saddr calc_expression (exp)
           char *exp
description : That's the expression evaluator. Productions used are :

    E -> T { {+|-} T }*
    T -> F { {*|/} F }*
    F -> B { {&|!} B }*
    B -> X | -X | `X           (two's and one's complement)
    X -> N { {~|^} N }*
    P -> D | #<hex> | %<bin> | '<ascii>' | \<ascii>\ | <label> | * | (E)

    D -> <dec>                 if expression evaluated by ASSSEMBLER
    D -> <dec> | <dec> r       if expression evaluated by LINKER

    where   E : expression
            T : term
            F : factor
            B : boolean
            X : exponentiation
            P : primary
            D : decimal number
warning : with this grammar, 5--5 is valid (5 minus -5), but 5---5 is not.
          This can be modified by : B -> -B | P . The code is more complex,
          and I'm not sure that it's a real improvement.
note : Algorithm used is recursive descent (Mr Vermeulen would be horrified !)
       like Forth/Assembler rom based assembler, but is quietly different...

******************************************************************************/

saddr
calc_expression (char *exp)
{
    saddr val;

    pextexp = extexp ;
    pexp = exp ;
    val = reduce_E() ;
    if (((val>=0L)||(val==EXP_EXT))&&(*pexp!=EOL)&&(*pexp!=' ')&&(*pexp!='\t'))
    {
        error(WRNEXP, "") ;    /* illegal expression */
        val = EXP_ERR ;
    }
    *pextexp = EOL ;
    return (val) ;
}


/******************************************************************************

                                   REDUCE_E


synopsis : saddr reduce_E()
description : This function reduces a given expression starting at pexp.

******************************************************************************/
static saddr
reduce_E(void)
{
    saddr val1, val2;
    char op, lrelabs;

    val1 = reduce_T () ;

    while ((((op = *pexp)=='+')||(op=='-'))&&(val1!=EXP_ERR))
    {
        lrelabs = relabs ;
        next_char () ;
        val2 = reduce_T () ;
        val1 = apply (val1, op, val2, lrelabs, relabs) ;
    }
    return (val1) ;
}


/******************************************************************************

                                   REDUCE_T


synopsis : saddr reduce_T ()
description : same as above, for T-production

******************************************************************************/

static saddr
reduce_T (void)
{
    saddr val1, val2 ;
    char op, lrelabs ;

    val1 = reduce_F () ;
    while ((((op = *pexp)=='*')||(op=='/'))&&(val1!=EXP_ERR))
    {
        lrelabs = relabs ;
        next_char () ;
        val2 = reduce_F () ;
        val1 = apply (val1, op, val2, lrelabs, relabs) ;
    }
    return (val1) ;
}


/******************************************************************************

                                   REDUCE_F


synopsis : saddr reduce_F ()
description : same as reduce_E

******************************************************************************/

static saddr
reduce_F (void)
{
    saddr val1, val2;
    char op, lrelabs ;

    val1 = reduce_B () ;
    while ((((op = *pexp)=='&')||(op=='!'))&&(val1!=EXP_ERR))
    {
        lrelabs = relabs ;
        next_char () ;
        val2 = reduce_B () ;
        val1 = apply (val1, op, val2, lrelabs, relabs) ;
    }
    return (val1) ;
}


/******************************************************************************

                                   REDUCE_B


synopsis : saddr reduce_B ()
description : reduces a boolean factor. This must be done by reduction of minus
              sign eventually.

******************************************************************************/

static saddr
reduce_B (void)
{
    saddr val;
    char op ;

    op = *pexp ;

    if ((op=='-')||(op=='`')) next_char () ;
    val = reduce_X () ;
    if (val<0L)
        return(val) ;
    switch (op)
    {
        case '-' :
            return (truncate24 (-val)) ;
        case '`' :
            return (truncate24 (~val)) ;
        default :
            return (val) ;
    }
}


/******************************************************************************

                                   REDUCE_X


synopsis : saddr reduce_X ()
description : same as reduce_E

******************************************************************************/

static saddr
reduce_X (void)
{
    saddr val1, val2;
    char op, lrelabs;

    val1 = reduce_P () ;
    while ((((op = *pexp)=='~')||(op=='^'))&&(val1!=EXP_ERR))
    {
        lrelabs = relabs ;
        next_char () ;
        val2 = reduce_P () ;
        val1 = apply (val1, op, val2, lrelabs, relabs) ;
    }
    return (val1) ;
}


/******************************************************************************

                                   REDUCE_P


synopsis : saddr reduce_P ()
description : these are the terminal rules.
note : rule P -> D is implemented "in line" in this code (not as a separate
  function).

******************************************************************************/

static saddr
reduce_P (void)
{
    saddr val ;
    char limit, line[MAXLEN] ;

    switch (*pexp)
    {
        case '#' :
            next_char () ;
            if (((*pexp>='0')&&(*pexp<='9')) ||
               ((*pexp>='A')&&(*pexp<='F')) ||
               ((*pexp>='a')&&(*pexp<='f')))
                val = hex_value () ;
            else
            {
                error (WRNIHX,"");          /* illegal hexadecimal constant */
                val = EXP_ERR ;
            }
            relabs = LABS ;
            break ;
        case '%' :
            next_char () ;
            if ((*pexp=='0')||(*pexp=='1'))
                val = bin_value () ;
            else
            {
                error (WRNIBC, "") ;        /* illegal binary constant */
                val = EXP_ERR ;
            }
            relabs = LABS ;
            break ;
        case '\'' :
        case '\\' :
            limit = *pexp ;
            next_char () ;
            val = ascii_value (limit) ;
            if (*pexp!=limit)
            {
                error (WRNASC,"");                 /* illegal ascii constant */
                val = EXP_ERR ;
            }
            next_char () ;
            relabs = LABS ;
            break ;
        case '*' :
            val = pc ;
            pexp++ ;
            sprintf (line, "%ldr", pc) ;
            relabs = LREL ;
            append_extexp (line) ;
            break ;
        case '(' :
            next_char () ;
            val = reduce_E () ;
            if ((*pexp!=')')&&(val>=0))
            {
                error (WRNPAR, "") ;         /* mismatched parenthesis */
                val = EXP_ERR ;
            }
            next_char () ;
            break ;
        case EOL :
            error (WRNEXP,"") ;              /* illegal expression     */
            val = EXP_ERR ;
            break ;

        default :
            if ((*pexp>='0')&&(*pexp<='9'))
            {
                val = dec_value () ;
                relabs = LABS ;
#if LINKER
                if (*pexp=='r')
                {
                    next_char() ;
                    relabs = LREL ;
                    val += tmodule[file].m_ad ;
                }
#endif
            }
            else val = label_value () ;
            break ;
    }
    return (val) ;
}


/******************************************************************************

                                   DEC_VALUE


synopsis : saddr dec_value ()
descrption : This function returns the decimal value of a constant. The search
             is stopped when a non numeric digit is reached.
             (this can be ),+,-,*,/,&,!).
             Finally, the founded value is returned.
note : this function doesn't check overflow. If there is, numbers are treated
       as 20 bits words, and overflow doesn't propagate on 32 bits of an
       integer (-1 is never reached when calculus).

******************************************************************************/

static saddr
dec_value (void)
{
    saddr val=0L ;

    do
    {
        val = truncate24 (val * 10L + (saddr) (*pexp-'0') ) ;
        next_char () ;
    }
    while ((*pexp>='0')&&(*pexp<='9')) ;
    return (val);
}


/******************************************************************************

                                   HEX_VALUE


synopsis : saddr hex_value ()
description : same as above for hexadecimal constants

******************************************************************************/

static saddr
hex_value (void)
{
    saddr i, val = 0L ;

    while ( ((*pexp>='0')&&(*pexp<='9')) ||
            ((*pexp>='A')&&(*pexp<='F')) ||
            ((*pexp>='a')&&(*pexp<='f')) )
    {
        if (*pexp<='9') i = (long int) ((*pexp) - '0') ;
        else if (*pexp<='F') i = (long int) ((*pexp) - 'A' + 10) ;
        else i = (long int) ((*pexp) - 'a' + 10) ;
        val = truncate24 (val*16L +  i) ;
        next_char () ;
    }
    return (val) ;
}


/******************************************************************************

                                   BIN_VALUE


synopsis : saddr bin_value ()
description : same as above for binary constants

******************************************************************************/

static saddr
bin_value (void)
{
    saddr val = 0L ;

    while ((*pexp=='0')||(*pexp=='1'))
    {
        val = truncate24 (val*2L + ((saddr) ((*pexp) - '0'))) ;
        next_char () ;
    }
    return (val) ;
}


/******************************************************************************

                                  ASCII_VALUE


synopsis : saddr ascii_value ()
description : same as above, but the search is stopped when encoutered a '.
              The pointer *pexp stands on this character.

******************************************************************************/

static saddr
ascii_value (char limit)
{
    saddr val = 0 ;

    while ((*pexp!=EOL)&&(*pexp!=limit))
    {
        val = truncate24 (val*256L + ((saddr) *pexp)) ;
        next_char () ;
    }
    return (val) ;
}


/******************************************************************************

                                  LABEL_VALUE


synopsis : saddr label_value (void)
description : parses the symbol, then tries to return the value founded in the
              symbol list.

******************************************************************************/

static saddr
label_value (void)
{
    char label[LBLLEN+2+10], *plabel ;	// +10 to cope with %ld max int
    int mx, need_par = 0, j = 0 ;
    saddr val ;

    mx = LBLLEN + ((*pexp=='=') ? 1 : 0) ;
    while ((*pexp!=EOL)&&(*pexp!=' ')&&(*pexp!='\t')&&
           (*pexp!=')')&&(*pexp!='\\'))
    {
        if (j<mx) label[j++] = *pexp ;
        pexp++ ;
    }
    label[j] = EOL ;
    plabel = label ;

    if ((val = symbol_value (label)) >= (saddr) 0)
    {                             /* found, copy value */
     
        if (relabs==LREL) sprintf (label, "%ldr", val) ;
        else sprintf (label, "%ld", val) ;
    }
    else if ((val == LBL_UDF) || (val == LBL_IVL))
    {   /* UDF : label not (yet) declared, IVl : invalid label */
        *plabel = EOL ;                /* incoherent value */
        val = EXP_ERR ;
    }
    else if ((val == LBL_EXT) || (val == LBL_XEQ))
    {   /* LBL_EXT: ext. label not known, LBL_XEQ: global defined with ext. */
        val = EXP_EXT ;                        /* keep label name */
    }
    else                       /* (val == LBL_SEQ) */
    {   /* LBL_SEQ : synonym, expandable */
        plabel = xlabel ;              /* get definition of label */
        need_par = 1 ;                 /* enclose label with (...) */
        val = EXP_EXT ;                /* and store it into extep */
    }

    if (need_par) append_extexp ("(") ;
    append_extexp (plabel) ;
    if (need_par) append_extexp (")") ;

    return (val) ;
}


/******************************************************************************

                                     APPLY


synopsis : saddr apply (val1, op, val2, relabs1, relabs2)
           saddr val1, val2
           char op, relabs1, relabs2
description : calculate the value of binary operator op applied to operands
              val1 & val2.
note : under overflow condition, numbers are truncated to 20 bits.

******************************************************************************/

static saddr
apply (saddr val1, char op, saddr val2, char relabs1, char relabs2)
{
    saddr val = 0 ;

    if (val2==EXP_ERR)                    return (EXP_ERR) ;
    if ((val1==EXP_EXT)||(val2==EXP_EXT)) return (EXP_EXT) ;

    switch (op)
    {
        case '+' :
            val = truncate24 (val1 + val2) ;
            break ;
        case '-' :
            val = truncate24 (val1 - val2 ) ;
            break ;
        case '*' :
            val = truncate24 (val1 * val2 ) ;
            break ;
        case '/' :
#if ASSEMBLER
            val = (val2 ? val1 / val2 : EXP_ERR ) ;
#else
            val = (val2 ? val1 / val2 : EXP_EXT ) ;
#endif
            if (val2==0L)   error (WRNNUL, "") ;   /* null divisor */
            break ;
        case '&' :
            val = val1 & val2 ;
            break ;
        case '!' :
            val = val1 | val2 ;
            break ;
        case '~' :
            val = truncate24 (val1*256 + val2) ;
            break ;
        case '^' :
            if ((val1<0)||(val2<0)||((val1==0)&&(val2==0)))
            {
                error (WRNIXP, "") ;           /* Illegal exponentiation */
#if ASSEMBLER
                val = EXP_ERR ;
#else
                val = EXP_EXT ;
#endif
            }
            else
            {
                val = 1 ;
                for (;val2>0 ; val2--) val *= val1 ;
                val = truncate24 (val) ;
            }
            break ;
    }
    if ((relabs1==LUDF)||(relabs2==LUDF))      relabs = LUDF ;
    else if ((relabs1==LREL)||(relabs2==LREL)) relabs = LREL ;
    else                                       relabs = LABS ;
    return (val) ;
}


/******************************************************************************

                                     TRUNC


synopsis : saddr truncate24 (val)
           saddr val
description : truncates 32 bits integer to 24 bits.

******************************************************************************/

static saddr
truncate24 (saddr val)
{
    return (val & 0xffffff) ;
}


/******************************************************************************

                                   NEXT_CHAR


synopsis : void next_char ()
description : stores the current character in extexp variable, and moves the
              expression pointer (pexp) forward one position.

******************************************************************************/
static void
next_char (void)
{
    *pextexp = *pexp ;
    pextexp++ ;
    pexp++ ;
}


/******************************************************************************

                                 APPEND_EXTEXP


synopsis : void append_extexp (line)
           char *line ;
description : append line to extexp string.

******************************************************************************/

static void
append_extexp (char *line)
{
    while (*line)
    {
        *pextexp = *line ;
        pextexp++ ;
        line++ ;
    }
}
