# Compresseur_Huffman

-------------------------------------------------
L'archive est composé de trois fichier principal:
-------------------------------------------------

1) compresseur_huffman.c : 
le code C pour compresser les fichiers texte, il prend deux paramètres:
- le premier est le nom de ficheir à compresser
- le deuxième est le nom de fichier qui sera généré par la comrpession

2) decompresseur_huffman.c :
le code C pour décompresser les fichiers texte, il prend également deux paramètres:
- le premier est le nom de ficheir à décompresser
- le deuxième est le nom de fichier qui sera généré par la décomrpession

3) huffman.py :
le script python pour archiver ou désarchiver un dossier (ou bien un simple fichier). Vous pouvez l'utiliser comme suivant:
huffman.py  [option]  [nom_dossier_fichier] 
[option]: vous devez impérativement choisir une des deux options suivantes:
1. -a : pour archiver
2. -d : pour désarchiver
[nom_dossier_fichier]: c'est le nom de dossier (ou fichier) qui sera archiver (ou désarchiver).

-----------------
!!! Remarques !!!
-----------------

- Le projet selon est complet (compression, décompression, archivage et désarchivage) et ne rencontre aucun bogues 
selon les tests fait par notre équipe ;)

- L'utilisation de l'archiveur (également le désarchiveur) ne nécessite aucune compilation faite par l'utilisatuer. 
Le script python compile lui-même les fichiers C nécessaires. 
La compilation est faite par le script uniquement si besoin (si le fichier C n'a pas déjà été compilé).

- Dans le cas d'archivage, un message de fin d'archivage (ou de désarchivage ) sera affiché afin d'indiquer que tous s'est bien passé.


----------------------------------------------------------------------------
M.MASHRA Marwan    
M.CAO Anh   
L2-info    
Université de Montpellier   
2019
----------------------------------------------------------------------------

