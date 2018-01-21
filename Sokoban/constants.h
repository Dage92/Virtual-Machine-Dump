/*
 * Nom:			GOODMAN
 * PRENOM:		David
 * # ETUDIANT:	21401471
*/

#ifndef	CONST_DEF
#define	CONST_DEF

	#define BOX_SIZE		34											//Taille d'une case (px)
	#define	NUMB_BOX_HEIGHT	12											//Nombre de cases en hauteur
	#define NUMB_BOX_WIDTH	12											//Nombre de cases en largeur
	#define WINDOW_HEIGHT	BOX_SIZE * NUMB_BOX_HEIGHT					//Hauteur du plateau de jeu
	#define WINDOW_WIDTH	BOX_SIZE * NUMB_BOX_WIDTH					//Largeur du plateau de jeu
	#define BUTTON_NB		6											//Nombre de bouton
	#define BUTTON_HEIGHT	BOX_SIZE									//Hauteur d'un bouton
	#define BUTTON_WIDTH	WINDOW_WIDTH/6								//Largeur d'un bouton
	
/* Enumerations */
	enum input_desc														//Identifie la description des actions (movements, options, etc) que le joueur peut realiser
	{
		UP, DOWN, LEFT, RIGHT, UNDO, REDO, INIT, PREV, NEXT, QUIT
	};
	typedef enum input_desc INPUT_DESC;
	
	enum {EMPTY, WALL, PLAYER, BOX, OBJ, PLAYER_OBJ, BOX_OBJ}			//Identifie le type d'une case
	
#endif
