/****************************************************/
/*   Projet      : Balade dans le metro             */
/*   Date        : mai 2009                         */
/*   Auteur      : Climoo                           */
/*   Fichier     : bm_reseau.h                      */
/*   Description : En-tete contenant les            */
/*   principales definitions de structures du projet*/
/*                                                  */
/* **************************************************/

#ifndef _BM_RESEAU_H

#define _BM_RESEAU_H

/*Constante pour la taille maximale d'une chaine de caracteres,
  utilises pour les noms de ligne ou de stations.*/
#define BM_MAX_STR 64

/*Constante retournee par une fonction ayant echoue a cause
  d'une erreur d'allocation memoire (echec d'un malloc par exemple).*/
#define ERR_ALLOC -2

/*Constante pour marquer une valeur maximale infinie. Utilisee pour le temps
  mis d'une station a une autre, lorsqu'il n'existe pas de chemin entre elles. */
#define INFINI ((unsigned int)~0)

/*La structure d'une station de metro */
typedef struct{
  wchar_t nom[BM_MAX_STR]; /*Le nom de la station*/
}BM_Station;

/*La structure d'une ligne de metro*/
typedef struct{
  wchar_t nom[BM_MAX_STR];    /*Le nom de la ligne de metro*/
  unsigned int indDepart;      /*La premiere station de la ligne*/
  unsigned int indTerminus;    /*La station terminus de la ligne*/
}BM_Ligne;

/*La reference de la ligne qui est sur un arc, ainsi que la direction prise par cet arc
  (ligne/2) = numero de la ligne
  (ligne%2) = direction de l'arc
  0 pour "vers le terminus",
  1 pour "vers la station de depart"*/
typedef unsigned int BM_refLigne;

/*Macro permettant d'obtenir le numero d'une ligne, a partir de sa reference sur un arc*/
#define getNumRefLig(refLig) ((refLig) >> 1)

/*Macro permettant d'obtenir la direction d'un arc, a partir de sa reference*/
#define getDirRefLig(refLig) ((refLig) & 1)

/*La structure de cellule, utilisee par le graphe des stations de metro.*/
typedef struct cel{
  unsigned int indStation; /*L'indice de la station (succeseur) dans le graphe vers lequel il y a un arc*/
  BM_refLigne reflig;      /*Reference de la ligne correspondant a cet arc, chaque arc etant une portion de ligne*/    
  struct cel *suiv;        /*Pointeur vers un autre successeur*/
}BM_Cellule;

/*La structure de graphe liant les differentes stations de metro*/
typedef struct{
  BM_Cellule **gr;           /*Liste d'adjacence du graphe. A chaque sommet (station) correspond
			                    la liste des stations qui lui succedent directement, par une ligne*/
}BM_Graphe;

/*La structure principale contenant toutes informations liees au reseau de metro*/
typedef struct{
  BM_Station *station;        /*Tableau contenant les informations sur chacune des stations de metro*/
  unsigned int nbStations;    /*Le nombre de stations du graphe*/
  BM_Ligne *ligne;            /*Tableau contenant les informations sur les lignes de metro*/
  unsigned int nbLignes;      /*Le nombre de lignes de metro*/
  BM_Graphe plan;             /*Le graphe des stations de metro*/
}BM_Reseau;

/*La structure donnant les temps de changement entre deux stations*/
typedef struct{
  unsigned int tpsDepl;  /*Le temps necessaire pour passer d'une station a une autre sans changer de ligne*/
  unsigned int tpsChgt;  /*le temps necessaire pour changer de ligne (correspondance)*/
} BM_Delais;

/*Fonction qui initialise un reseau a vide*/
void initialiserReseau(BM_Reseau *bm);

/*Fonction qui permet d'ajouter une nouvelle ligne dans le reseau*/
unsigned int ajouterLigne(BM_Reseau *bm, wchar_t *nomLigm);

/*Fonction qui permet d'ajouter une nouvelle station dans le reseau*/
unsigned int ajouterStation(BM_Reseau *bm, wchar_t *nomSta);

/*Fonction qui permet de rechercher une ligne dans le reseau,
  on retourne -1 si on ne la trouve pas, ou son indice dans bm sinon.*/
unsigned int rechercherLigne(BM_Reseau *bm, wchar_t *nomLigm);

/*Fonction qui permet de rechercher une station dans le reseau,
  on retourne -1 si on ne la trouve pas, ou son indice dans bm sinon.*/
unsigned int rechercherStation(BM_Reseau *bm, wchar_t *nomSta);

/*Fonction qui ajoute une transition entre deux stations (dans les deux sens), selon la ligne 
  de numero numLigne. On une transition de numStap vers numStas dans le sens "vers le terminus"*/
char ajouterTransition(BM_Reseau *,unsigned int numStap, unsigned int numStas, unsigned int numLigne);

/*Macro qui defini la station de depart d'une ligne dans le reseau*/
#define definirStaDepLig(br,indLig,indSta) ((br).ligne[(indLig)].indDepart = (indSta))

/*Macro qui defini la station d'arrivee d'une ligne dans le reseau*/
#define definirStaArrLig(br,indLig,indSta) ((br).ligne[(indLig)].indTerminus = (indSta))

/*Fonction qui supprime toute la memoire utilise par un reseau*/
void supprimerReseau(BM_Reseau *br);

#endif
