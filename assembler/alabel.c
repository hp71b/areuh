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

                               LABEL  PROCESSING


ps_label, find_label, add_label, label_value, s_init

******************************************************************************/

#include "aglobal.h"


extern saddr calc_expression() ;
extern char *memoire() ;

struct symbol *find_label (char *label) ;
struct symbol *add_label (char *label, saddr val, char *exp, char type, char os) ;

/******************************************************************************

                                   PS_LABEL


synopsis : void ps_label (label, mnemo, modif)
           char *label, *mnemo, *modif
description : "pass" function call "ps_label" when a label is encoutered.
              Thus, in function of pass number, we must process the label.

              If pass one, the label is declared and its value is assigned (if
              possible, else -1), only if it is not yet declared.

              If pass two, the label is declared.
              We calculate the value of the label. If it had a value, we
              compare both. If not, the value is assigned.

******************************************************************************/

void ps_label (char *label, char *mnemo, char *modif)
{
    struct symbol *ad;
    saddr val ;

    /* la ligne ci-dessous teste si la partie "utile" du label est "FiLeNd" */
    if (strcmp(label + (*label=='='), "FiLeNd")==0) error (ERRFLN, "") ;

    ad = find_label (label) ;
    /* during pass two, label must be found. Otherwise, 
       the assembler is bug-full !!!                         */
    if (strcmp (mnemo, "EQU"))
    {                                  /* implicit declaration */
        switch (passnb)
        {
            case 1 :
                if (ad==NULL) add_label (label, pc, "", LREL, 0) ;
                else if ((ad->s_value==LBL_UDF)||(ad->s_value==LBL_EXT))
                {
                     ad->s_value = pc ;
                     ad->s_type = LREL ;
                }
                break ;
            case 2 :
                if ((ad->s_value!=pc)||(ad->s_type!=LREL))
                    error (WRNDUP, ""); /* duplicate label */
                break ;
        }
    }
    else                               /* explicit declaration (EQU) */
    {
        val = calc_expression (modif) ;
        switch (passnb)
        {
            case 1:
                if (ad==NULL)
                {
                    if (val >= 0L)
                    {
                        add_label (label, val, "", relabs, 0) ;
                    }
                    else if (val == EXP_ERR)
                    {
                        add_label (label, LBL_IVL, "", LUDF, 0) ;
                    }
                    else                             /* (val == EXP_EXT) */
                    {
                        if (*label=='=')
                            add_label (label, LBL_XEQ, extexp, LUDF, 0) ;
                        else add_label (label, LBL_SEQ, extexp, LUDF, 0) ;
                    }
                }
                else if ((ad->s_value==LBL_UDF)||(ad->s_value==LBL_EXT))
                {
                    if (val >= 0L)
                    {
                        ad->s_value = val ;
                        ad->s_type = relabs ;
                    }
                    else if (val == EXP_ERR)
                    {
                        ad->s_value = LBL_IVL ;
                        ad->s_type = LUDF ;
                    } 
                    else            /* (val == EXP_EXT) */
                    {
                        ad->s_value = (*label=='=') ? LBL_XEQ : LBL_SEQ ;
                        ad->s_def = memoire (strlen (extexp) + 1) ;
                        strcpy (ad->s_def, extexp) ;
                        ad->s_type = LUDF ;
                    }
                }
                break;                 /* du 'case pass one' */
            case 2:
                if (ad->s_value >= 0L) /* first value of label in first pass */
                {
                    if (val!=ad->s_value) error (WRNDUP, "") ;
                }
                else if (ad->s_value == LBL_IVL)
                {
                    if (val >= 0L)
                    {
                        ad->s_value = val ;
                        ad->s_type = relabs ;
                    }
                    else if (val == EXP_ERR)
                    {
                        ad->s_value = 0L ;
                        ad->s_type = LABS ;
                    }
                    else        /* EXP_EXT */
                    {
                        ad->s_def = memoire (strlen(extexp)+1);
                        if (ad->s_def == (char *) NULL)
                            error (ERRMEM, "") ;
                        strcpy (ad->s_def, extexp) ;
                        ad->s_value = (*label=='=')? LBL_XEQ :LBL_SEQ ;
                        ad->s_type = LUDF ;
                    }
                }
                else if ((ad->s_value == LBL_XEQ) || (ad->s_value == LBL_SEQ))
                {
                    if (val == EXP_EXT)
                    {
                        if (strcmp(ad->s_def, extexp))
                            error (WRNDUP, "") ; /* duplicate label */
                    }
                    else
                    {
                        ad->s_value = val ;
                        ad->s_type = relabs ;
                    }
                }
                break;                 /* end of 'case pass 2' */
        }
    }
}


/******************************************************************************

                                  FIND_LABEL


synopsis : struct symbol *find_label (label)
           char *label
description : find_label searches the symbol list for a given label.
              If the search is succesful, find_label returns a pointer to the
              structure. If not, NULL is returned.

******************************************************************************/

struct symbol *find_label (char *label)
{
    struct symbol *p ;
    int test ;

    if ((!modular)&&(*label=='=')) label++ ;
    p = h_label[(int) ((*label=='=') ? label[1] : label[0])]->s_next ;
    while (p)
    {
        test = strcmp (label, p->s_name) ;
        if (test==0)    break ;
        p = p->s_next ;
        if (test<0) p = NULL ;         /* label < p->s_name : not found */
    }
    return (p) ;
}


/******************************************************************************

                                  ADD_LABEL


synopsis : struct symbol *add_label (label, val, exp, type, os)
           char *label, *exp
           saddr val
           char type, os
description : add_label inserts the label (name and value) in the symbol list.
note : the symbol list is always sorted by label name.

******************************************************************************/

struct symbol *add_label (char *label, saddr val, char *exp, char type, char os)
{
    struct symbol *p, *s ;
    int b=1 ;

    if ((!modular)&&(*label=='=')) label++ ;
    p = h_label[(int) ((*label=='=') ? label[1] : label[0]) ] ;
    s = p ;
    while ((b) && (s))
    {
        if (strcmp(label, s->s_name)<0)   b = 0 ;
        else
        {
            p = s ; s = s->s_next ;
        }
    }
    s = (struct symbol *) memoire (sizeof(struct symbol)) ;
    if (s)
    {
        s->s_next = p->s_next ;
        p->s_next = s ;
        strcpy (s->s_name, label) ;
        s->s_decl = (sint) linenb ;
        s->s_xref = (struct xtable *) NULL ;
        s->s_value = val ;
        s->s_type = type ;
        s->s_os = os ;
        if ((val==LBL_XEQ)||(val==LBL_SEQ))
        {
            s->s_def = memoire (strlen (exp) + 1) ;
            strcpy (s->s_def, exp) ;
        }
        else s->s_def = (char *) NULL ;
    }
    else  error (ERRMEM, "") ;

    return (s) ;
}


/******************************************************************************

                                 SYMBOL_VALUE


synopsis : saddr symbol_value (label)
           char *label
description : label_value returns the value of the label, knowing its name.
              If it doesn't exist, and this occurs during the second pass, an
              error message is generated.
note : if symbol not found in the symbol list, -1 is returned. It's to the
       caller (label_value) to report the error "unrecognized label".

******************************************************************************/

saddr symbol_value (char *label)
{
    struct symbol *ad;
    struct xtable *x ;
    saddr val ;

    ad = find_label (label) ;
    if (ad) val = ad->s_value ;
    else
    {
        val = ((*label=='=')&&(modular)) ? LBL_EXT : LBL_UDF ;
        ad = add_label (label, val, "", LUDF, 0) ;
    }
    if (passnb==2)
    {                      /* label must be found in pass two */
        if (xref)          /* need to store it in cross-ref. table */
        {
            x = (struct xtable *) memoire (sizeof(struct xtable)) ;
            if (x)
            {
                x->x_next = ad->s_xref ;
                if (ad->s_xref) ad->s_xref->x_prev = x ;
                ad->s_xref = x ;
                x->x_prev = (struct xtable *) NULL ;
                x->x_line = (sint) linenb ;
            }
            else error (ERRMEM, "") ;
        }
        if (val==LBL_UDF)
            error (WRNULB, "") ;   /* undefined label */
    }

    if (val==LBL_SEQ) xlabel = ad->s_def ;
    ad->s_os = 0 ;                 /* (O.S.) label is used */
    relabs = ad->s_type ;
    return (val) ;
}


/******************************************************************************

                                    S_INIT


synopsis : void s_init ()
description : initializes the symbol list. It is structured as follow :
              - a table (0..255) which points to a list of labels beginning
                with the character (index in this table is the first char.)
              - a chained list of labels,
              - a doubly linked list of references on this label. This is not
                initialized here.

******************************************************************************/

void s_init ()
{
    struct symbol *s ;
    int i ;

    s = (struct symbol *) memoire (256*sizeof(struct symbol)) ;
    if (!(s)) error (ERRMEM, "") ;

    for (i=0; i<256; i++)
    {
        strcpy (s->s_name, "") ;
        s->s_value = (saddr) 0 ;
        s->s_type = LUDF ;
        s->s_decl = (sint) 0 ;
        s->s_xref = (struct xtable *) NULL ;
        s->s_next = (struct symbol *) NULL ;
        h_label[i] = s++ ;
    }
}
