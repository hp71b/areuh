#
# Makefile general pour areuh
#
# A pour seule fonction d'appeler et d'enchainer les differents Makefiles
#

SUBDIR	      = assembler linker msg dump equ
BINDEST       = /usr/local/bin
LIBDEST       = /usr/local/lib
MANDEST       = /usr/local/man/man1.Z
SHELL	      = /bin/sh

#
# Les cibles :
#	all : compiler tout
#	clean : effacer tout
#	depend : recalculer toutes les dependances et les inserer ici
#	install : installe le programme dans l'aborescence
#	tags : cree les tags
#

all:
	@for i in $(SUBDIR);\
	do (\
	    echo Making $$i ...;\
	    cd $$i;\
	    $(MAKE) ROOT=$(ROOT)\
	); done

clean:
	@for i in $(SUBDIR);\
	do (\
	    echo Cleaning $$i ...;\
	    cd $$i;\
	    $(MAKE) clean\
	); done

depend:
	@for i in $(SUBDIR);\
	do (\
	    echo Creating dependencies for $$i ...;\
	    cd $$i;\
	    $(MAKE) ROOT=$(ROOT) depend\
	); done

install:
	@for i in $(SUBDIR);\
	do (\
	    echo Installing $$i ...;\
	    cd $$i;\
	    $(MAKE) BINDEST=$(BINDEST) LIBDEST=$(LIBDEST) MANDEST=$(MANDEST) install\
	); done
