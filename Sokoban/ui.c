/*
 * Nom:			GOODMAN
 * PRENOM:		David
 * # ETUDIANT:	21401471
*/

#include <uvsqgraphics.h>
#include <ui.h>
#include <stdio.h>
#include <string.h>

void init_ui()												//Initialisation de l'affichage
{
	init_graphics(WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_WM_SetCaption("GOODMAN David - Sokoban", NULL);
	affiche_auto_off;
}

void displayLvlInfo(LEVEL L, int h_but)						//Affichage de l'information du niveau actuel
{
}

void display_board(BOARD B)									//Affichage du plateau de jeu
{
	int i, j;
	for (i=0; i<WINDOW_WIDTH; i++)
	{
		for (j=0; j<WINDOW_HEIGHT; j++)
		{
			strcpy(str, "./sprites/");
			if (B.[i][j].desc == WALL)
			{
				strcat(str, "wall.jpg");
			}
			else if (B.[i][j].desc == PLAYER)
			{
				strcat(str, "sprite_down.png");
			}
			else if (B.[i][j].desc == BOX)
			{
				strcat(str, "chest.jpg");
			}
			else if (B.[i][j].desc == OBJ)
			{
				strcat(str, "objective.png");
			}
			else if (B.[i][j].desc == PLAYER_OBJ)
			{
				strcat(str, "sprite_obj.png");
			}
			else (B.[i][j].desc == BOX_OBJ)
			{
				strcat(str, "chest_ok.jpg");
			}
			affiche_image(str, B.[i][j], BOX_SIZE, BOX_SIZE);
		}
	}
}

void display_Sokoban()										//Affichage du jeu
{
