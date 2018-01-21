/* **************************************************/
/*   Projet      : Balade dans le metro             */
/*   Date        : mai 2009                         */
/*   Auteur      : Climoo                           */
/*   Fichier     : bm_chemin.h                      */ 
/*   Description : En-tete du module charge         */
/*                 de calculer le plus court        */
/*                 chemin entre deux sommets        */
/*                                                  */
/* **************************************************/

#ifndef _BM_CHEMIN_H

#define _BM_CHEMIN_H

/*Structure resultat de recherche du plus court chemin a partir d'un sommet du graphe*/
typedef struct{
   unsigned int *distance; /*Tableau des distances des plus courts chemins vers chaque sommet*/
   unsigned int *staprec;  /*Tableau des stations predecesseurs pour connaitre le chemin vers chaque sommet*/
   BM_refLigne  *ligprec;  /*Tableau des lignes predecesseurs pour connaitre la ligne empruntee pour arriver a un sommet*/
}BM_Pcc; 
/*Voir documentation pour plus d'explications.*/

/*Fonction qui permet de trouver tous les plus courts chemins depuis un station d'indice dep dans 
  le reseau br. Les delais sont renseignes dans bd. On retourne la strucutre BM_Pcc en resultat,
  ou (BM_Pcc*)-1 si erreur d'allocation dynamique de memoire.*/
BM_Pcc *trouverPlusCourtsChemins(BM_Reseau *br, BM_Delais *bd, unsigned int dep);

/*Fonction qui permet de liberer la memoire utilisee par un pcc.*/
void supprimerPlusCourtChemin(BM_Pcc *pcc);

#endif
