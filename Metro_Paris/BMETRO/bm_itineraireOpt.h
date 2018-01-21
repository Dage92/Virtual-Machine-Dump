/* **************************************************/
/*   Projet      : Balade dans le metro             */
/*   Date        : mai 2009                         */
/*   Auteur      : Climoo                           */
/*   Fichier     : bm_itineraire.h                  */
/*   Description : En-tete du module charge         */
/*                 de calculer un itineraire        */
/*                 optimise a partir d'une requete. */
/*                                                  */
/* **************************************************/

#ifndef _BM_ITINERAIREOPT_H

#define _BM_ITINERAIREOPT_H

/*Fonction qui calcule un itineraire optimise a partir d'une requete bq sur le reseau br.
  Itineraire resultant dans iti.
  Renvoie 1 si reussite, ERR_ALLOC si erreur d'allocation memoire.*/
char faireItineraireMultiOpt(BM_Reseau *br, BM_Ind_Requete *bq, BM_Itineraire *iti);

#endif
