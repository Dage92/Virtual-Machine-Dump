/* **************************************************/
/*   Projet      : Balade dans le metro             */
/*   Date        : mai 2009                         */
/*   Auteur      : Climoo                           */
/*   Fichier     : bm_requete.h                     */
/*   Description : En-tete du module charge         */
/*   d'initialiser la requete formulee par          */
/*   l'utilisateur, a partir des parametres         */
/*   passes au programme                            */
/*                                                  */
/* **************************************************/

#ifndef _BM_REQUETE_H

#define _BM_REQUETE_H

/*La structure d'une requete, ou demande de chemin entre deux stations de metro*/
typedef struct{
  wchar_t nomDepart[BM_MAX_STR];    /*La station de depart du chemin*/
  wchar_t nomArrivee[BM_MAX_STR];   /*La station d'arrivee du chemin*/
  wchar_t (*nomInterm)[BM_MAX_STR]; /*Les eventuelles stations intermediaires a traverser*/
  unsigned int nbInterm;            /*Le nombre de stations intermediaires*/
  char optim;                       /*Indique si on doit chercher un chemin optimal, 1 si oui, 0 sinon*/  
  BM_Delais delais;                 /*Les delais entre les stations*/
}BM_Requete;

/*Fonction qui permet d'initialiser une requete, a partir des parametres passes
  au programme lors de son demarrage. Les elements inconnus mais necessaires sont 
  demandes a l'utilisateur. La fonction affiche des erreurs si les parametres ne sont
  pas coherent. L'indice dans argv du nom du fichier est place dans indNonFic,
  de facon a ce que le fichier puisse etre ouvert ensuite.
  Retourne 1 en cas de reussite, ERR_ALLOC en cas d'erreur d'allocation dynamique, 0
  si echec de la creation de la requete car parametres incoherents.*/
char initialiserRequete(int argc, char **argv, BM_Requete *br, int *indNomFic);

/*Fonction qui supprime toute la memoire utilisee par une requete.*/
void supprimerRequete(BM_Requete *br);

#endif
