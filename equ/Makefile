#
# Ce Makefile fait un minimum de choses
#

LIBDEST	      = /usr/local/lib

EP	      = hp71.ep hpil.ep

#
# Les cibles :
#	all : rien (ne fait que signaler l'absence eventuelle de hp71.ep)
#	install : installe le programme dans l'aborescence
#	clean : rien (compatibilité avec le Makefile général)
#	tags : rien (compatibilité avec le Makefile général)
#

all:		$(EP)

depend:;	mkmf ROOT=$(ROOT)

install:	$(EP)
		if [ "$(LIBDEST)" != . ] ; \
		then \
		    (cd $(LIBDEST) ; rm -f $(EP)) ; \
		    cp $(EP) $(LIBDEST) ;\
		fi ;

clean: ;

tags: ;
