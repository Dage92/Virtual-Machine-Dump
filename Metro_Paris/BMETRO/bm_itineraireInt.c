/* **************************************************/
/*   Projet      : Balade dans le metro             */
/*   Date        : mai 2009                         */
/*   Auteur      : Climoo                           */
/*   Fichier     : bm_itineraire.h                  */
/*   Description : Module charge                    */
/*                 de calculer un itineraire        */
/*                 multiple (stations               */
/*                 intermediaires non optimise)     */ 
/*                                                  */
/* **************************************************/

#include <wchar.h>
#include <stdlib.h>
#include "bm_reseau.h"
#include "bm_chemin.h"
#include "bm_requete.h"
#include "bm_itineraire.h"

/*Calcul du chemin : depart => int[0], puis int[0] => int[1], puis ..., puis int[N] => arrivee
  L'itineraire multiple est juste la concatenation de plusieurs itineraires simples*/
char faireItineraireMulti(BM_Reseau *br, BM_Ind_Requete *bq, BM_Itineraire *iti){
  unsigned int i = 0;
  BM_Itineraire tmp; /*Itineraire temporaire*/
  BM_Ind_Requete rtmp = *bq; /*Requete temporaire*/
  BM_Pcc *pcc;
  char ok = 1;
  char existeChemin = 1;
  iti->tpsTotal = 0;
  iti->nbStaTotal = 0;
  iti->nbEtapes = 0;
  iti->etape = NULL;
  while((i < bq->nbInterm) && (ok) && (existeChemin)){ /*Calcul des chemins intermediaires*/
    rtmp.indArrivee = bq->indInterm[i];
    pcc = trouverPlusCourtsChemins(br,&(bq->delais),rtmp.indDepart);
    ok = (pcc != (BM_Pcc*)ERR_ALLOC);
    ok = ((ok) && (faireItineraireSimple(pcc,br,&rtmp,&tmp) != ERR_ALLOC)); /*ok recoit 0 si erreur d'allocation*/
    if (ok)
      supprimerPlusCourtChemin(pcc);
    existeChemin = (tmp.nbEtapes != (unsigned int)-1); /*Si pas de chemin, existeChemin recoit 0*/
    if (existeChemin)
      ok = (ok) && (concatenerItineraires(iti,&tmp,bq->delais.tpsChgt) != ERR_ALLOC); /**/     
    free(tmp.etape);
    rtmp.indDepart = bq->indInterm[i];
    i++;
  }
  if (ok && existeChemin){
    rtmp.indArrivee = bq->indArrivee;
    pcc = trouverPlusCourtsChemins(br,&(bq->delais),rtmp.indDepart);
    ok = (pcc != (BM_Pcc*)ERR_ALLOC);
    ok = ((ok) && (faireItineraireSimple(pcc,br,&rtmp,&tmp) != ERR_ALLOC));
    if (ok)
      supprimerPlusCourtChemin(pcc);
    existeChemin = (tmp.nbEtapes != (unsigned int)-1); 
    if (existeChemin)
      ok = (ok && (concatenerItineraires(iti,&tmp,bq->delais.tpsChgt) != ERR_ALLOC));
    free(tmp.etape);
  }
  if (!existeChemin){ /*S'il a ete impossible de rejoindre une des stations, intermediaires ou non*/
    iti->etape = NULL;  
    iti->nbEtapes = -1;
  }
  if (!ok)
    return ERR_ALLOC;
  return 1;
}
