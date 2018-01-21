/****************************************************/
/*   Projet      : Balade dans le metro             */
/*   Date        : mai 2009                         */
/*   Auteur      : Climoo                           */
/*   Fichier     : bm_reseau.c                      */
/*   Description : Module contenant les             */
/*   principales definitions de structures du projet*/
/*                                                  */
/* **************************************************/


#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include "bm_reseau.h"
#include "bm_utiles.h"

/*Increment de la taille du tableau de ligne de la structure BM_Reseau*/
#define BM_INC_LIG 16

/*Increment de la taille du tableau de stations de la structure BM_Reseau*/
#define BM_INC_STA 64

static char insererCellule(BM_Cellule **liste, unsigned int numSta, BM_refLigne reflig){
  BM_Cellule *nouv;
  nouv = (BM_Cellule*)malloc(sizeof(BM_Cellule));
  if (nouv == NULL)
    return ERR_ALLOC;
  nouv->indStation = numSta;
  nouv->reflig = reflig;
  nouv->suiv = *liste;
  *liste = nouv;
  return 1;
}
 
unsigned int rechercherLigne(BM_Reseau *bm, wchar_t *nomLig){
  unsigned int i = 0;
  while ((i < bm->nbLignes)){
    if (comparerNoms(nomLig,bm->ligne[i].nom))
      return i;
    i++;
  }
  return -1;
}

unsigned int rechercherStation(BM_Reseau *bm,wchar_t *nomSta){
  unsigned int i = 0;
  while (i < bm->nbStations){
    if (comparerNoms(nomSta,bm->station[i].nom))
      return i;
    i++;
  }
  return -1;
}

void initialiserReseau(BM_Reseau *bm){
  bm->station = NULL;
  bm->ligne = NULL;
  bm->nbStations = 0;
  bm->nbLignes = 0;
  (bm->plan).gr = NULL;
}

unsigned int ajouterLigne(BM_Reseau *bm,wchar_t *nomLigm){
  unsigned int res = rechercherLigne(bm,nomLigm);
  if (res == (unsigned int)-1){ /*Si on a pas trouve la ligne*/
    if ((bm->nbLignes % BM_INC_LIG) == 0){ /*Si plus de place dans le tableau*/
        bm->ligne = (BM_Ligne*)realloc(bm->ligne,(BM_INC_LIG + bm->nbLignes)*sizeof(BM_Ligne));
      if (bm->ligne == NULL) /*Si erreur d'allocation dynamique*/
	return ERR_ALLOC;
    }
    wcscpy(bm->ligne[bm->nbLignes].nom,nomLigm);
    res = bm->nbLignes ++;
  }
  return res;
}


unsigned int ajouterStation(BM_Reseau *bm,wchar_t *nomSta){
  unsigned int res = rechercherStation(bm,nomSta);
  if (res == (unsigned int)-1){ /*Si on a pas trouve la station*/
    if ((bm->nbStations % BM_INC_STA) == 0){  /*Si plus de place dans le tableau*/
      bm->station = (BM_Station*)realloc(bm->station,(BM_INC_STA + bm->nbStations)*sizeof(BM_Station));
      (bm->plan).gr = (BM_Cellule **)realloc((bm->plan).gr,(BM_INC_STA + bm->nbStations)*sizeof(BM_Cellule*));
      if((bm->station == NULL) || ((bm->plan).gr == NULL)) /*Si erreur d'allocation dynamique*/
	return ERR_ALLOC;
      memset((bm->plan).gr + bm->nbStations, 0, (BM_INC_STA)*sizeof(BM_Cellule*));/*Initialisation des cases a NULL*/
    }
    wcscpy(bm->station[bm->nbStations].nom,nomSta);
    res = bm->nbStations ++;
  }      
  return res;
}

char ajouterTransition(BM_Reseau *br,unsigned int numStaDep, unsigned int numStaArr, unsigned int numLigne){
  if (insererCellule( &(br->plan.gr[numStaDep]), numStaArr, (numLigne<<1)) == ERR_ALLOC ) /*Arc vers le terminus*/
    return ERR_ALLOC;
  return insererCellule( &(br->plan.gr[numStaArr]), numStaDep, (numLigne<<1) + 1); /*Arc vers le depart*/
}

void supprimerReseau(BM_Reseau *br){
  BM_Cellule *cel, *celt;
  unsigned int i = 0;
  free(br->station);
  free(br->ligne);
  if (br->plan.gr != NULL){
    while (i < br->nbStations){
      cel=br->plan.gr[i];
      while (cel != NULL){
	celt = cel->suiv;
	free(cel);
	cel = celt ;
      }     
      i++;
    }
    free(br->plan.gr);
  }
}

/*
int main(void){
  BM_Reseau br;
  unsigned int i=0;
  initialiserReseau(&br);
  setlocale(LC_ALL,"");
  ajouterLigne(&br,L"Ligne 1");
  ajouterLigne(&br,L"Ligne 2");
  ajouterStation(&br,L"Mont");
  ajouterStation(&br,L"Pigalle");
 
  for (i = 0; i < br.nbStations ;i++){
    fputws(br.station[i].nom,stdout);
    putwchar('\n');
  }
  for (i = 0; i < br.nbLignes ;i++){
    fputws(br.ligne[i].nom,stdout);
    putwchar('\n');
  }
  fputws(L"Reussi\n",stdout);
  return 0;
}*/
