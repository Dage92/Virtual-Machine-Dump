/* **************************************************/
/*   Projet      : Balade dans le metro             */
/*   Date        : mai 2009                         */
/*   Auteur      : Climoo                           */
/*   Fichier     : bm_utiles.h                      */
/*   Description : En-tete du module contenant      */
/*   plusieurs fonctions de bases utiles            */ 
/*   dans diverses modules                          */
/*                                                  */
/* **************************************************/

#ifndef _BM_UTILES_H

#define _BM_UTILES_H

/*Fonction qui permet de comparer deux chaines de caracteres,
  sans prendre en compte la casse et les caracteres speciaux.
  Retourne 1 si les chaines sont egales, 0 sinon */
char comparerNoms(wchar_t *nom1, wchar_t *nom2);

/*Fonction qui permet d'effectuer la saisie d'une chaine
  de caracteres au clavier, au maximum de longueur taille.
  Retourne 1 en cas de reussite, 0 sinon*/
char lireChaine(wchar_t *,int taille);

/*Macro qui affiche une chaine de caractere sur la sortie standard*/
#define afficherChaine(s) (fputws((s),stdout))

/*Fonction qui affiche une chaine de caracteres sur la sortie standard,
  complete par des espaces si necessaires.lgmax caracteres
  sont affiches*/
void afficherChaineEsp(wchar_t *s, int lgmax);

/*Fonction qui permet de lire un entier depuis la sortie
  standard*/
char lireEntier(unsigned int *n);

/*Fonction qui affiche un entier sur la sortie standard*/
void afficherEntier(unsigned int n);

/*Fonction qui affiche un entier sur la sortie standard
  complete par des espaces si necessaires. lgmax caracteres
  sont affiches*/
void afficherEntierEsp(unsigned int n, int lgmax);

/*Fonction qui permet d'afficher une heure au format hh:mm:ss,
  completee par des espaces si necessaires. lgmax caracteres
  sont affiches*/
void afficherHeureEsp(unsigned int nbsec,int lgmax);

/*Fonction qui permet d'afficher un temps au format
  hh heure(s) mm minute(s) ss seconde(s), completee
  par des espaces si necessaires. lgmax caracteres
  sont affiches*/
void afficherTempsEsp(unsigned int nbsec, int lgmax);

#endif /*bm_utiles.h*/
