/* **************************************************/
/*   Projet      : Balade dans le metro             */
/*   Date        : mai 2009                         */
/*   Auteur      : Climoo                           */
/*   Fichier     : bm_itineraire.h                  */
/*   Description : Module charge                    */
/*                 de calculer un itineraire        */
/*                 optimise a partir d'une requete. */
/*                                                  */
/* **************************************************/

#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <stdio.h>
#include "bm_reseau.h"
#include "bm_requete.h"
#include "bm_chemin.h"
#include "bm_itineraire.h"
#include "bm_itineraireOpt.h"
#include "bm_utiles.h"

/*Constante definissant le nombre de stations a partir duquel on va chercher
  un itineraire veritablement optimal. Au dela de cette limite, on va seulement
  donner une approximation de l'itineraire le plus court.*/
#define BM_SEUIL_OPTM 8

/*Creation d'un arbre : 
  Racine : Station de depart
  Feuilles : Station d'arrivee

Trouver un chemin optimal <=> Trouver un chemin de la racine vers la feuille qui possede la
plus courte distance.

*/

/*Structure d'un noeud de l'arbre pour effectuer la recherche d'un chemin optimal*/
typedef struct bm_noeud{
  unsigned int numSta;      /*Le numero de la station du noeud, dans le reseau*/
  unsigned int numStaTab;   /*L'indice de la station dans le tableau des stations intermediaires de la requete*/
  unsigned int temps;       /*Le temps mis pour parvenir a cette station*/
  struct bm_noeud *pere;    /*Pointeur vers le pere de ce noeud*/
  struct bm_noeud *filsg;   /*Pointeur vers le fils gauche de ce noeud*/
  struct bm_noeud *frered;  /*Pointeur vers le frere droit de ce noeud*/
} BM_Noeud;

/*Macro pour desallouer la memoire utilisee par un noeud*/
#define supprimerNoeud(noeud) (free((noeud)))

/*Fonction creant un nouveau noeud de l'arbre.
  Les champs du fils, du premier frere droit et du pere, sont mis a NULL*/
static BM_Noeud *creerNoeud(unsigned int numSta,unsigned int numStaTab, unsigned int temps);

/*Fonction permettant de liberer toute la memoire utilisee par un arbre.*/
static void libererArbre(BM_Noeud *nd);

/*Fonction qui initialise tous les plus courts chemins necessaires a une requete
  (depuis la station de depart, et depuis chaque station intermediaire)*/
static BM_Pcc **initialiserPccs(BM_Reseau *br, BM_Ind_Requete *bq);

/*Fonction qui supprime toutes la memoire utilises par un tableau de pcc*/
static void supprimerPccs(BM_Pcc **pccs, unsigned int nbpccs);

/*Fonction qui certifie avant recherche l'existence d'un chemin entre
  les stations d'une requete*/
static char verifierExistenceChemin(BM_Pcc *, BM_Ind_Requete *);

/*Fonction qui renvoie un entier vers la prochaine station non visitee, a partir de l'indice ind
  dans le tableau visites.*/
static unsigned int prochainNonVisite(unsigned int ind, char *visites, unsigned int nbInter);
  
/*Fonction qui cree l'arbre de recherche d'un chemin optimal*/
static char creerArbre(BM_Noeud *noeud,BM_Pcc **pccs, char *visites,BM_Ind_Requete *, BM_Noeud **mfeuil);

/*Fonction qui cree l'itineraire du chemin optimal depuis une feuille de l'arbre, en remontant
  jusque la racine.*/
static char creerCheminOptimal(BM_Reseau *br,BM_Noeud *noeud,BM_Pcc **pccs,BM_Ind_Requete *bq,BM_Itineraire *iti);

/*Fonction qui recherche la station la plus proche d'une autre. Le pcc en parametre
  doit etre celui de la station de depart de la requete. La station la plus proche
  ne doit pas appartenir aux visites.*/
static unsigned int trouverStationPlusProche(BM_Pcc *,BM_Ind_Requete *,char *visites); 

static BM_Noeud *creerNoeud(unsigned int numSta,unsigned int numStaTab, unsigned int temps){
  BM_Noeud *res = (BM_Noeud*)malloc(sizeof(BM_Noeud));
  if (res == NULL)
    return NULL;
  res->numStaTab = numStaTab;
  res->numSta = numSta;
  res->temps = temps;
  res->filsg = res->frered = res->pere = NULL;
  return res;
}

static void libererArbre(BM_Noeud *nd){
  if (nd != NULL){
    if (nd->filsg != NULL)
      libererArbre(nd->filsg);
    if (nd->frered != NULL)
      libererArbre(nd->frered);
    supprimerNoeud(nd);
  }
  return ;
}

static BM_Pcc **initialiserPccs(BM_Reseau *br, BM_Ind_Requete *bq){
  BM_Pcc **pccs = (BM_Pcc **)malloc(sizeof(BM_Pcc*)*(bq->nbInterm + 1));
  unsigned int i = 0;
  char ok = 1;
  if (pccs == NULL)
    return (BM_Pcc **)ERR_ALLOC;
  pccs[0] = trouverPlusCourtsChemins(br,&(bq->delais),bq->indDepart);/*indice 0 : pcc depuis la station de depart*/
  ok = (pccs[0] != (BM_Pcc*)ERR_ALLOC);
  while ((i < bq->nbInterm) && (ok)) { /*indice de 1 a nbInterm + 1 : pcc depuis les stations intermediaires*/
    pccs[i + 1] = trouverPlusCourtsChemins(br,&(bq->delais),bq->indInterm[i]);
    ok = (pccs[i + 1] != (BM_Pcc*)ERR_ALLOC);
    if (ok)
      i++;
  }
  if (! ok ){ /*Si erreur d'allocation memoire*/
    supprimerPccs(pccs,i);
    return (BM_Pcc **)ERR_ALLOC;
  }
  return pccs;
}

static void supprimerPccs(BM_Pcc **pccs, unsigned int nbpccs){
  unsigned int i = nbpccs - 1;
  while (i != (unsigned int)-1){
    supprimerPlusCourtChemin(pccs[i]);
    i--;
  }
  free(pccs);
}

static char verifierExistenceChemin(BM_Pcc *pcc, BM_Ind_Requete *req){
  unsigned int i = 0;
  /*Recherche de l'existence d'un chemin jusqu'aux stations intermediaires*/
  while ((i < req->nbInterm) && (pcc->distance[req->indInterm[i]] != INFINI))
    i++;
  /*Recherche de l'existence d'un chemin vers la station d'arrivee*/
  return ((i >= req->nbInterm)  && (pcc->distance[req->indArrivee] != INFINI));
}

static unsigned int prochainNonVisite(unsigned int ind, char *visites, unsigned int nbInter){
  while ((ind < nbInter) && (visites[ind] != 0))
    ind ++;
  if (ind < nbInter)
    return ind;
  return -1;
}


static char creerCheminOptimal(BM_Reseau *br,BM_Noeud *noeud,BM_Pcc **pccs,BM_Ind_Requete *bq,BM_Itineraire *iti){
  BM_Itineraire tmp , tmp2;
  BM_Ind_Requete rtmp = *bq;
  char ok = 1;
  tmp.etape = NULL;
  if (noeud->pere != NULL){
    rtmp.indDepart = noeud->pere->numSta;
    rtmp.indArrivee = noeud->numSta;
    ok = (faireItineraireSimple(pccs[noeud->pere->numStaTab],br,&rtmp,&tmp) != ERR_ALLOC);
    ok = ((ok) && (concatenerItineraires(&tmp,iti,(rtmp.delais.tpsChgt))) != ERR_ALLOC);
    tmp2 = *iti;
    *iti = tmp;
    supprimerItineraire(&tmp2);
    ok = ((ok) && (creerCheminOptimal(br,noeud->pere,pccs,bq,iti) != ERR_ALLOC));
  }
  if (!ok)
    return ERR_ALLOC;
  return ok ;
}

static char creerArbre(BM_Noeud *noeud,BM_Pcc **pccs, char *visites,BM_Ind_Requete *bq, BM_Noeud **mfeuil){
  BM_Noeud *filsg = NULL, *frered, *prec;
  unsigned int proc, numSta, temps;
  proc = prochainNonVisite(0,visites,bq->nbInterm);
  if (proc == (unsigned int)-1){ /*Si c'est une feuille*/
    /*temps du noeud augmente du temps pour aller jusque l'arrivee*/
    noeud->temps += pccs[noeud->numStaTab]->distance[bq->indArrivee];
   
    /*Avec un itineraire plus court que celui trouve jusqu'a present */
    if ((*mfeuil == NULL) || ((*mfeuil)->temps > noeud->temps )) 
      *mfeuil = noeud; /* Alors le meilleur chemin est vers cette feuille*/
    return 1;
  }

  /*Creation du fils gauche du noeud*/
  if (proc != (unsigned int)-1){ /*S'il existe une station intermediaire par laquelle on est pas encore passee*/
    numSta = bq->indInterm[proc]; /*Le numero de la station du nouveau noeud*/
    temps = noeud->temps + pccs[noeud->numStaTab]->distance[numSta] ;/*Le temps necessaire pour y parvenir*/
    filsg = creerNoeud(numSta, proc +1, temps);
    if (filsg == NULL)
      return ERR_ALLOC;
    noeud->filsg = filsg; /*Creation des liens vers ce noeud*/
    filsg->pere = noeud;
    visites[proc] = 1;
    if (creerArbre(filsg,pccs,visites,bq,mfeuil) == ERR_ALLOC)
      return ERR_ALLOC;	
    visites[proc] = 0;
    proc ++;
  }
  /*Creations des autres fils, les freres du fils gauche*/
  proc = prochainNonVisite(proc,visites,bq->nbInterm);
  prec = filsg;
  while (proc != (unsigned int)-1){ /*Tant qu'il existe une station intermediaire par laquelle on est pas encore passee*/
    numSta = bq->indInterm[proc]; /*Le numero de la station du nouveau noeud*/
    temps = noeud->temps + pccs[noeud->numStaTab]->distance[numSta] ;/*Le temps necessaire pour y parvenir*/
    frered = creerNoeud(numSta, proc +1, temps);
    if (frered == NULL)
      return ERR_ALLOC;
    prec->frered = frered; /*Creation des liens vers ce noeud*/
    frered->pere = noeud;
    prec = frered;
    visites[proc] = 1;
    if (creerArbre(frered,pccs,visites,bq,mfeuil) == ERR_ALLOC)
      return ERR_ALLOC;	
    visites[proc] = 0;
    proc++;
    proc = prochainNonVisite(proc,visites,bq->nbInterm);
  }
  return 1;
}

static void supprimerStationsEgales(BM_Ind_Requete *bq){
  unsigned int i = 0, j;
  while (i < bq->nbInterm){
    /*Si une station intermediaire est la meme que celle de depart ou d'arrivee*/
    if ((bq->indInterm[i] == bq->indDepart) || (bq->indInterm[i] == bq->indArrivee)){
      /*Suppression de la station intermediaire (par decalage)*/
      bq->nbInterm --;
      memmove(bq->indInterm + i,bq->indInterm + i + 1 /**/, (bq->nbInterm - i)*sizeof(bq->indInterm[0]));
      
    }
    else{ /*Sinon recherche parmi les stations intermediaires*/
      j = i + 1;
      while ((j < bq->nbInterm) && (bq->indInterm[j] != bq->indInterm[i]) )
	j ++;
      if (j < bq->nbInterm){ /*Si on a trouve une station egale*/
	/*Suppression de la station intermediaire (par decalage)*/
	bq->nbInterm --;
	memmove(bq->indInterm + i,bq->indInterm + i + 1 /**/, (bq->nbInterm - i)*sizeof(bq->indInterm[0]));
      }
      else
	i++;
    }
  }
}

static unsigned int trouverStationPlusProche(BM_Pcc *pcc,BM_Ind_Requete *req,char *visites){
  unsigned int min = INFINI, indMin = -1;
  unsigned int i = 0;
  while (i < req->nbInterm){
    /*Si la station n'a pas ete deja visitee et que sa distance est plus faible que le min*/
    if  ((! visites[i])  && (pcc->distance[req->indInterm[i]] < min)){
      min = pcc->distance[req->indInterm[i]];
      indMin = i;
    }
    i++;
  }
  return indMin;
}

static char creerCheminSatisfaisant(BM_Reseau *br, BM_Pcc **pccs,BM_Ind_Requete *req, BM_Itineraire *iti,char *visites){
  BM_Itineraire tmp;
  unsigned int numSta;
  BM_Ind_Requete rtmp = *req;
  char ok = 1;
  iti->nbEtapes = 0; /*Inititialisation de l'itineraire comme vide*/
  iti->etape = NULL;
  iti->tpsTotal = 0;
  iti->nbStaTotal = 0;
  numSta = trouverStationPlusProche(pccs[0],req,visites);
  while ((numSta != (unsigned int) -1) && (ok)){
    visites[numSta] = 1;                                                /*On defini comme visitee la station la plus proche*/
    rtmp.indArrivee = req->indInterm[numSta];
    ok = (faireItineraireSimple(pccs[rtmp.indDepart],br,&rtmp,&tmp) != ERR_ALLOC); /*Creation itineraire simple vers cette station*/
    ok = ((ok) && (concatenerItineraires(iti,&tmp,req->delais.tpsChgt) != ERR_ALLOC));
    rtmp.indDepart = rtmp.indArrivee;
    numSta = trouverStationPlusProche(pccs[rtmp.indDepart],req, visites);
  }
  if (!ok)
    return ERR_ALLOC;
  return 1;
}

/*
PRINCIPE DE L'ALGORITHME:
-Creer l'ensemble des plus courts chemins depuis la station de depart,
 et depuis les stations intermediaires.
 On peut ainsi savoir rapidement quels sont les plus courts chemins
 vers n'importe quelle station du reseau.
-Si le nombre de stations intermediaire est inferieur au seuil,
  -Creer un arbre. La noeud represente une station, la racine est la station de depart,
   et chaque arc representant un chemin d'une station a une autre.
   L'arbre est complet, on a tous les chemins depuis la racine vers la station
   d'arrivee, en passant par toutes les stations intermediaires, dans tous les ordres possibles
  -A partir de la meilleure feuille, celle qui correspond au chemin le plus court,
   creer l'itineraire correspondant.
-Sinon
  (On utilise une heuritique pour obtenir un chemin plus court, mais pas necessairement optimal).
  -A partir de la station de depart, on se dirige vers la station intermediaire
   la plus proche. 
  -A partir de cette station, on se dirige encore vers la station intermediaire,
   la plus proche, jusqu'a parvenir a la station d'arrivee.
  (On obtient au fur et a mesure du parcours l'itineraire).
*/
char faireItineraireMultiOpt(BM_Reseau *br, BM_Ind_Requete *bq, BM_Itineraire *iti){
  BM_Pcc **pccs ;
  BM_Noeud *racine,  *mfeuil = NULL;
  char *visites, ok = 1 ;
  BM_Ind_Requete rtmp = *bq;
  supprimerStationsEgales(bq);
  pccs = initialiserPccs(br,bq);
  if (pccs == (BM_Pcc **)ERR_ALLOC)
    return ERR_ALLOC;
  if (verifierExistenceChemin(pccs[0],bq) == 0){ /*Si requete insatisfiable*/
    supprimerPccs(pccs, bq->nbInterm  + 1);
    iti->etape = NULL; /*Indiquer un itineraire non trouve*/
    iti->nbEtapes = -1;
    return 1;
  }
  /*Creation tableau de booleens des stations visitees, initialise a 0 pour non visites partout.*/
  visites = (char *)calloc(bq->nbInterm,sizeof(char));
  if (visites == NULL){  
    supprimerPccs(pccs, bq->nbInterm + 1);
    return ERR_ALLOC;
  }
  if (bq->nbInterm > BM_SEUIL_OPTM){ /*Si requete trop complexe avec creation d'arbre*/
    afficherChaine(L"\n[Itineraire proche de l'optimalite]\n\n");
    ok = (creerCheminSatisfaisant(br,pccs,bq,iti,visites) != ERR_ALLOC);
  }
  else{
    afficherChaine(L"\n[Itineraire optimal]\n\n");
    racine = creerNoeud(bq->indDepart,0,0); /*racine : station de depart, temps de 0 pour y parvenir*/
    ok = (racine != NULL);
    ok = ((ok) && (creerArbre(racine,pccs,visites,bq, &mfeuil) != ERR_ALLOC));
    rtmp.indDepart = mfeuil->numSta; /*Creation de la derniere partie de l'itineraire d'abord*/
    rtmp.indArrivee = bq->indArrivee;
    ok = ((ok) && (faireItineraireSimple(pccs[mfeuil->numStaTab],br, &rtmp, iti) != ERR_ALLOC));
    ok = ((ok) && (creerCheminOptimal(br,mfeuil,pccs,bq,iti) != ERR_ALLOC));
    libererArbre(racine);
  }
  supprimerPccs(pccs, bq->nbInterm + 1); 
  free(visites);
  if (!ok)
    return ERR_ALLOC;  
  return 1;
}
