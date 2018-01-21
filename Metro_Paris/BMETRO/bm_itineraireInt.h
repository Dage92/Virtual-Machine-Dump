/* **************************************************/
/*   Projet      : Balade dans le metro             */
/*   Date        : mai 2009                         */
/*   Auteur      : Climoo                           */
/*   Fichier     : bm_itineraire.h                  */
/*   Description : En-tete du module charge         */
/*                 de calculer un itineraire        */
/*                 multiple (stations               */
/*                 intermediaires non optimise)     */ 
/*                                                  */
/* **************************************************/

#ifndef _BM_ITINERAIREINT_H

#define _BM_ITINERAIREINT_H

/*Fonction qui calcule un itineraire multiple (avec des stations intermediaires),
  non optimise. L'itineraire fourni est celui qui passe par toutes les stations 
  intermediaires, mais dans l'ordre dans lequel elles apparaissent.*/
char faireItineraireMulti(BM_Reseau *br, BM_Ind_Requete *bq, BM_Itineraire *iti);

#endif
