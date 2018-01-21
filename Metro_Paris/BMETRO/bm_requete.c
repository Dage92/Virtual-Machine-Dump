/* **************************************************/
/*   Projet      : Balade dans le metro             */
/*   Date        : mai 2009                         */
/*   Auteur      : Climoo                           */
/*   Fichier     : bm_requete.c                     */
/*   Description : Module charge                    */
/*   d'initialiser la requete formulee par          */
/*   l'utilisateur, a partir des parametres         */
/*   passes au programme                            */
/*                                                  */
/* **************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "bm_reseau.h"
#include "bm_utiles.h"
#include "bm_requete.h"

#define VERSION "1.0"

/*Les prefixes des differents parametres possibles du programme*/

#define OPT_AID "-h"  /*Pour obtenir l'aide du jeu en ligne de commande*/
#define OPT_VER "-v"  /*Pour obtenir la version du programme*/
#define OPT_FIC "-f"  /*Pour specifier le fichier d'entree*/
#define OPT_DEP "-x"  /*Pour specifier la station de depart*/
#define OPT_ARR "-y"  /*Pour specifier la station d'arrivee*/
#define OPT_INT "-i"  /*Pour specifier une station intermediaire*/
#define OPT_OPM "-o"  /*Pour specifier l'optimisation du parcours*/
#define OPT_CHG "-c"  /*Pour specifier le temps pris lorsqu'il est necessaire de changer de ligne*/
#define OPT_DPL "-d"  /*Pour specifier le temps pris lors d'un trajet entre deux stations sur une meme ligne*/

/*Les erreurs pouvant survenir lors de l'analyse des parametres*/

#define MULTI_SPEC   "Multiple definition"   /*Un parametre a ete defini plusieurs fois*/
#define ARG_MANQUANT "Argument manquant"     /*Un parametre est manquant apres une option*/
#define OPT_INCONNU  "Option inconnue"      /*Une option qui est inconnue du programme, commencant par un '-'*/
#define ARG_INCONNU  "Argument inconnu"      /*Un parametre dont on ne connait pas la signification (ne commencant pas pas '-')*/
#define NON_ENTIER   "Entier non valide"     /*Un parametre attendu comme entier n'en etait pas un valide*/
#define NON_FIC      "Fichier non specifie"  /*Aucun fichier n'a ete dans la liste des arguments*/

/*Increment de la taille du tableau de stations intermediaire de la structure BM_Requete*/
#define BM_INC_INTER 1

/*Fonction qui affiche l'aide du programme sur la sortie standard*/
static void afficherAide(char *nomProg);

/*Fonction qui initialise une requete a vide (tous les champs a 0)*/
static void initialiserRequeteVide(BM_Requete *br);

/*Fonction qui permet de remplacer les parametres necessaires
  mais inconnus d'une requete*/
static void definirInconnuesRequete(BM_Requete *br);

/*Fonction qui permet d'ajouter une station intermediaire a la requete*/
static char ajouterStationIntermediaire(BM_Requete *br, char *nomStation);

static void afficherAide(char *nomProg){
  printf("%s : usage :\n",nomProg);
  printf(" [%s]         : affiche les options possibles\n",OPT_AID);
  printf(" [%s]         : affiche la version du programme\n",OPT_VER);
  printf(" [%s fichier] : effectue la resolution du fichier\n",OPT_FIC);
  printf(" [%s station] : specifier la station d'entree\n",OPT_DEP);
  printf(" [%s station] : specifier la station d'arrivee\n",OPT_ARR);
  printf(" [%s station] : specifier une station intermediaire\n",OPT_INT);
  printf(" [%s]         : demander un chemin optimal (si stations intermediares)\n",OPT_OPM);
  printf(" [%s temps]   : specifier le temps de deplacement entre deux stations (en secondes)\n",OPT_DPL);
  printf(" [%s temps]   : specifier le temps de changement entre deux lignes\n",OPT_CHG);
  return ;
}

static void afficherVersion(char *nomPrgm){
  printf("%s : version %s\n",nomPrgm, VERSION);
  printf("Mai 2009\n");
  printf("Les sources presentees sont libre de droits\n");
}


static void initialiserRequeteVide(BM_Requete *br){
  br->nomDepart[0] = '\0';
  br->nomArrivee[0] = '\0';
  br->nomInterm = NULL;
  br->nbInterm = 0;
  br->optim = 0;
  br->delais.tpsDepl = 0;
  br->delais.tpsChgt = 0;
}

static void definirInconnuesRequete(BM_Requete *br){
  if (br->nomDepart[0] == L'\0'){
    afficherChaine(L"Station de depart : ");
    lireChaine(br->nomDepart,BM_MAX_STR);
  }
  if (br->nomArrivee[0] == L'\0'){
    afficherChaine(L"Station d'arrivee : ");
    lireChaine(br->nomArrivee,BM_MAX_STR);
  }
  if (br->delais.tpsDepl == 0){
    afficherChaine(L"Temps de deplacement : ");
    lireEntier(&(br->delais.tpsDepl));
  }
  if (br->delais.tpsChgt == 0){
    afficherChaine(L"Temps de changement : ");
    lireEntier(&(br->delais.tpsChgt));
  }
  return;
}

static char ajouterStationIntermediaire(BM_Requete *br, char *nomStation){
  if ((br->nbInterm % BM_INC_INTER) == 0){
    br->nomInterm = (wchar_t (*)[BM_MAX_STR])realloc(br->nomInterm,(br->nbInterm+BM_INC_INTER)*sizeof(wchar_t)*BM_MAX_STR);
    if (br->nomInterm == NULL)
      return ERR_ALLOC;
  }
  mbstowcs(br->nomInterm[br->nbInterm],nomStation,BM_MAX_STR);
  br->nbInterm++;
  return 1;
}

static void afficherErreur(char *nomPrgm,char *opt, char *typ){
  wchar_t tmp[BM_MAX_STR];
  mbstowcs(tmp,nomPrgm,BM_MAX_STR);
  afficherChaine(tmp);
  afficherChaine(L" : ");
  mbstowcs(tmp,opt,BM_MAX_STR);
  afficherChaine(tmp);
  afficherChaine(L" : ");
  mbstowcs(tmp,typ,BM_MAX_STR);
  afficherChaine(tmp);
  putwchar(L'\n');  
}

char initialiserRequete(int argc, char **argv, BM_Requete *br, int *indNomFic){
  int i = 1;
  char cont = 1;               /*cont == 1 <=> continuer a parcourir les arguments*/
  char *fin;
  initialiserRequeteVide(br);
  *indNomFic = -1; /*L'indice dans argv du nom du fichier, pas encore connu*/
  while ((i < argc) && (cont)){
    if (strcmp(argv[i],OPT_AID) == 0){
      afficherAide(argv[0]);
      cont = 0;
    }
    else if (strcmp(argv[i],OPT_VER) == 0){
      afficherVersion(argv[0]);
      cont = 0;
    }
    else if (strcmp(argv[i],OPT_FIC) == 0){
      if (*indNomFic != -1){ /*Si le fichier a deja ete specifie*/
	afficherErreur(argv[0],OPT_FIC,MULTI_SPEC);
	cont = 0;
      }
      else if ((i + 1) >= argc){
	afficherErreur(argv[0],OPT_FIC,ARG_MANQUANT);
	cont = 0;
      }
      else{
	*indNomFic = i+1;
	i += 2;
      }
    }
    else if (strcmp(argv[i],OPT_DEP) == 0){
      if (br->nomDepart[0] != L'\0'){ /*Si la station de depart a deja ete specifiee*/
	afficherErreur(argv[0],OPT_DEP,MULTI_SPEC);
	cont = 0;
      }
      else if ((i + 1) >= argc){
	afficherErreur(argv[0],OPT_DEP,ARG_MANQUANT);
	cont = 0;
      }
      else{
	mbstowcs (br->nomDepart, argv[i+1],BM_MAX_STR);
	i += 2;
      }
    }
    else if (strcmp(argv[i],OPT_ARR) == 0){
      if (br->nomArrivee[0] != L'\0'){ /*Si la station de depart a deja ete specifiee*/
	afficherErreur(argv[0],OPT_ARR,MULTI_SPEC);
	cont = 0;
      }
      else if ((i + 1) >= argc){
	afficherErreur(argv[0],OPT_ARR,ARG_MANQUANT);
	cont = 0;
      }
      else{
	mbstowcs (br->nomArrivee, argv[i+1],BM_MAX_STR);
	/*	afficherChaine(br->nomArrivee);*/
	i += 2;
      }
    }
    else if (strcmp(argv[i],OPT_INT) == 0){
      if ((i + 1) >= argc){
	afficherErreur(argv[0],OPT_INT,ARG_MANQUANT);
	cont = 0;
      }
      else{
	if (ajouterStationIntermediaire(br,argv[i+1]) == ERR_ALLOC)
	  return ERR_ALLOC;
	i += 2;
      }
    }
    else if (strcmp(argv[i],OPT_OPM) == 0){
      if (br->optim == 1){ /*Si option d'optimisation deja specifiee*/
	afficherErreur(argv[0],OPT_OPM,MULTI_SPEC);
	cont = 0;
      }
      else{
	br->optim = 1;
	i++;
      }
    }
    else if (strcmp(argv[i],OPT_DPL) == 0){
      if (br->delais.tpsDepl != 0 ){ /*Si la station de depart a deja ete specifiee*/
	afficherErreur(argv[0],OPT_DPL,MULTI_SPEC);
	cont = 0;
      }
      else if ((i + 1) >= argc){
	afficherErreur(argv[0],OPT_DPL,ARG_MANQUANT);
	cont = 0;
      }
      else{
	br->delais.tpsDepl = (unsigned int)strtol(argv[i+1],&fin,10);
	if (*fin != '\0'){
	  afficherErreur(argv[0],OPT_DPL,NON_ENTIER);
	  cont = 0;
	}
	i += 2;
      }
    }
    else if (strcmp(argv[i],OPT_CHG) == 0){
      if (br->delais.tpsChgt != 0 ){ /*Si la station de depart a deja ete specifiee*/
	afficherErreur(argv[0],OPT_ARR,MULTI_SPEC);
	cont = 0;
      }
      else if ((i + 1) >= argc){
	afficherErreur(argv[0],OPT_ARR,ARG_MANQUANT);
	cont = 0;
      }
      else{
	br->delais.tpsChgt = (unsigned int)strtol(argv[i+1],&fin,10);
	if (*fin != '\0'){
	  afficherErreur(argv[0],OPT_CHG,NON_ENTIER);
	  cont = 0;
	}
	i += 2;
      }
    }
    else if (argv[i][0] == '-'){
      afficherErreur(argv[0],argv[i],OPT_INCONNU);
      cont = 0;
    }
    else{
      afficherErreur(argv[0],argv[i],ARG_INCONNU);
      cont = 0;
    }
  }
  if (cont ==0) /*Si la requete n'est pas valide (erreur ou affichage de l'aide ou de la version)*/
    supprimerRequete(br); /*Suppression eventuelle de la memoire utilisee par la requete*/
  else if (*indNomFic == -1){ /*Si le fichier n'a pas ete specifie*/
    afficherErreur(argv[0],OPT_FIC,NON_FIC);
    return 0;
  }
  else
    definirInconnuesRequete(br);
  return cont;
}

void supprimerRequete(BM_Requete *br){
  if (br->nomInterm != NULL)
    free(br->nomInterm);
  return ;
}
