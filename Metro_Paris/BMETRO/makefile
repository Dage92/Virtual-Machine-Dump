#
# Makefile - Balade en Metro
#
# Nom de l'executable
EXEC = bmetro

# Parametres de compilation
CC = gcc
OPTIONS = -O3 -W -Wall -ansi -pedantic
SRC= bm_main.c bm_lectureFichier.c bm_chemin.c bm_requete.c bm_utiles.c bm_itineraire.c bm_reseau.c bm_affichage.c bm_itineraireInt.c bm_itineraireOpt.c
OBJ= $(SRC:.c=.o)

$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(OPTIONS)

%.o: %.c
	$(CC) -c $< $(OPTIONS)

clean:
	@rm -f \#*# *.o *~ $(EXEC) && echo "Nettoyage effectue avec succes"


bm_affichage.o: bm_affichage.c bm_reseau.h bm_utiles.h bm_requete.h \
  bm_chemin.h bm_itineraire.h bm_affichage.h
bm_chemin.o: bm_chemin.c bm_reseau.h bm_chemin.h
bm_itineraire.o: bm_itineraire.c bm_reseau.h bm_utiles.h bm_requete.h \
  bm_chemin.h bm_itineraire.h bm_itineraireInt.h bm_itineraireOpt.h
bm_itineraireInt.o: bm_itineraireInt.c bm_reseau.h bm_chemin.h \
  bm_requete.h bm_itineraire.h
bm_itineraireOpt.o: bm_itineraireOpt.c bm_reseau.h bm_requete.h \
  bm_chemin.h bm_itineraire.h bm_itineraireOpt.h bm_utiles.h \
  bm_affichage.h
bm_lectureFichier.o: bm_lectureFichier.c bm_reseau.h bm_utiles.h \
  bm_lectureFichier.h
bm_main.o: bm_main.c bm_reseau.h bm_utiles.h bm_lectureFichier.h \
  bm_requete.h bm_chemin.h bm_itineraire.h bm_affichage.h
bm_requete.o: bm_requete.c bm_reseau.h bm_utiles.h bm_requete.h
bm_reseau.o: bm_reseau.c bm_reseau.h bm_utiles.h
bm_utiles.o: bm_utiles.c bm_carac.h bm_reseau.h bm_utiles.h
