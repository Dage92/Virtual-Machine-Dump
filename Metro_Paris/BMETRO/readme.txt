Auteur : Climoo
Les sources qui constituent ce projet "Balade dans le métro parisien" de même que la
documentation associée sont la propriété de leur auteur.
Je donne mon accord au site developpez.com pour l'utilisation de tout ou partie des sources
et de la documentation de ce projet dans les pages developpez.com
___________________________________________________________________________________________

Système d'exploitation : Ubuntu et Windows XP 
Type et fréquence du processeur utilisé : Pentium 
Compilateur : gcc , avec les options -W -Wall -ansi -pedantic -O3
Temps passé sur le projet : 45 heures environ
Fichier utilisé pour le projet : ListeStationsUtf8.txt
___________________________________________________________________________________________

Pour executer le programme:
-Se placer dans le répertoire avec tous les fichiers du projet
-Taper "make"
-Taper "bmetro " puis la liste des paramètres du programme.
___________________________________________________________________________________________

Décomposition modulaire et fichiers du projet:

- bm_main.c                   : contient la fonction principale main
- bm_lireFichier.[hc]         : gère la lecture du fichier source des stations de Métro, 
                                initialisation des structure composant le réseau
- bm_reseau.[hc]              : gère les fonctions de bases d'ajout ou de recherche de stations
- bm_requete.[hc]             : chargé de reconnaître la demande de trajet demandé par l'utilisateur
- bm_chemin.[hc]              : contient la fonction de calcul du plus court chemin d'une
                                station    
- bm_itineraire.[hc]          : calcul un itineraire en fonction d'une requete de l'utilisateur
- bm_itineraireInt.[hc]       : calcul un itineraire multiple, avec ordre des stations intermédiaires
                                imposé (module auxilaire de bm_itineraire)
- bm_itineraireOpt.[hc]       : calcul un itineraire oprimal, sans ordre des stations imposé
                                (module auxiliaire de bm_itineraire)                                 
- bm_affichage.[hc]           : chargé de l'affichage d'un itineraire résultant
- bm_utiles.[hc]              : contient plusieurs fonctions d'entrées sortie utilisées 
                                dans plusieurs modules
- bm_carac.h                  : contient les valeurs caracteres spéciaux (en particulier les caractères
                                accentués) en Unicode, qu'il est chargé de ne pas différencier d'autres.
								contient aussi quelques macros associés.
- readme.txt                  : ce fichier.			 
___________________________________________________________________________________________

Déroulement du programme:
Le programme principale se décompose en plusieurs parties
-Evaluer la requête formulée par l'utilisateur (station de depart et d'arrivée, temps moyen ...)
-Effectuer la lecture du fichier des stations
-Chercher un itinéraire répondant à la requête formulée
-Afficher le résultat

Choix de modélisation:
	Il s'agit ici de représenter les liens qui existent entre des stations de métro d'une
ville, afin de rechercher des itineraires entre elles. Une représentation simple est celle
sous forme de graphe. Chaque sommet est une station, chaque "portion" de ligne entre
deux stations forme un arc. Il existe plusieurs moyens efficaces de représenter un graphe,
ici on a choisi la modélisation sous forme de listes d'adjacence (pour chaque sommet du
graphe, on stocke dans une liste chaînée, l'ensemble de ses successeurs).
Les valuations des arcs, c'est à dire les distances entre les stations, ont toutes 
été supposées égales. En réalité, pour passer d'une station à une autre, il y a un cout
qui varie selon le fait qu'il soit nécessaire de changer ou non de ligne. Pour passer 
d'une station à l'autre, on stockera simplement via quelle ligne cela peut se faire
(dans les maillons des listes chaînées).

Algorihmes utilisés:
	Pour calculer le plus court chemin d'une station à une autre, l'algorithme de Dijkstra
a été utilisé, car c'est un algorithme simple et efficace, qui permet de connaître
(en un temps polynomial), tous les plus courts chemin à partir d'une station, ainsi
que le temps nécessaire mis pour y parvenir. Grâce à cet algorithme, on peut déjà
répondre à une requête simple de l'utilisateur, à savoir quel est le plus court chemin
d'une station à une autre du métro.
	Pour calculer un itinéraire multiple, mais dont l'ordre des étapes est imposé,
cela revient a calculer plusieurs itineraires simples, et de les mettre chacun bout
à bout, afin de créer l'itinéraire final. 
	Dans le cas ou l'ordre des étapes n'est pas défini, on se trouve alors face à 
un problème beaucoup plus complexe, qui ne peut être résolu en un temps normal, pour
un nombre suffisamment important de stations intermédiaires (en effet, le nombre
de chemin a exploré est de l'ordre de la factorielle du nombre de stations intermédiaires).
	Deux approches ont été utilisées pour proposer une solution.
La première consiste à construire un arbre, pour lequel chaque noeud correspond à une station.
La racine est la station de départ, et chacun de ses successeurs sont les stations vers
lesquelles il est possible de se rendre. Puis, à partir de chacun de ses stations, on 
stocke de nouveau les stations vers lesquelles il est possible de se rendre (moins la première),
etc... Il va de soit que seul le chemin le plus court est utilisé (calculé à l'aide de Dijkstra).
On progresse ainsi dans l'arbre, jusqu'à parvenir à la station d'arrivée (nécessairement la dernière
du trajet). A ce moment, on recherche alors la feuille de l'arbre qui a la distance la plus courte ;
c'est celle qui contient l'itinéraire optimal. On peut alors, en remontant à partir de cette feuille
jusque la racine, déterminer cet itinéraire.
	Cette méthode, complète, assure de trouver le meilleur itinéraire. Cependant, comme expliqué plus 
haut, il n'est pas possible dans la pratique de l'utiliser, si le nombre de stations intermédiaires
est trop élevé. Expérimentalement et avec les performances de la machine sur laquelle ont été fait
les tests, ce seuil a été fixé à 8. 
	Au delà de ce seuil, on applique donc une autre méthode, moins coûteuse, mais qui ne garantit
pas la totale optimalité du trajet. On va en fait choisir l'ordre des stations en commençant
par celle qui se trouve la plus proche de celle de départ, puis par celle qui est de nouveau
la plus proche, jusqu'à parvenir à la station d'arrivée. Dans la pratique, cette heuristique
de choix n'est pas des plus mauvaises, et l'itinéraire trouvé peut être considéré satisfaisant, pour
un simple parisien désireux de visiter un grand nombre de stations de métro...
_________________________________________________________________________________________

Remarques importantes sur le programme et son fonctionnement:

1)Dans le cas des itinéraires multiples, entre chaque étape de l'itinéraire, on compte toujours
un temps de changement, même si la personne reste sur la même ligne. Clairement, si la personne
a mentionné qu'elle voulait passer par une station, alors on va considérer que la personne
descend du métro à cette station, et attendra le prochain pour repartir.
Exemple: soit une ligne composée, dans l'ordre, des stations A,B,C,D,E.
L'utilisateur demande l'itinéraire de A vers E, en passant par D.
Le temps mis sera de (4*(temps entre deux stations) + 1*(temps de changement).
L'utilisateur demande l'itinéraire de A vers E.
Le temps mis sera de (4*(temps entre deux stations).
Alors que pourtant, on a réalisé exactement le même parcours.
Ce choix a été fait suite à une remarque faite dans le fil de discussion du programme.

2)La compatibilité du programme est (normalement) assurée sur les systèmes Linux
et sous Windows. Le programme a été testée sur ces deux OS. Cependant, il n'a pas 
été testé sur d'autres plateformes comme Mingw ou Cygwin. Il est donc possible
de rencontrer des problèmes de compilation/exécution sur ces plateformes (mais
la nécessité d'être compatible avec n'était pas explicitement demandé dans le sujet).
