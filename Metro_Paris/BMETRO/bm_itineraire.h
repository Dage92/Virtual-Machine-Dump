/* **************************************************/
/*   Projet      : Balade dans le metro             */
/*   Date        : mai 2009                         */
/*   Auteur      : Climoo                           */
/*   Fichier     : bm_itineraire.h                  */
/*   Description : En-tete du module charge         */
/*                 de calculer un itineraire        */
/*                 a partir d'une requete.          */
/*                                                  */
/* **************************************************/

#ifndef _BM_ITINERAIRE_H

#define _BM_ITINERAIRE_H

/*La structure d'une etape d'un itineraire. Equivaut a une instruction a donner
 a quelqu'un qui checherait son itineraire*/
typedef struct{
  BM_refLigne reflig;       /*Numero de la ligne et direction dans laquelle l'emprunter*/
  unsigned int indStaArret; /*Numero de la station ou il faut s'arreter*/
  unsigned int tps;         /*Temps necessaire pour cette etape*/
  unsigned int nbStations;  /*Nombre de stations visitees au cours de l'etape*/
} BM_Etape;

/*La structure d'un itineraire a donner pour se rendre d'une station a une autre*/
typedef struct{
  unsigned int nbEtapes;   /*Le nombre de l'etape de l'itineraire*/
  BM_Etape *etape;         /*Le tableau des differentes etapes de l'itineraire*/
  unsigned int tpsTotal;   /*Le temps total necessaire pour l'itineraire*/
  unsigned int nbStaTotal; /*Le nombre total de stations visitees*/
}BM_Itineraire;
  
/*La structure des indices des stations dans le reseau, demandees dans une requete*/
typedef struct{
  unsigned int indDepart;  /*L'indice de la station de depart*/
  unsigned int indArrivee; /*L'indice de la station d'arrivee*/
  unsigned int *indInterm; /*Les indices des stations intermediaires*/
  unsigned int nbInterm;   /*Le nombre de stations intermediaire*/
  BM_Delais delais;        /*Les delais entre les stations*/
}BM_Ind_Requete;

/*Fonction qui met bout a bout deux itineraires. Le resultat se trouve dans iti1.
  On obtient l'itineraire qui realise d'abord iti1, puis iti2.
  Le temps de changement doit etre donne en parametre, car necessairement une
  correspondance en plus est necessaire pour le nouveau itineaire iti1.
  Retourne 1 en cas de succes, ERR_ALLOC sinon*/
char concatenerItineraires(BM_Itineraire *iti1, BM_Itineraire *iti2,unsigned int tpsChgt);

/*Fonction qui etant une donne une requete bq, realise un itineraire simple, de la station
  de depart vers la station d'arrivee, sans aucun intermediaire. Le pcc a partir de la 
  station de depart doit etre initialise avant et passe en parametre a la fonction.
  Itineraire resultant dans iti.
  Retourne 1 en cas de succes, ERR_ALLOC sinon*/
char faireItineraireSimple(BM_Pcc *pcc, BM_Reseau *br, BM_Ind_Requete *bq, BM_Itineraire *iti);

/*Fonction qui etant un reseau et une requete quelconque, realise un itineraire.
  Resultat dans iti.
  Retourne 1 en cas de succes, ERR_ALLOC sinon*/
char trouverItineraire(BM_Reseau *br, BM_Requete *req, BM_Itineraire *iti);

/*Fonction qui permet la suppression en memoire d'un itineraire*/
void supprimerItineraire(BM_Itineraire *);


#endif /*bm_itineraire.h*/
