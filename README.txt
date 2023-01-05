Ce projet a pour but de développer une application qui étudie la 
rentabilité d'une installation photovoltaïque.

1. Modélisation du problème

La structure Maison recense les équipemets électro-ménagers de l'utilisateur, 
qui sélectionne les équipements qu'il possède dans le catalogue.

Gestion du fichier catalogue nommé puissance_electromenagers.csv :
Ce catalogue est importé dans une table de hachage, qui est un tableau de liste chainées.
Les recherches d'equipement sont donc rapides grâce au principe des tables de hachages.

Les données des fichiers csv sont importés dans des tableaux à chaque fois que cela est nécessaire

L'utilisateur peut également ajouter des équipements dans le catalogue, en entrant leurs noms sans majuscules

L'utilisateur peut se référer à la consommation moyenne d'un ménage français: 6kW par jour

/*----------------------------------*\
Structure du projet :

Le dossier src contient tous les .c
    - filesmanagement.c regroupe les fonctions liées à la lecture, l'extraction et l'écriture de données dans les fichiers .csv, et a la manipulation de la table de hachage, ainsi qu'à la manipulation de la structure Maison
    - simulation.c regroupe le menu et les fonctions de calcul utilisées dans le menu
    - GraphicsSDL.c regroupe les fonctions liées à la gestion des graphes: ouvrir une fenêtre, tracer un graphique avec sa légende pour différentes plages temporelles  

Le dossier bin contient prog.exe (résultat de la compilation de notre travail), ainsi que les librairies dynamiques

Le dossier lib contient des librairies liées aux graphes

arial.ttf sert à définir la police utilisée dans la légende des graphes

Le dossier Dailydata regroupe les fichiers .csv contenant les irradiations journalières sur un mois donné de la plage 2005-2016

Le dossier include regroupe tous les fichiers headers, qui contiennent les prototypes des fonctions

/*----------------------------------*\
2. Menu
Fonctionnalités :

1- Ajouter les equipements (et leur temps d'utilisation) a la maison (case sensitive) 
2- Indiquer la surface exploitable pour les panneaux solaires et connaitre le nombre max de panneaux instalables 
3- Connaitre le nombre de panneaux pour sastisfaire la consommation moyenne de la maison 
4- Connaitre le nombre de panneaux pour sastisfaire la consommation instantannée maximale de la maison 
5- Specifier le nombre de panneaux a installer 
6- Obtenir les graphs (comparaison d'un meme mois au cours des annees 2005-2016) de la production en Wh moyenne sur une heure  de la maison avec le nombre de panneaux specifies  
7- Obtenir les graphs (comparaison des mois d'une meme annee) de la production en Wh moyenne sur une heure de la maison avec le nombre de panneaux specifies  
8- Duree de rentabilite de l'exploitation par energie solaire (en supposant que les besoins de la maison sont assurees)  
9- Obtenir la consommation journaliere de la maison 
10- Puissance moyenne requise par la maison 
11- Prix de la facture d'electricite a l'annee sans panneau solaire 
12- Connaitre les surplus totaux (heure/heure) en energie d'une journee typique d'un mois a specifier 
13- Connaitre les deficits totaux (heure/heure) en energie d'une journee typique d'un mois a specifier 
14- Graph production en Wh de l'installation (+comparaison avec conso Moyenne) au cours d'une journee typique d'un mois a specifier 
15- Ajouter un equipement au catalogue (/!\ conflits de nom non gérés )
16- Indiquer à l'utilisateur comment répartir efficacement ses besoins énergétiques, sur un horizon de temps donné
17- Quitter le menu

/*----------------------------------*\
3. Compilation avec l'outil make :

Se placer dans le repertoire sol/src
Taper la commande make
Puis pour executer le programme, taper ../bin/prog

Compilation en ligne de commande :
Se placer à la racine du projet, puis taper:
gcc -g src/*.c -o bin/prog -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image
execution : bin/prog

/*----------------------------------*\

Remarque : 
Nous avons implémenté un système de Graph grâce à la bibliothèque SDL et des extensions de la SDL
nous avons géré cet affichage "pixel par pixel" et avec interaction clavier, ce qui représente un travail très conséquent, dans le cadre des 5 points
pour "passer de 15 à 20" en plus des consignes demandés.

Nous avons préféré cette implémentation de graphiques a celle suggéré par l'énoncé (prévision) car les prévisions indiquées sur le site n'était pas spécifiques à la ville de Gardanne (comme nos données) et n'était prévisionelles que sur 1 ou 2 jours.

/*----------------------------------*\
Analyse des résultats obtenus :

Les surplus énergétiques trouvés sont plus élevés en Juillet, et les déficits énergétiques en hiver sont plus importants, ce qui est cohérent avec le rayonnement 

/*-------------------------------------*\
4. Améliorations possibles

    - Possibilité de gérer les conflits de nom des équipements électroménagers dans la fonction d'Ajout d'equipements
    - Prendre en compte la casse et les espaces pour rechercher un equipement dans la table de hachage. 
    - Prendre en compte les prévisions d'ensoleillement pour l'aide à la répartition efficace des besoins énergétiques de l'utilisateur     
    - Calculer la quantité en WH du stockage nécessaire selon la période choisie (pour combler les déficits avec les surplus)


Avantages

Réalisme de la modélisation choisie pour les utilisateurs gardannais
    - L'application peut s'adapter à un large public cible au sein de Gardanne, et s'adapte à la surface exploitable de l'utilisateur, ainsi qu'à la consommation électrique de son ménage
    - L'affichage des graphes permet à l'utilisateur d'avoir une vision d'ensemble des variations de puissance mises en jeu. 