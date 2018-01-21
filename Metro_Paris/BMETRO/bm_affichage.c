/* **************************************************/
/*   Projet      : Balade dans le metro             */
/*   Date        : mai 2009                         */
/*   Auteur      : Climoo                           */
/*   Fichier     : bm_affichage.h                   */
/*   Description : Module charge d'afficher les     */
/*                 informations  liees a un         */
/*		   itineraire                       */
/*                                                  */
/* **************************************************/

#include <stdio.h>
#include <wchar.h>
#include "bm_reseau.h"
#include "bm_utiles.h"
#include "bm_requete.h"
#include "bm_chemin.h"
#include "bm_itineraire.h"
#include "bm_affichage.h"

/*
  Affichage d'un itineraire: Tableau d'etapes.
  A chaque ligne correspond une etape du trajet
  4 colonnes : 
  Ligne : la ligne empruntee pour cette etape
  Direction : la direction du metro sur la ligne
  Arret : la station a laquelle il faut s'arreter
  Temps cumule : le temps cumule de chaque etape du trajet
*/

/*Caractere correspondant a une ligne horizontal*/
#define BM_CH_SEPH L'='

/*Caractere correspondant a une ligne verticale (colonne) */
#define BM_CH_SEPV L'|'

/*Macro pour afficher un separateur de de colonne*/
#define afficherChCol() (putwchar(BM_CH_SEPV))

/*Macro pour afficher un caractere de ligne*/
#define afficherChLig() (putwchar(BM_CH_SEPH))

/*Les donnees relatives au tableau affichage de l'itineraire*/
#define BM_ITI_TITRE L"[Itineraire]"   /*Titre du tableau*/
#define BM_ITI_COL 4                   /*Nombre de colonnes du tableau*/
#define BM_ITI_LIG L"Ligne"            /*Titre de la colonne indiquant les lignes*/
#define BM_ITI_DIR L"Direction"        /*Titre de la colonne indiquant les directions*/ 
#define BM_ITI_ARR L"Arret"            /*Titre de la colonne indiquant les arrets*/
#define BM_ITI_TPS L"Temps cumule"     /*Titre de la colonne indiquant le temps cumule*/

/*Les donnees relatives au tableau affichage d'une requete*/
#define BM_REQ_TITRE L"[Requete]"             /*Titre du tableau*/
#define BM_REQ_COL 2                          /*Nombre de colonnes du tableau*/
#define BM_REQ_LARG 34                        /*Largeur d'une colonne du tableau*/
#define BM_REQ_DEP L"Depart"                  /*Titre de la ligne indiquant la station de depart*/
#define BM_REQ_ARR L"Arrivee"                 /*Titre de la ligne indiquant la station d'arrivee*/
#define BM_REQ_INT L"Intermediaires"          /*Titre de la ligne indiquant les stations intermediaires*/
#define BM_REQ_CHG L"Temps de changement"     /*Titre de la ligne indiquant le temps de changement moyen*/
#define BM_REQ_DPL L"Temps de entre stations" /*Titre de la ligne indiquant le temps de changement moyen*/
#define BM_REQ_OPM L"Optimisation"            /*Titre de la ligne indiquant la demande d'optimisation du trajet*/

/*Les donnees relatives au tableau affichage des statistiques d'un itineraire*/
#define BM_STAT_TITRE L"[Statistiques Itineraires]"  /*Titre du tableau*/
#define BM_STAT_COL 2                                /*Nombre de colonnes du tableau*/
#define BM_STAT_LARG 34                              /*Largeur d'une colonne du tableau*/
#define BM_STAT_TT1 L"Temps total"                   /*Titre de la ligne indiquant le temps total*/
#define BM_STAT_TT2 L"Temps total (secondes)"        /*Titre de la ligne indiquant le temps total en secondes*/
#define BM_STAT_NBC L"Nombre de correspondances"     /*Titre de la ligne indiquant le nombre de correspondances*/
#define BM_STAT_NBS L"Nombre de stations visitees"   /*Titre de la ligne indiquant le nombre de stations vivitees*/

/*La chaine correspondant a un itineraire inexistant*/
#define BM_NON_ITI L"Pas d'itineraire trouve par le metro\n"

/*La chaine correspondant a un itineraire vide : station de depart et d'arrivee identique*/
#define BM_ITI_VIDE L"Pas besoin de se deplacer : station d'arrivee deja atteinte\n"

/*Fonction qui affiche une ligne horizontale tableau de longueur nbchr*/
static void afficherLigneHorizontale(int nbchr);

/*Fonction qui recherche la plus grande longueur des directions d'un itineraire*/
static int maxLenDirection(BM_Itineraire *iti, BM_Reseau *br);

/*Fonction qui recherche la plus grande longueur des arrets d'un itineraire*/
static int maxLenArret(BM_Itineraire *iti, BM_Reseau *br);

/*Fonction qui recherche la plus grande longueur des lignes d'un itineraire*/
static int maxLenLigne(BM_Itineraire *iti, BM_Reseau *br);

/*Fonction qui affiche un titre de tableau centre*/
static void afficherTitreCentre(wchar_t *titre, int larg);

/*Fonction qui affiche l'ente du tableau de l'itineraire*/
static void afficherEnteteItineraire(int, int, int);

/*Fonction qui affiche une etape, ligne du tableau de l'itineraire affiche*/
static void afficherEtape(BM_Etape *etape, BM_Reseau *br, int lgligne, int lgdir, int lgarret, unsigned int tpsCumul);

/*Fonction qui affiche les differentes statiques d'un itineraire*/
static void afficherStatsItineraire(BM_Itineraire *);

static void afficherLigneHorizontale(int nbchr){
  int i = 0;
  while (i < nbchr){
    afficherChLig();
    i++;
  }
}

static int maxLenDirection(BM_Itineraire *iti, BM_Reseau *br){
  BM_refLigne ref;
  unsigned int i = 0;
  int max = (sizeof(BM_ITI_DIR)/sizeof(wchar_t));
  int len;
  while (i < iti->nbEtapes){
    ref = (iti->etape[i]).reflig;
    if (getDirRefLig(ref) == 1) /*si vers la station de depart*/
      len = wcslen(br->station[br->ligne[getNumRefLig(ref)].indDepart].nom); 
    else /*si vers le terminus*/
      len = wcslen(br->station[br->ligne[getNumRefLig(ref)].indTerminus].nom);
    if (len > max)
      max = len ;
    i++;
  }
  return max + 2;
}

static int maxLenArret(BM_Itineraire *iti, BM_Reseau *br){
  unsigned int i = 0;
  int max = (sizeof(BM_ITI_ARR)/sizeof(wchar_t));
  int len;
  while (i < iti->nbEtapes){
      len = wcslen(br->station[ iti->etape[i].indStaArret].nom);
    if (len > max)
      max = len ;
    i++;
  }
  return max + 2;
}

static int maxLenLigne(BM_Itineraire *iti, BM_Reseau *br){
  BM_refLigne ref;
  unsigned int i = 0;
  int max = (sizeof(BM_ITI_LIG)/sizeof(wchar_t));
  int len;
  while (i < iti->nbEtapes){
    ref = iti->etape[i].reflig;
    len = wcslen(br->ligne[getNumRefLig(ref)].nom);
    if (len > max)
      max = len ;
    i++;
  }
  return max + 2;
}

static void afficherTitreCentre(wchar_t *titre, int larg){
  int lgtitre = wcslen(titre);
  int mil = (larg - lgtitre) /2;
  afficherLigneHorizontale(mil);
  afficherChaine(titre);
  if (2*mil + lgtitre  == larg)
    afficherLigneHorizontale(mil);
  else
    afficherLigneHorizontale(mil+1);
  putwchar(L'\n');
}

static void afficherEnteteItineraire(int lgligne, int lgdir, int lgarret){
  int longueurLigne = lgligne + lgdir + lgarret + (sizeof(BM_ITI_TPS)/sizeof(wchar_t)) + BM_ITI_COL +1;
  afficherTitreCentre(BM_ITI_TITRE,longueurLigne);
  /*Affichage de chaque colonne*/
  afficherChCol();
  afficherChaineEsp(BM_ITI_LIG,lgligne);
  afficherChCol();
  afficherChaineEsp(BM_ITI_DIR,lgdir);
  afficherChCol();
  afficherChaineEsp(BM_ITI_ARR,lgarret);
  afficherChCol();
  afficherChaineEsp(BM_ITI_TPS,(sizeof(BM_ITI_TPS)/sizeof(wchar_t)));
  afficherChCol();
  putwchar(L'\n');
  /*Affichage d'une ligne de separation entre en-tete et differentes etapes*/
  afficherLigneHorizontale(longueurLigne);
  putwchar(L'\n');
  return ;
}

static void afficherEtape(BM_Etape *etape, BM_Reseau *br, int lgligne, int lgdir, int lgarret, unsigned int tpsCumul){
  BM_refLigne ref = etape->reflig;
  afficherChCol();
  afficherChaineEsp(br->ligne[getNumRefLig(ref)].nom,lgligne);
  afficherChCol();
  if (getDirRefLig(ref) == 1) /*si vers la station de depart*/
    afficherChaineEsp(br->station[br->ligne[getNumRefLig(ref)].indDepart].nom,lgdir);
  else /*si vers le terminus*/
    afficherChaineEsp(br->station[br->ligne[getNumRefLig(ref)].indTerminus].nom,lgdir);
  afficherChCol();
  afficherChaineEsp(br->station[etape->indStaArret].nom,lgarret);
  afficherChCol();
  afficherHeureEsp((tpsCumul + etape->tps),(sizeof(BM_ITI_TPS)/sizeof(wchar_t)));
  afficherChCol();
  putwchar(L'\n');
}

static void afficherStatsItineraire(BM_Itineraire *iti){
  putwchar(L'\n');
  afficherTitreCentre(BM_STAT_TITRE,BM_STAT_LARG*(BM_STAT_COL) + BM_STAT_COL + 1);
  afficherChCol();
  afficherChaineEsp(BM_STAT_TT1,BM_STAT_LARG);
  afficherChCol();
  afficherTempsEsp(iti->tpsTotal,BM_STAT_LARG);
  afficherChCol();
  putwchar(L'\n');
  afficherChCol();
  afficherChaineEsp(BM_STAT_TT2,BM_STAT_LARG);
  afficherChCol();
  afficherEntierEsp(iti->tpsTotal,BM_STAT_LARG);
  afficherChCol();
  putwchar(L'\n');
  afficherChCol();
  afficherChaineEsp(BM_STAT_NBC,BM_STAT_LARG);
  afficherChCol();
  afficherEntierEsp(iti->nbEtapes,BM_STAT_LARG);
  afficherChCol();
  putwchar(L'\n');
  afficherChCol();
  afficherChaineEsp(BM_STAT_NBS,BM_STAT_LARG);
  afficherChCol();
  afficherEntierEsp(iti->nbStaTotal,BM_STAT_LARG);
  afficherChCol();
  putwchar(L'\n');
  afficherLigneHorizontale(BM_STAT_LARG*(BM_STAT_COL) + BM_STAT_COL + 1);
  putwchar(L'\n');
  return ;
}

void afficherItineraire(BM_Itineraire *iti, BM_Reseau *br){
  int lgligne, lgdir, lgarret;
  unsigned int i = iti->nbEtapes - 1 ;
  unsigned int tpsCumul = 0;
  if (iti->nbEtapes == (unsigned int)-1){ 
    afficherChaine(BM_NON_ITI);
    return;
  }
  else if (iti->nbEtapes == 0){
    afficherChaine(BM_ITI_VIDE);
    return;
  }
  putwchar(L'\n');
  lgligne = maxLenLigne(iti,br);
  lgdir = maxLenDirection(iti,br);
  lgarret = maxLenArret(iti,br);
  afficherEnteteItineraire(lgligne,lgdir,lgarret);
  while(i != (unsigned int)-1) {
    afficherEtape(&(iti->etape[i]),br,lgligne,lgdir,lgarret,tpsCumul);
    tpsCumul += iti->etape[i].tps;
    i --;
  }
  afficherLigneHorizontale((lgligne + lgdir + lgarret + (sizeof(BM_ITI_TPS)/sizeof(wchar_t)) + BM_ITI_COL + 1));
  putwchar(L'\n');
  afficherStatsItineraire(iti);
  return ;
}

void afficherRequete(BM_Requete *req){
  unsigned int i = 1;
  putwchar(L'\n');
  afficherTitreCentre(BM_REQ_TITRE,BM_REQ_LARG*(BM_REQ_COL) + BM_REQ_COL + 1);
  afficherChCol();
  afficherChaineEsp(BM_REQ_DEP,BM_REQ_LARG);
  afficherChCol();
  afficherChaineEsp(req->nomDepart,BM_REQ_LARG);
  afficherChCol();
  putwchar(L'\n');
  afficherChCol();
  afficherChaineEsp(BM_REQ_ARR,BM_REQ_LARG);
  afficherChCol();
  afficherChaineEsp(req->nomArrivee,BM_REQ_LARG);
  afficherChCol();
  putwchar(L'\n');
  if (req->nbInterm > 0){
    afficherChCol();
    afficherChaineEsp(BM_REQ_INT,BM_REQ_LARG);
    afficherChCol();
    afficherChaineEsp(req->nomInterm[0],BM_REQ_LARG);
    afficherChCol();
    putwchar(L'\n');
    while (i < req->nbInterm){
      afficherChCol();
      afficherChaineEsp(L"",BM_REQ_LARG);
      afficherChCol();
      afficherChaineEsp(req->nomInterm[i],BM_REQ_LARG);
      afficherChCol();
      putwchar(L'\n');
      i++;
    }
  }
  afficherChCol();
  afficherChaineEsp(BM_REQ_CHG,BM_REQ_LARG);
  afficherChCol();
  afficherTempsEsp(req->delais.tpsChgt,BM_REQ_LARG);
  afficherChCol();
  putwchar(L'\n');
  afficherChCol();
  afficherChaineEsp(BM_REQ_DPL,BM_REQ_LARG);
  afficherChCol();
  afficherTempsEsp(req->delais.tpsDepl,BM_REQ_LARG);
  afficherChCol();
  putwchar(L'\n');
  afficherChCol();
  afficherChaineEsp(BM_REQ_OPM,BM_REQ_LARG);
  afficherChCol();
  if (req->optim)
    afficherChaineEsp(L"Oui",BM_REQ_LARG);
  else
    afficherChaineEsp(L"Non",BM_REQ_LARG);
  afficherChCol();
  putwchar(L'\n');
  afficherLigneHorizontale(BM_REQ_LARG*(BM_REQ_COL) + BM_REQ_COL + 1);
  putwchar(L'\n');
  return ;
}
