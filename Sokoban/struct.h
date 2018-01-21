/*
 * Nom:			GOODMAN
 * PRENOM:		David
 * # ETUDIANT:	21401471
*/

#ifndef	STRUCT_DEF
#define	STRUCT_DEF
#include "constants.h"

struct lvl_info									//Information sur le niveau actuel
{
	char* filename;								//Nom du fichier d'ou provient le niveau
	int lvlNb;									//Numero du niveau
	int roundNb;								//Le nombre d'action qui a ete prise jusqu'a maintenent
};
typedef struct lvl_info LVL_INFO;


struct board
{
	POINT player;
	LVL_INFO info;
};
typedef struct board BOARD;

struct button
{
	char* name;
	POINT r, l;
	INPUT_DESC I;
};
typedef struct button BUTTON;

struct input
{
	INPUT_DESC desc;
	POINT p;
};
typedef struct input INPUT;

struct game
{
	BOARD lvl;
	BUTTON X[BUTTON_NB];
};
typedef struct game GAME;

#endif
