Auteur : Climoo
Les sources qui constituent ce projet "Balade dans le m�tro parisien" de m�me que la
documentation associ�e sont la propri�t� de leur auteur.
Je donne mon accord au site developpez.com pour l'utilisation de tout ou partie des sources
et de la documentation de ce projet dans les pages developpez.com
___________________________________________________________________________________________

Syst�me d'exploitation : Ubuntu et Windows XP 
Type et fr�quence du processeur utilis� : Pentium 
Compilateur : gcc , avec les options -W -Wall -ansi -pedantic -O3
Temps pass� sur le projet : 45 heures environ
Fichier utilis� pour le projet : ListeStationsUtf8.txt
___________________________________________________________________________________________

Pour executer le programme:
-Se placer dans le r�pertoire avec tous les fichiers du projet
-Taper "make"
-Taper "bmetro " puis la liste des param�tres du programme.
___________________________________________________________________________________________

D�composition modulaire et fichiers du projet:

- bm_main.c                   : contient la fonction principale main
- bm_lireFichier.[hc]         : g�re la lecture du fichier source des stations de M�tro, 
                                initialisation des structure composant le r�seau
- bm_reseau.[hc]              : g�re les fonctions de bases d'ajout ou de recherche de stations
- bm_requete.[hc]             : charg� de reconna�tre la demande de trajet demand� par l'utilisateur
- bm_chemin.[hc]              : contient la fonction de calcul du plus court chemin d'une
                                station    
- bm_itineraire.[hc]          : calcul un itineraire en fonction d'une requete de l'utilisateur
- bm_itineraireInt.[hc]       : calcul un itineraire multiple, avec ordre des stations interm�diaires
                                impos� (module auxilaire de bm_itineraire)
- bm_itineraireOpt.[hc]       : calcul un itineraire oprimal, sans ordre des stations impos�
                                (module auxiliaire de bm_itineraire)                                 
- bm_affichage.[hc]           : charg� de l'affichage d'un itineraire r�sultant
- bm_utiles.[hc]              : contient plusieurs fonctions d'entr�es sortie utilis�es 
                                dans plusieurs modules
- bm_carac.h                  : contient les valeurs caracteres sp�ciaux (en particulier les caract�res
                                accentu�s) en Unicode, qu'il est charg� de ne pas diff�rencier d'autres.
								contient aussi quelques macros associ�s.
- readme.txt                  : ce fichier.			 
___________________________________________________________________________________________

D�roulement du programme:
Le programme principale se d�compose en plusieurs parties
-Evaluer la requ�te formul�e par l'utilisateur (station de depart et d'arriv�e, temps moyen ...)
-Effectuer la lecture du fichier des stations
-Chercher un itin�raire r�pondant � la requ�te formul�e
-Afficher le r�sultat

Choix de mod�lisation:
	Il s'agit ici de repr�senter les liens qui existent entre des stations de m�tro d'une
ville, afin de rechercher des itineraires entre elles. Une repr�sentation simple est celle
sous forme de graphe. Chaque sommet est une station, chaque "portion" de ligne entre
deux stations forme un arc. Il existe plusieurs moyens efficaces de repr�senter un graphe,
ici on a choisi la mod�lisation sous forme de listes d'adjacence (pour chaque sommet du
graphe, on stocke dans une liste cha�n�e, l'ensemble de ses successeurs).
Les valuations des arcs, c'est � dire les distances entre les stations, ont toutes 
�t� suppos�es �gales. En r�alit�, pour passer d'une station � une autre, il y a un cout
qui varie selon le fait qu'il soit n�cessaire de changer ou non de ligne. Pour passer 
d'une station � l'autre, on stockera simplement via quelle ligne cela peut se faire
(dans les maillons des listes cha�n�es).

Algorihmes utilis�s:
	Pour calculer le plus court chemin d'une station � une autre, l'algorithme de Dijkstra
a �t� utilis�, car c'est un algorithme simple et efficace, qui permet de conna�tre
(en un temps polynomial), tous les plus courts chemin � partir d'une station, ainsi
que le temps n�cessaire mis pour y parvenir. Gr�ce � cet algorithme, on peut d�j�
r�pondre � une requ�te simple de l'utilisateur, � savoir quel est le plus court chemin
d'une station � une autre du m�tro.
	Pour calculer un itin�raire multiple, mais dont l'ordre des �tapes est impos�,
cela revient a calculer plusieurs itineraires simples, et de les mettre chacun bout
� bout, afin de cr�er l'itin�raire final. 
	Dans le cas ou l'ordre des �tapes n'est pas d�fini, on se trouve alors face � 
un probl�me beaucoup plus complexe, qui ne peut �tre r�solu en un temps normal, pour
un nombre suffisamment important de stations interm�diaires (en effet, le nombre
de chemin a explor� est de l'ordre de la factorielle du nombre de stations interm�diaires).
	Deux approches ont �t� utilis�es pour proposer une solution.
La premi�re consiste � construire un arbre, pour lequel chaque noeud correspond � une station.
La racine est la station de d�part, et chacun de ses successeurs sont les stations vers
lesquelles il est possible de se rendre. Puis, � partir de chacun de ses stations, on 
stocke de nouveau les stations vers lesquelles il est possible de se rendre (moins la premi�re),
etc... Il va de soit que seul le chemin le plus court est utilis� (calcul� � l'aide de Dijkstra).
On progresse ainsi dans l'arbre, jusqu'� parvenir � la station d'arriv�e (n�cessairement la derni�re
du trajet). A ce moment, on recherche alors la feuille de l'arbre qui a la distance la plus courte ;
c'est celle qui contient l'itin�raire optimal. On peut alors, en remontant � partir de cette feuille
jusque la racine, d�terminer cet itin�raire.
	Cette m�thode, compl�te, assure de trouver le meilleur itin�raire. Cependant, comme expliqu� plus 
haut, il n'est pas possible dans la pratique de l'utiliser, si le nombre de stations interm�diaires
est trop �lev�. Exp�rimentalement et avec les performances de la machine sur laquelle ont �t� fait
les tests, ce seuil a �t� fix� � 8. 
	Au del� de ce seuil, on applique donc une autre m�thode, moins co�teuse, mais qui ne garantit
pas la totale optimalit� du trajet. On va en fait choisir l'ordre des stations en commen�ant
par celle qui se trouve la plus proche de celle de d�part, puis par celle qui est de nouveau
la plus proche, jusqu'� parvenir � la station d'arriv�e. Dans la pratique, cette heuristique
de choix n'est pas des plus mauvaises, et l'itin�raire trouv� peut �tre consid�r� satisfaisant, pour
un simple parisien d�sireux de visiter un grand nombre de stations de m�tro...
_________________________________________________________________________________________

Remarques importantes sur le programme et son fonctionnement:

1)Dans le cas des itin�raires multiples, entre chaque �tape de l'itin�raire, on compte toujours
un temps de changement, m�me si la personne reste sur la m�me ligne. Clairement, si la personne
a mentionn� qu'elle voulait passer par une station, alors on va consid�rer que la personne
descend du m�tro � cette station, et attendra le prochain pour repartir.
Exemple: soit une ligne compos�e, dans l'ordre, des stations A,B,C,D,E.
L'utilisateur demande l'itin�raire de A vers E, en passant par D.
Le temps mis sera de (4*(temps entre deux stations) + 1*(temps de changement).
L'utilisateur demande l'itin�raire de A vers E.
Le temps mis sera de (4*(temps entre deux stations).
Alors que pourtant, on a r�alis� exactement le m�me parcours.
Ce choix a �t� fait suite � une remarque faite dans le fil de discussion du programme.

2)La compatibilit� du programme est (normalement) assur�e sur les syst�mes Linux
et sous Windows. Le programme a �t� test�e sur ces deux OS. Cependant, il n'a pas 
�t� test� sur d'autres plateformes comme Mingw ou Cygwin. Il est donc possible
de rencontrer des probl�mes de compilation/ex�cution sur ces plateformes (mais
la n�cessit� d'�tre compatible avec n'�tait pas explicitement demand� dans le sujet).
