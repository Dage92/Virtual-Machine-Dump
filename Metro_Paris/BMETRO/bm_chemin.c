/****************************************************/
/*   Projet      : Balade dans le metro             */
/*   Date        : mai 2009                         */
/*   Auteur      : Climoo                           */
/*   Fichier     : bm_chemin.c                      */ 
/*   Description : Module charge                    */
/*                 de calculer le plus court        */
/*                 chemin entre deux sommets        */
/*                                                  */
/****************************************************/

#include <stdlib.h>
#include "bm_reseau.h"
#include "bm_chemin.h"


/*Fonction qui initiliase la structure d'un pcc, champ aux valeurs par defaut.*/
static BM_Pcc *initialiserPcc(BM_Reseau *bm, unsigned int dep, unsigned int tps);

/*Fonction qui permet de choisir le sommet qui est a la distance la plus courte,
  et n'appartenant pas aux sommets dejas vus.*/
static unsigned int choisirSommet(BM_Pcc *pcc, char *cvus, unsigned int nbSta);

static BM_Pcc *initialiserPcc(BM_Reseau *bm, unsigned int dep, unsigned int tps){
  unsigned int i = 0;
  BM_Pcc *res;
  BM_Cellule *cel;
  res = (BM_Pcc *)calloc(1,sizeof(BM_Pcc));
  if (res == NULL)
    return NULL;
  res->distance = (unsigned int *)malloc(sizeof(unsigned int)*bm->nbStations);
  if (res->distance == NULL){
    free(res); 
    return NULL;
  }
  res->staprec = (unsigned int *)malloc(sizeof(unsigned int)*bm->nbStations);
  if (res->staprec == NULL){
    free(res->distance); free(res);
    return NULL;
  }
  res->ligprec = (unsigned int *)malloc(sizeof(unsigned int)*bm->nbStations);
  if (res->ligprec == NULL){
    free(res->staprec); free(res->distance); free(res);
    return NULL;
  }   
  while (i < bm->nbStations){
    res->distance[i] = INFINI;
    res->staprec[i] = dep;
    res->ligprec[i] = -1;
    i++;
  }
  res->distance[dep] = 0;
  cel = bm->plan.gr[dep];
  while (cel != NULL){
    res->distance[cel->indStation] = tps;
    res->ligprec[cel->indStation] = cel->reflig;
    cel = cel->suiv;
  }  
  return res;
}

static unsigned int choisirSommet(BM_Pcc *pcc, char *cvus, unsigned int nbSta){
  unsigned int i =0;
  unsigned int dmin = INFINI;
  unsigned int res = (unsigned int)-1;
  while (i < nbSta){
    if (cvus[i] == 0){ /*Si ce sommet n'a pas deja ete visite*/
      if (pcc->distance[i] < dmin){
	dmin = pcc->distance[i];
	res = i;
      }
    }
    i++;
  }
  return res;
}

BM_Pcc *trouverPlusCourtsChemins(BM_Reseau *br, BM_Delais *bd, unsigned int dep){
  char *cvus;
  BM_Pcc *res;
  BM_Cellule *cely; 
  unsigned int  x, y;
  cvus = (char *)calloc(br->nbStations,sizeof(char)); /*sommets deja parcourus*/
  if (cvus == NULL) /*Erreur allocation memoire*/
    return (BM_Pcc *)ERR_ALLOC; 
  res = initialiserPcc(br,dep, bd->tpsDepl); /*la structure pcc resultante*/
  if (res == NULL){/*Erreur allocation memoire*/
    free(cvus);
    return (BM_Pcc *)ERR_ALLOC;
  }
  cvus[dep] = 1;
  x = choisirSommet(res,cvus,br->nbStations);
  while( x != (unsigned int)-1){
    cvus[x] = 1 ;
    cely = br->plan.gr[x];
    while (cely != NULL){
      y = cely->indStation;
      if (cvus[y] == 0){ /*si le sommet y n'appartient pas aux "deja vus"*/
	/*si on a pas eu besoin de changer de ligne pour aller a y*/
	if (res->ligprec[x] == cely->reflig){ 
	  if (res->distance[y] > res->distance[x] + bd->tpsDepl){
	    res->distance[y] = res->distance[x] + bd->tpsDepl;
	    res->staprec[y] = x;
	    res->ligprec[y] = cely->reflig ;
	    
	  }
	}
	else{ /*Si un changement de ligne est necessaire pour passer de x a y*/
	  if (res->distance[y] > res->distance[x] + bd->tpsChgt + bd->tpsDepl){
	    res->distance[y] = res->distance[x] + bd->tpsChgt + bd->tpsDepl;
	    res->staprec[y] = x;
	    res->ligprec[y] = cely->reflig ;
	  }
	}
      }
      cely = cely->suiv;
    }
    x = choisirSommet(res,cvus,br->nbStations);
  }
  free(cvus);
  return res;
}     

void supprimerPlusCourtChemin(BM_Pcc *pcc){
  free(pcc->distance);
  free(pcc->ligprec);
  free(pcc->staprec);
  free(pcc);
  return ;
}




