/*
 * Nom:			GOODMAN
 * PRENOM:		David
 * # ETUDIANT:	21401471
*/

CFLAGS = -c  -g  -Wall

all:	sokoban
	./sokoban	sasquatch1.xsb
	
test: clean compil
	./sokoban  -n  8  sasquatch1.xsb

compil: sokoban
	

sokoban: sokoban.o editor.o history.o read_write.o ruleset.o ui.o
	gcc -o sokoban sokoban.o editor.o history.o read_write.o ruleset.o ui.o -luvsqgraphics `sdl-config --libs` -lm -lSDL_ttf

sokoban.o: sokoban.c constants.h struct.h
	gcc $(CFLAGS) `sdl-config --cflags` sokoban.c

read_write.o: read_write.c read_write.h constants.h struct.h
	gcc $(CFLAGS) `sdl-config --cflags` read_write.c

ui.o: ui.c ui.h constants.h struct.h
	gcc $(CFLAGS) `sdl-config --cflags` ui.c

history.o: history.c history.h constants.h struct.h
	gcc $(CFLAGS) `sdl-config --cflags` history.c

ruleset.o: ruleset.c ruleset.h constants.h struct.h
	gcc $(CFLAGS) `sdl-config --cflags` ruleset.c

editor.o: editor.c editor.h constants.h struct.h
	gcc $(CFLAGS) `sdl-config --cflags` editor.c

clean:
	rm -f sokoban
	rm -f *.o