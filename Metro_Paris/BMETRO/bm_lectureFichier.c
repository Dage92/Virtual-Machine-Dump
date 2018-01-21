/* **************************************************/
/*   Projet      : Balade dans le metro             */
/*   Date        : mai 2009                         */
/*   Auteur      : Climoo                           */
/*   Fichier     : bm_lectureFichier.c              */
/*   Description : Module charge                    */
/*   de lire un fichier de donnees sources          */
/*   et d'initialiser les principales structures    */
/*   de donnees                                     */
/*                                                  */
/* **************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include "bm_reseau.h"
#include "bm_utiles.h"
#include "bm_lectureFichier.h"

/*Le Byte Order Mask d'un fichier UTF-8*/
#define BOM_UTF8 "\xEF\xBB\xBF"

/*Le nombre de caracteres qui composent la BOM UTF-8*/
#define TAILLE_BOM (sizeof(BOM_UTF8)/sizeof(char))

/*La taille maximale d'un caractere UTF-8*/
#define MAX_TAILLE_CAR_UTF8 4

/*La taille maximale (valeur arbitraire) d'une ligne du fichier*/
#define MAX_LIG 128

/*Caractere de retour chariot*/
#define CR L'\x0D'

/*Caractere delimitant le debut du nom d'une ligne de metro*/
#define CAR_DEB_LIGM L'['

/*Caractere delimitant la fin du nom d'une ligne de metro*/
#define CAR_FIN_LIGM L']'

/*Chaines de caracteres correspondants aux erreurs possibles qui peuvent survenir
  lors de la lecture du fichier*/
#define ERR_OUVERTURE_FIC L"Impossible d'ouvrir le fichier en lecture\n"
#define ERR_LECTURE_FIC   L"Impossible de lire le fichier\n"
#define ERR_BOM           L"Byte Order Mask du fichier incorrect : necessite un fichier encode UTF-8\n"
#define ERR_LECTURE_LIG   L"Ligne non lue, trop longue ou caractere non Unicode detectee\n"
#define ERR_DEFINITION_LIG L"Nom de ligne de metro non definie\n"

/*La structure de fichier bufferise (contenu du fichier mis dans un tableau)*/
typedef struct{
  char *tab;        /*Tableau de caracteres correspondant au contenu du fichier*/
  long int taille;  /*Taille du tableau de caracteres*/
  long int pos;     /*Position de lecture actuelle*/
}FichierBuffer;

/*Fonction qui affiche une erreur sur la sortie standard. 
 Si numLig est plus grand strictement que 0, on affiche
 le numero de ligne avant l'erreur (numero de la ligne ou l'erreur a eu lieu).
 Sinon, on affiche pas le numero de ligne*/
static void afficherErreur(unsigned int numLig, wchar_t *chErr);

/*Fonction qui permet d'initialiser une structure de fichier bufferisee
  a partir d'un fichier ouverture en lecture (lecture binaire).
  Retourne 1 si succes, 0 sinon*/
static char initialiserFichierBuffer(FILE *f, FichierBuffer *fb);

/*Fonction qui retourne le prochain caractere (char) d'un fichier
  bufferise. Retourne 1 si succes, 0 si fin de fichier atteinte*/
static char prochainCarac(FichierBuffer *fb, char *carac);

/*Fonction qui retourne la taille d'un caractere Unicode encode
  en UTF-8, le nombre d'octet sur lequel il est encode.
  Retourne 0 si carac1 n'est pas valide.*/
static int getTailleCarac(char carac1);

/*Fonction qui retourne 1 si wc est un caractere d'espacement 
 (comme LF, CR, BS), 0 sinon */
static char estEspace(wchar_t wc);

/*Fonction qui retourne le prochain caractere Unicode d'un fichier
  bufferise. Retourne 1 si un caractere a bien pu etre lu,
  0 si fin de fichier atteinte, -1 en cas d'erreur.*/
static char prochainCaracUnicode(FichierBuffer *fb,wchar_t *wc);

/*Identique a prochainCaracUnicode, mais le caractere est le prochain
  different d'un espace. Meme valeurs de retour que prochainCaracUnicode.*/
static char prochainCaracUnicodeNonEsp(FichierBuffer *fb, wchar_t *wc);

/*Fonction qui supprime les espaces en fin d'une chaine de caractere.
  Retourne un pointeur vers le dernier caractere de la chaine, qui n'est
  pas un espace.*/
static wchar_t *supprimerEspacesFin(wchar_t *chfin);

/*Fonction qui permet de lire un nom de ligne de metro a partir d'une ligne du fichier
  La station est automatiquee ajoutee au reseau si necessaire.
  Retourne l'indice de la ligne en cas de succes, ERR_ALLOC si erreur
  d'allocation dynamique*/
static unsigned int lireNomLigne(BM_Reseau *bm,wchar_t *ligf);

/*Fonction qui permet de lire un nom de ligne de metro a partir d'une ligne du fichier*
  La ligne de metro est automatiquee ajoutee au reseau si necessaire.
  Retourne l'indice de la station en cas de succes, ERR_ALLOC si erreur
  d'allocation dynamique*/
static unsigned int lireNomStation(BM_Reseau *bm,wchar_t *ligf);

/*Macro qui definit la station de metro comme station de depart de la ligne de numero ligm*/
#define definirStaDepart(ligm,indSta) ( (ligm).indDepart = (indSta) )

/*Macro qui definit la station de metro comme station de terminus de la ligne de numero ligm*/
#define definirStaTerminus(ligm,indSta) ((ligm).indTerminus = (indSta) )

/*Fonction qui lit la prochaine ligne du fichier. Les lignes vides
  sont automatiquement ignorees. Le premier caractere d'une ligne
  lue est necessairement different d'un caractere d'espacement*/
static char lireLigneFichier(FichierBuffer *fb, wchar_t *lig);


static void afficherErreur(unsigned int numLig, wchar_t *chErr){
  if (numLig != 0){ /*Si erreur liee a une ligne*/
    afficherChaine(L"Ligne ");
    afficherEntier(numLig);
  }
  afficherChaine(chErr);
}

static char initialiserFichierBuffer(FILE *f, FichierBuffer *fb){
  if (fseek(f,0,SEEK_END) != 0)
    return 0;
  fb->taille = (ftell(f)); /*Taille du fichier en octet*/
  if (fb->taille == -1)
    return 0;
  rewind(f); /*Revenir au debut du fichier*/
  fb->tab = (char*)malloc((fb->taille+1)*sizeof(char));
  if (fb->tab == NULL)
    return -1;
  if (fread (fb->tab,sizeof(char),fb->taille,f) != (size_t)fb->taille){
    free(fb->tab);
    return 0;
  }
  fb->pos = 0;
  return 1;
}

static char prochainCarac(FichierBuffer *fb, char *carac){
  if (fb->pos >= fb->taille)
    return 0;
  *carac = fb->tab[fb->pos];
  fb->pos ++;
  return 1;
}

/*
  Encodage des caracteres UTF-8 :
  0xxxxxxx 	                        1 octet codant 1 a 7 bits
  110xxxxx 10xxxxxx                	2 octets codant 8 a 11 bits
  1110xxxx 10xxxxxx 10xxxxxx      	3 octets codant 12 a 16 bits
  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx 	4 octets codant 17 a 21 bits*/


/*Masques pour determiner la taille du code du caractere*/
#define MASQUE_1 128      /*1000 0000 : caractere encode sur 1 octet*/
#define MASQUE_2 32       /*0010 0000 : caractere encode sur 2 octets*/
#define MASQUE_3 16       /*0001 0000 : caractere encode sur 3 octets*/
#define MASQUE_4 8        /*0000 1000 : caractere encode sur 4 octets*/

/*Masques pour recuperer les valeurs du caractere*/
#define MASQUE_C1 127  /*0111 1111* : valeur d'un caracetre encode sur 1 octet*/
#define MASQUE_C2 31   /*0001 1111* : valeur d'un caracetre encode sur 2 octets (1er) */
#define MASQUE_C3 15   /*0000 1111* : valeur d'un caracetre encode sur 3 octets (1er) */
#define MASQUE_C4 7    /*0000 0111* : valeur d'un caractere encode sur 4 octets (1er) */
#define MASQUE_C  63   /*0011 1111* : valeur d'un caractere encode sur 2,3 ou 4 octets (2em, 3em ou 4em) */  

static int getTailleCarac(char carac1){
  if ((carac1 & MASQUE_1) == 0)
    return 1;
  if ((carac1 & MASQUE_2) == 0)
    return 2;
  if ((carac1 & MASQUE_3) == 0)
    return 3;
  if ((carac1 & MASQUE_4) == 0)
    return 4;
  return 0;
}

static char prochainCaracUnicode(FichierBuffer *fb,wchar_t *wc){
  int tcarac;
  char carac[MAX_TAILLE_CAR_UTF8] = {0,0,0,0};
  *wc = (wchar_t)0;
  if (prochainCarac(fb,carac) == 0) /*Lecture du premier octet, qui determine la taille du caractere encode*/
    return 0; /*retour de fin du fichier*/
  tcarac = getTailleCarac(carac[0]);
  if (tcarac == 0) /*Si caractere non UTF8 detecte*/
    return -1;
  switch(tcarac){
  case 1:
    *wc = carac[0] & MASQUE_C1; 
    break;
  case 2:
    prochainCarac(fb,carac + 1);
    *wc = (carac[0] & MASQUE_C2) << 6;
    *wc = *wc | (carac[1] & MASQUE_C);
    break;
  case 3:
    prochainCarac(fb,carac + 1);
    prochainCarac(fb,carac + 2);
    *wc = (carac[0] & MASQUE_C3) << 13;
    *wc = *wc | (carac[1] & MASQUE_C << 8);
    *wc = *wc | (carac[2] & MASQUE_C );
    
    break;
  case 4:
    prochainCarac(fb,carac + 1);
    prochainCarac(fb,carac + 2);
    prochainCarac(fb,carac + 3);
    *wc = (carac[0] & MASQUE_C) << 20;
    *wc = *wc | (carac[1] & MASQUE_C) << 16;
    *wc = *wc | (carac[2] & MASQUE_C) << 8;
    *wc = *wc | (carac[3] & MASQUE_C);
    break;
  }
  return 1;
}

static char estEspace(wchar_t wc){
  return ((iswspace(wc)) || (wc == CR));
}

static char prochainCaracUnicodeNonEsp(FichierBuffer *fb, wchar_t *wc){
  char cont;
  cont  = prochainCaracUnicode(fb,wc);
  while ((cont == 1) && (estEspace(*wc))){
    cont  = prochainCaracUnicode(fb,wc);
  }	
  if (cont == 0) 
    return 0;
  if (cont == -1)
    return -1;
  return 1;
}

static wchar_t *supprimerEspacesFin(wchar_t *chfin){
  while(estEspace(*chfin)){
    chfin--;
  }
  return chfin;
}


static char lireLigneFichier(FichierBuffer *fb, wchar_t *lig){
  char ok, finLig, finFic ;
  wchar_t *ptFin;
  int i = 1;
  ok = prochainCaracUnicodeNonEsp(fb,lig);
  if (ok == 0){
    return 0;
  }
  if (ok == -1)
    return -1;
  do{
    ok = (prochainCaracUnicode(fb, lig + i));
    if (ok == -1) /*Si erreur de lecture*/
      return -1;
    finFic = (ok == 0);
    finLig = (lig[i] == L'\n');
    if (!finLig)
      i++;
  } while ((i < MAX_LIG) && (!finFic) && (!finLig));
  if (i >= MAX_LIG)
    return -1; /*Si ligne trop longue*/
  ptFin = supprimerEspacesFin(lig + i);
  *(ptFin +1) = L'\0'; /*Caractere de fin de ligne*/
  if (finFic)
    return 0;
  return 1; 
}

static char lireBOM(FichierBuffer *fb){
  char bom[TAILLE_BOM];
  unsigned int i;
  char ok = 1;
  bom[TAILLE_BOM-1] = '\0';
  for (i = 0; ((i < TAILLE_BOM -1) && ok); i++)
    ok = prochainCarac (fb,bom+i);
  if (!ok)
    return 0;
  return (strcmp(bom,BOM_UTF8) == 0);
}

static void supprimerFichierBuffer(FichierBuffer *fb){
  free(fb->tab);
  fb->pos = 0;
  fb->taille = 0;
}


static unsigned int lireNomLigne(BM_Reseau *bm,wchar_t *ligf){
  wchar_t *lt = ligf;
  wchar_t *ptFin;
  wchar_t tmp[BM_MAX_STR]; /*contient le nom de la ligne lue*/
  unsigned int i = 0;
  lt ++; /*On passe le premier caractere, qui vaut CAR_DEB_LIGM*/
  while(estEspace(*lt)) /*On passe les eventuels espaces au debut du nom de la ligne*/
    lt++;
  if ((*lt == CAR_FIN_LIGM) || (*lt == '\0')) /*Si le nom de ligne n'existe pas*/
    return -1;
  while ((i < BM_MAX_STR) && (lt[i] != '\0') && (lt[i] != CAR_FIN_LIGM)){
    tmp[i] = lt[i]; /*Recopie de la ligne dans la chaine temporaire*/
    i++;
  }
  if (lt[i] == '\0'){/*Chaine non terminee par CAR_FIN_LIGM*/
    return -1;
  }
  if (i == BM_MAX_STR)   /*Si un nom de ligne est trop grand*/
    i-=2;                /*On raccourcit la chaine d'un caractere, le reste est ignore*/
  else
    i--;
  ptFin = supprimerEspacesFin(tmp + i);/*Suppression des espaces en fin de chaine*/
  *(ptFin+1) = L'\0';         /*Fin de chaine positionnee*/
  i = rechercherLigne(bm,tmp);
  if (i == (unsigned int)-1)  /*Si la ligne de metro n'existe pas dans le reseau*/
    i = ajouterLigne(bm,tmp);
  return i;
}

static unsigned int lireNomStation(BM_Reseau *bm,wchar_t *ligf){
  wchar_t *lt = ligf;
  wchar_t tmp[BM_MAX_STR];
  unsigned int i = 0;
  while ((i < BM_MAX_STR) && (lt[i] != '\0')){
    tmp[i] = lt[i]; /*Recopie de la ligne dans la chaine temporaire*/
    i++;
  }
  if (i == BM_MAX_STR)   /*Si un nom de station est trop grand*/
    i --;              /*On raccourcit la chaine, le reste est ignore*/
  tmp[i] = '\0';         /*Fin de chaine positionnee*/	
  i = rechercherStation(bm,tmp);
  if (i == (unsigned int)-1) /*Si la station n'existe pas dans le reseau*/
    i = ajouterStation(bm,tmp);
  return i;
}



char lireFichier(char *nomFic, BM_Reseau *metro){
  FILE *fd = fopen(nomFic,"rb"); /*Ouverture du fichier en binaire*/
  FichierBuffer fb;
  wchar_t lig[MAX_LIG];
  char finFic = 0, ok = 1, retourLec, errAlloc = 0;
  unsigned int cptLig = 0 ; /*numero courant de la ligne*/
  unsigned int numStationPrec = -1, numStationSucc = -1, numLigne = -1;
  initialiserReseau(metro);
  if (fd == NULL){ /*Erreur d'ouverture du fichier*/
    afficherErreur(0,ERR_OUVERTURE_FIC);
    return 0;
  }
  if (!initialiserFichierBuffer(fd,&fb)){
    afficherErreur(0,ERR_LECTURE_FIC);
    fclose(fd);
    return 0;
  }
  fclose(fd);
  if (!lireBOM(&fb)){
    afficherErreur(0,ERR_BOM);
    return 0;
  }
  while ((!finFic) && (ok) && (!errAlloc)){
    retourLec = (lireLigneFichier(&fb,lig));
    cptLig ++ ; 
    finFic = (retourLec == 0); /*finFic recoit 1 si on est parvenu a la fin du fichier*/
    if (retourLec == -1){
      afficherErreur(cptLig,ERR_LECTURE_LIG); 
      ok = 0;
    }  /*ok recoit 0 si erreur lors lecture d'une ligne*/
    if ((ok) && (*lig != L'\0')){ /*Si pas d'erreur et si ligne non vide (possible si ligne vide en fin de fichier)*/
      if (*lig == CAR_DEB_LIGM){ /*Si on a rencontre le debut d'une definition de ligne*/
	if (numLigne != (unsigned int)-1)     /*Si ce n'est pas la premiere, il faut definir le terminus de la precedente*/
	  definirStaTerminus(metro->ligne[numLigne],numStationSucc);
	numLigne = lireNomLigne(metro,lig);
	errAlloc = (numLigne == (unsigned int)ERR_ALLOC);
	numStationPrec = -1;/*Pas de station precedente*/
      }
      else{
	if (numLigne == (unsigned int)-1){ /*Si la ligne n'est pas encore defini, et qu'on tente de lire une station*/
	  afficherErreur(cptLig,ERR_DEFINITION_LIG);/*Erreur definition de ligne manquante*/
	  ok = 0;
	}
	else {
	  if (numStationPrec == (unsigned int) -1){ /*S'il s'agit de la premiere station de la ligne*/
	    numStationPrec = lireNomStation(metro,lig);
	    errAlloc = (numLigne == (unsigned int)ERR_ALLOC);
	    definirStaDepart(metro->ligne[numLigne],numStationPrec);
	  }
	  else{ /*Si c'est une station quelconque de la ligne (pas la 1ere)*/
	    numStationSucc = lireNomStation(metro,lig);
	    errAlloc = (numStationSucc == (unsigned int)ERR_ALLOC);
	    errAlloc = (errAlloc || (ajouterTransition(metro,numStationPrec,numStationSucc,numLigne) == ERR_ALLOC));
	    numStationPrec = numStationSucc;
	  }
	}
      }
    }
  }
  supprimerFichierBuffer(&fb);
  if (!ok){ /*Si erreur de lecture du fichier*/
    supprimerReseau(metro);
    return 0;
  }
  if (errAlloc){ /*Si erreur d'allocation de memoire*/
    supprimerReseau(metro);
    return ERR_ALLOC;
  }
  return 1;
}
