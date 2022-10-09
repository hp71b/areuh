#
# Makefile genere automatiquement par mkmf (le masque par defaut a ete adapte)
#

CFLAGS	      = -O -Wall -Wextra -Werror
LDFLAGS	      =
LIBS	      =
SYSLIBS	      =
BINDEST	      = /usr/local/bin

#
# S'il y a plus d'un programme, il faut aussi remplacer la cible
# $(PROGRAM) par autant de cibles que necessaire.
#

PROGRAM       = adp

SRCS	      = adp.c

HDRS	      = common.h \
		err.h

EXTHDRS	      = /usr/include/stdio.h

OBJS	      = adp.o

MANDEST	      = /usr/local/man/man1.Z

MANPAGES      = adp.1

#
# Les cibles :
#	all : compiler tout
#	clean : effacer tout
#	depend : recalculer toutes les dependances et les inserer ici
#	install : installe le programme dans l'aborescence
#	tags : cree les tags
#

all:		$(PROGRAM)

$(PROGRAM):     $(OBJS) $(LIBS)
		cc $(LDFLAGS) $(OBJS) $(LIBS) $(SYSLIBS) -o $(PROGRAM)

clean:;		rm -f $(OBJS) $(PROGRAM) core tags

depend:;	mkmf ROOT=$(ROOT)

install:	$(PROGRAM)
		-strip $(PROGRAM)
		if [ $(BINDEST) != . ] ; \
		then \
		    (cd $(BINDEST) ; rm -f $(PROGRAM)) ; \
		    cp $(PROGRAM) $(BINDEST) ; \
		    if [ "$(MANPAGES)" != none ] ; \
		    then \
			(cd $(MANDEST) ; rm -f $(MANPAGES)) ; \
			for i in $(MANPAGES) ; \
			do \
			    compress < $$i > $(MANDEST)/$$i ; \
			done ; \
		    fi ; \
		fi

tags:           $(HDRS) $(SRCS)
		ctags $(HDRS) $(SRCS)

#
# Dependances calculees automatiquement par mkmf.
# Ne rien changer apres cette ligne !
#
###
adp.o: common.h err.h
