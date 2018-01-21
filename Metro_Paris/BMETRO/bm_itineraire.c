/* **************************************************/
/*   Projet      : Balade dans le metro             */
/*   Date        : mai 2009                         */
/*   Auteur      : Climoo                           */
/*   Fichier     : bm_itineraire.h                  */
/*   Description : Module charge                    */
/*                 de calculer un itineraire        */
/*                 a partir d'une requete et d'un   */
/*                 reseau de metro                  */
/*                                                  */
/* **************************************************/

#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include "bm_reseau.h"
#include "bm_utiles.h"
#include "bm_requete.h"
#include "bm_chemin.h"
#include "bm_itineraire.h"
#include "bm_itineraireInt.h"
#include "bm_itineraireOpt.h"


/*Fonction qui affiche que la station nomSta n'est pas connue du reseau*/
static void afficherErreurStation(wchar_t *nomSta);


static void afficherErreurStation(wchar_t *nomSta){
  afficherChaine(nomSta);
  afficherChaine(L" : Station inconnue du reseau\n");
}

char concatenerItineraires(BM_Itineraire *iti1, BM_Itineraire *iti2, unsigned int tpsChgt){
  BM_Etape *nv= (BM_Etape*)malloc((iti1->nbEtapes + iti2->nbEtapes)*sizeof(BM_Etape));
  if (nv == NULL)
    return ERR_ALLOC;
  memcpy(nv, iti2->etape, (iti2->nbEtapes)*sizeof(BM_Etape));
  memcpy(nv + iti2->nbEtapes,iti1->etape, (iti1->nbEtapes)*sizeof(BM_Etape));
  if ((iti1->nbEtapes > 0) && (iti2->nbEtapes > 0)){ /*S'il y avait au moins une etape dans les itineraires*/
    nv[iti2->nbEtapes - 1].tps += tpsChgt ; /*Un temps de changement en plus est necessaire, meme si on reste sur la meme ligne*/
    iti1->tpsTotal += tpsChgt ;
  }
  free(iti1->etape);
  iti1->etape = nv;
  iti1->nbEtapes += iti2->nbEtapes;
  iti1->tpsTotal += iti2->tpsTotal;
  iti1->nbStaTotal += iti2->nbStaTotal;
  return 1;
}


char faireItineraireSimple(BM_Pcc *pcc,BM_Reseau *br, BM_Ind_Requete *bq, BM_Itineraire *iti){
  BM_refLigne ref;
  unsigned int indSta;
  unsigned int tps, nbStaVisites;
  unsigned int indEtape = 0;
  if (pcc->distance[bq->indArrivee] == (unsigned int)~0){ /*S'il n'existe pas de chemin*/
    iti->nbEtapes = -1; /*signifie que l'on ne peut atteindre la destination*/
    iti->etape = NULL;
    return 1;
  }
  iti->etape = (BM_Etape *)malloc(sizeof(BM_Etape)*br->nbStations);
  iti->tpsTotal = 0;
  iti->nbStaTotal = 0;
  if (iti->etape == NULL){
    supprimerPlusCourtChemin(pcc);
    return ERR_ALLOC;
  }
  ref = pcc->ligprec[bq->indArrivee];
  indSta = bq->indArrivee;
  while ( indSta != bq->indDepart){ /*Tant que l'on est pas parvenu a la station de depart*/
    tps = 0; /*temps initial de l'etape*/
    nbStaVisites = 0; /*nombre initial de stations visites de cette etapes*/
    while( (indSta != bq->indDepart)  && (ref == pcc->ligprec[indSta]) ){ /*Tant que l'on parcourt la meme ligne*/
      indSta = pcc->staprec[indSta];
      tps += bq->delais.tpsDepl;
      nbStaVisites ++ ;
    }
    if (indSta != bq->indDepart) /*Si ce n'est pas la premiere etape*/
      tps += bq->delais.tpsChgt; /*On a du changer de ligne: temps de l'etape augmente*/
    /*Initialisation de l'etape*/
    iti->etape[indEtape].reflig = ref ;
    iti->etape[indEtape].indStaArret = bq->indArrivee;
    iti->etape[indEtape].tps = tps;
    iti->etape[indEtape].nbStations = nbStaVisites ;
    iti->tpsTotal += tps;
    iti->nbStaTotal += nbStaVisites;
    indEtape++;
    ref = pcc->ligprec[indSta];
    bq->indArrivee = indSta;
  }
  /*Reajustement de la taille du tableau d'etapes*/
  iti->etape = (BM_Etape*)realloc(iti->etape,sizeof(BM_Etape)*(indEtape));
  if ((iti->etape == NULL) && (indEtape > 1)){
    supprimerPlusCourtChemin(pcc);
    return ERR_ALLOC;
  }
  iti->nbEtapes = indEtape;
  return 1;
}

char trouverItineraire(BM_Reseau *br, BM_Requete *req, BM_Itineraire *iti){
  BM_Pcc *pcc;
  BM_Ind_Requete indreq;
  unsigned int i = 0;
  char ok ;
  indreq.delais.tpsChgt = req->delais.tpsChgt;
  indreq.delais.tpsDepl = req->delais.tpsDepl;
  indreq.indDepart = rechercherStation(br,req->nomDepart);
  indreq.indArrivee = rechercherStation(br,req->nomArrivee);
  if (indreq.indDepart == (unsigned int) -1){/*station de depart inexistante*/
    for (i = 0; i < wcslen(req->nomDepart) ; i ++)
      afficherEntier(req->nomDepart[i]);
    afficherErreurStation(req->nomDepart);
    return 0;
  }
  if (indreq.indArrivee == (unsigned int) -1){/*station d'arrivee inexistante*/
    afficherErreurStation(req->nomArrivee);
    return 0;
  }
  if (req->nbInterm > 0){ /*Recherche des indices des stations intermediaires*/
    indreq.nbInterm = req->nbInterm;
    indreq.indInterm = (unsigned int *)malloc(req->nbInterm*sizeof(unsigned int));
    if (indreq.indInterm == NULL)
      return ERR_ALLOC;
    while(i < indreq.nbInterm){
      indreq.indInterm[i] = rechercherStation(br,req->nomInterm[i]);
      if (indreq.indInterm[i] == (unsigned int)-1) {/*station intermediaire inexistante*/
	afficherErreurStation(req->nomInterm[i]);
	free(indreq.indInterm);
	return 0;
      }
      i++;
    }
  }
  if (req->nbInterm > 0){
    if ((req->optim) && (req->nbInterm > 1))
      ok = faireItineraireMultiOpt(br,&indreq,iti);
     /*  ok = faireItineraireMulti(br,&indreq,&(req->delais),iti); */
    else 
      ok = faireItineraireMulti(br,&indreq,iti);
  }
  else{
    pcc = trouverPlusCourtsChemins(br,&(req->delais),indreq.indDepart);
    if (pcc == (BM_Pcc*)ERR_ALLOC)
      return ERR_ALLOC;
    ok = faireItineraireSimple(pcc,br,&indreq,iti);
    supprimerPlusCourtChemin(pcc);
  }
  if (req->nbInterm > 0)
    free(indreq.indInterm);
  return ok;
}


void supprimerItineraire(BM_Itineraire *iti){
  free(iti->etape);
}
