#
# Makefile genere automatiquement par mkmf (le masque par defaut a ete adapte)
#

CFLAGS	      = -O
LDFLAGS	      =
LIBS	      =
SYSLIBS	      =
BINDEST	      = /usr/local/bin

#
# S'il y a plus d'un programme, il faut aussi remplacer la cible
# $(PROGRAM) par autant de cibles que necessaire.
#

PROGRAM       = aas

SRCS	      = aerror.c \
		ainput.c \
		alabel.c \
		alist.c \
		amain.c \
		amnemo.c \
		aobj.c \
		aopc1.c \
		aopc2.c \
		apass.c \
		areport.c \
		autil.c \
		exp.c \
		mdep.c \
		tabgrp.c \
		tabopc.c

HDRS	      = agen.h \
		aglobal.h \
		common.h \
		err.h \
		flag.h

EXTHDRS	      = /usr/include/stdio.h \
		/usr/include/sys/stat.h \
		/usr/include/sys/types.h

OBJS	      = aerror.o \
		ainput.o \
		alabel.o \
		alist.o \
		amain.o \
		amnemo.o \
		aobj.o \
		aopc1.o \
		aopc2.o \
		apass.o \
		areport.o \
		autil.o \
		exp.o \
		mdep.o \
		tabgrp.o \
		tabopc.o

MANDEST	      = /usr/local/man/man1.Z

MANPAGES      = aas.1

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
aerror.o: aglobal.h common.h /usr/include/stdio.h err.h
ainput.o: aglobal.h common.h /usr/include/stdio.h err.h
alabel.o: aglobal.h common.h /usr/include/stdio.h err.h
alist.o: aglobal.h common.h /usr/include/stdio.h err.h
amain.o: aglobal.h common.h /usr/include/stdio.h err.h
amnemo.o: aglobal.h common.h /usr/include/stdio.h err.h
aobj.o: aglobal.h common.h /usr/include/stdio.h err.h
aopc1.o: aglobal.h common.h /usr/include/stdio.h err.h agen.h
aopc2.o: aglobal.h common.h /usr/include/stdio.h err.h agen.h
apass.o: aglobal.h common.h /usr/include/stdio.h err.h agen.h
areport.o: aglobal.h common.h /usr/include/stdio.h err.h
autil.o: aglobal.h common.h /usr/include/stdio.h err.h
exp.o: flag.h aglobal.h common.h /usr/include/stdio.h err.h
mdep.o: flag.h aglobal.h common.h /usr/include/stdio.h err.h \
	/usr/include/sys/types.h /usr/include/sys/stat.h
tabgrp.o: aglobal.h common.h /usr/include/stdio.h err.h
tabopc.o: aglobal.h common.h /usr/include/stdio.h err.h
