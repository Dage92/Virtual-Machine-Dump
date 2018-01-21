/* **************************************************/
/*   Projet      : Balade dans le metro             */
/*   Date        : mai 2009                         */
/*   Auteur      : Climoo                           */
/*   Fichier     : bm_affichage.h                   */
/*   Description : En-tete du module charge         */
/*                 d'afficher les informations      */
/*		   liees a un itineraire            */
/*                                                  */
/* **************************************************/

#ifndef _BM_AFFICHAGE_H

#define _BM_AFFICHAGE_H

/*Fonction qui permet l'affichage d'un itineraire iti. Le reseau
  correspondant doit etre fourni.*/
void afficherItineraire(BM_Itineraire *iti, BM_Reseau *br);

/*Fonction qui realise l'affichage d'une requete quelconque.*/
void afficherRequete(BM_Requete *req);

#endif /*bm_affichage.h*/
