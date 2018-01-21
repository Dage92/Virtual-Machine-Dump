/* **************************************************/
/*   Projet      : Balade dans le metro             */
/*   Date        : mai 2009                         */
/*   Auteur      : Climoo                           */
/*   Fichier     : bm_main.c                        */
/*   Description : Module contenant la fonction     */
/*   principale du programme                        */
/*                                                  */
/* **************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>

#if ((defined(WIN32))  || (defined(_WIN32)))
#include <windows.h>
#define WIN_CODEPAGE 1252
#endif

#include "bm_reseau.h"
#include "bm_utiles.h"
#include "bm_lectureFichier.h"
#include "bm_requete.h"
#include "bm_chemin.h"
#include "bm_itineraire.h"
#include "bm_affichage.h"

/*Fonction principale.*/
int main(int argc, char **argv){
  BM_Reseau br ;
  BM_Requete req;
  BM_Itineraire iti;
  int indFic;
  char ret;
  setlocale (LC_ALL, "");
#if ((defined(WIN32))  || (defined(_WIN32)))
  SetConsoleOutputCP(WIN_CODEPAGE);
#endif
  ret = initialiserRequete(argc,argv,&req,&indFic);
  if (ret == ERR_ALLOC){
    afficherChaine(L"Erreur fatale : erreur d'allocation dynamique de memoire\n");
    return -1;
  }
  if (ret == 0){
    afficherChaine(L"Erreur : Requete invalide\n");
    return 1;
  }
  ret = lireFichier(argv[indFic],&br);
  if (ret == ERR_ALLOC){
    afficherChaine(L"Erreur fatale : erreur d'allocation dynamique de memoire\n");
    supprimerRequete(&req);
    return -1;
  }
  if (ret == 0){
    supprimerRequete(&req);
    return 1;
  }
  afficherRequete(&req);

  ret = trouverItineraire(&br,&req,&iti);
  if (ret == ERR_ALLOC){
    supprimerReseau(&br);
    supprimerRequete(&req);
    afficherChaine(L"Erreur fatale : erreur d'allocation dynamique de memoire\n");
    return -1;
  }
  if (ret){ /*Si pas d'erreur lors de la creation de l'itineraire*/
    afficherItineraire(&iti,&br);
    supprimerItineraire(&iti);
  }
  supprimerRequete(&req);
  supprimerReseau(&br);
  return 0;
}
