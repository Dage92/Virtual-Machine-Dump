/* **************************************************/
/*   Projet      : Balade dans le metro             */
/*   Date        : mai 2009                         */
/*   Auteur      : Climoo                           */
/*   Fichier     : bm_utiles.h                      */
/*   Description : Module contenant                 */
/*   plusieurs fonctions de bases utiles            */ 
/*   dans diverses modules                          */
/*                                                  */
/* **************************************************/

#include <locale.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include "bm_carac.h"
#include "bm_reseau.h"
#include "bm_utiles.h"


/*La base dans laquelle sont lues les entiers*/
#define BASE_ENTIER 10

/*Les differents chiffres utilises*/
#define WCH_CHIFFRES L"0123456789"

/*Fonction qui vide le buffer de l'entree standard*/
static void viderStdin();

/*Fonction qui donne l'equivalent norme d'un caractere,
  en minuscule, et sans accent. Si le caractere
  n'est pas connu, un caractere identique est renvoye*/
static wchar_t donnerEquivalent(wchar_t carac);

/*Fonction qui normalise une chaine ch2, remplace 
  chaque caractere de ch2 par son caractere norme.
  Resultat dans ch1 */
static void normaliserChaine(wchar_t *ch1, wchar_t *ch2);

static void viderStdin()
{
  wint_t c;
  while ((c = getwchar() != '\n') && (c != WEOF));
}

static wchar_t donnerEquivalent(wchar_t carac){
  if (estAAccent(carac))
    return L'a';
  if (estIAccent(carac))
    return L'i';
  if (estUAccent(carac))
    return L'u';
  if (estEAccent(carac))
    return L'e';
  if (estOAccent(carac))
    return L'o';
  if (estCCedille(carac))
    return L'c';
  if (estSeparateur(carac))
    return L' ';
  return towlower(carac);
}

static void normaliserChaine(wchar_t *ch1, wchar_t *ch2){
  wchar_t *tch1 = ch1, *tch2 = ch2;
  while( *tch2 != L'\0'){
    *tch1 = donnerEquivalent(*tch2);
    tch1 ++;
    tch2 ++;
  }
  *tch1 = L'\0';
}

char comparerNoms(wchar_t *nom1, wchar_t *nom2){
  wchar_t tmp1[BM_MAX_STR];
  wchar_t tmp2[BM_MAX_STR];
  normaliserChaine(tmp1,nom1);
  normaliserChaine(tmp2,nom2);
  return (wcscmp(tmp1,tmp2) == 0 ); /*Comparaison des deux chaines normalisees*/
}

char lireChaine(wchar_t *nom, int taille){
  wchar_t *ps = nom;
  if (fgetws(nom,taille,stdin) == NULL)
    return 1;

  while(*ps !=  L'\n' && *ps != L'\0')
    ps++;
  if (*ps == L'\0'){ /*Saisi trop longue*/
    viderStdin(); 
    return 1;
  }    
  *ps = L'\0'; /*Suppression du retour chariot stockee par fgetws*/
  return 0;
}

void afficherChaineEsp(wchar_t *s, int lgmax){
  int i = 0;
  wchar_t *st = s;
  while ((i < lgmax) && (*st != L'\0')){
    putwchar(*st);
    st++;
    i++;
  }
  while (i < lgmax){ /*Affichage des espaces restants*/
    putwchar(L' ');
    i ++;
  }
}

char lireEntier(unsigned int *n){
  wchar_t tmp[BM_MAX_STR];
  char tmpn[BM_MAX_STR*sizeof(wchar_t)];
  char *fin;
  long int lu;
  do{
    lireChaine(tmp,BM_MAX_STR);
    wcstombs(tmpn,tmp,BM_MAX_STR*sizeof(wchar_t)-1);
    lu = strtol(tmpn,&fin,BASE_ENTIER);
    if ((lu <= 0) || (*fin != '\0')) /*Si erreur de lecture d'un entier ou si entier negatif*/
      fputws(L"Entier non reconnu. Veuillez recommencez.\n",stdout);
    else
      *n = (unsigned int)lu;
  }
  while((*fin != '\0') || (lu <= 0));
  return 0;
}

void afficherEntier(unsigned int n){
  wchar_t chiffre[] = WCH_CHIFFRES;
  wchar_t ch[BM_MAX_STR];
  int i = 0;
  if (n == 0){
    putwchar(L'0');
    putwchar(L' ');
    return ;
  }
  while(n > 0){
    ch[i++] = chiffre[n % BASE_ENTIER];
    n = (n / BASE_ENTIER);
  }
  i--;
  while(i >=0){
    putwchar(ch[i--]);
  }
  putwchar(L' ');
}

void afficherEntierEsp(unsigned int n, int lgmax){
  wchar_t chiffre[] = WCH_CHIFFRES;
  wchar_t ch[BM_MAX_STR];
  int i = 0;
  int nb = 0;
  if (n == 0){
    putwchar(L'0');
    nb++;
  }
  while(n > 0){
    ch[i++] = chiffre[n % BASE_ENTIER];
    n = (n / BASE_ENTIER);
  }
  i--;
  while(i >=0){
    putwchar(ch[i--]);
    nb++;
  }
  while (nb < lgmax){
    putwchar(L' ');
    nb ++;
  }
}

/*affichage toujours de taille 2*/
static void afficherEntierHeure(unsigned int nb, wchar_t esp){
  if (nb<10){
    putwchar(esp);
    afficherEntierEsp(nb,1);
  }
  else
    afficherEntierEsp(nb,2);
}

void afficherHeureEsp(unsigned int nbsec,int lgmax){
  unsigned int nbheure, nbmin ;
  int nb = 8;
  nbheure = nbsec / 3600;
  nbmin = (nbsec / 60) % 60;
  nbsec = nbsec % 60;
  afficherEntierHeure(nbheure,L'0');
  putwchar(L':');
  afficherEntierHeure(nbmin,L'0');
  putwchar(L':');
  afficherEntierHeure(nbsec,L'0');
  while (nb < lgmax){
    putwchar(L' ');
    nb ++;
  }
}

void afficherTempsEsp(unsigned int nbsec,int lgmax){
  unsigned int nbheure, nbmin ;
  int nb = 0; /*nombre de caractere affiches*/
  nbheure = nbsec / 3600;
  nbmin = (nbsec / 60) % 60;
  nbsec = nbsec % 60;
  if (nbheure == 1){
    afficherEntierHeure(nbheure,L' ');
    afficherChaine(L" heure ");
    nb+=9;
  }
  else if (nbheure > 1){
    afficherEntierHeure(nbheure,L' ');
    afficherChaine(L" heures ");
    nb+=10;
  }
  if (nbmin == 1){
    afficherEntierHeure(nbmin,L' ');
    afficherChaine(L" minute ");
    nb+=10;
  }
  else if (nbmin > 1){
    afficherEntierHeure(nbmin,L' ');
    afficherChaine(L" minutes ");
    nb+=11;
  }
  if (nbsec == 1){
    afficherEntierHeure(nbsec,L' ');
    afficherChaine(L" seconde");
    nb+=10;
  }
  else if (nbsec > 1){
    afficherEntierHeure(nbsec,L' ');
    afficherChaine(L" secondes");  
    nb+=11;
  }
  while (nb < lgmax){
    putwchar(L' ');
    nb ++;
  }
}
