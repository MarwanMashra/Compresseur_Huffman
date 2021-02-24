#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import re,os,sys


liste_fichier={};



#fonction pour remplis liste_fichier, je l'utilise dans l'archiveur
def parcours(repertoire):
	if(os.path.isdir(repertoire)):
		liste = os.listdir(repertoire)
		for fichier in liste :
			parcours(repertoire+"/"+fichier)
	else:
		nblignes = sum(1 for line in open(repertoire))
		liste_fichier[repertoire]=nblignes




if (len(sys.argv)==3 and sys.argv[1][0]=='-' and len(sys.argv[1])==2 and (sys.argv[1][1]=='a' or sys.argv[1][1]=='d')):

	liste=sys.argv[2].split('/')     #pour enlever le slash inutile à la fin du dossier
	nom_sans_slash= liste[0]
	
	#########################################################################################################################
	########################################   archiveur   ##################################################################
	#########################################################################################################################


	if(sys.argv[1][1]=='a'):
		parcours(nom_sans_slash)       #liste_fichier={'fichier : nb-lignes '}
		cat="cat "
		texte=str(len(liste_fichier))+"\n"
		for fichier in liste_fichier:         
			texte+=str(liste_fichier[fichier])+"\n"+fichier+"\n"
			cat+=fichier+" "
		liste_contenu_fichier=os.popen(cat)
		for ligne in liste_contenu_fichier:
			texte+=ligne
		file=open("ceci_est_un_fichier_de_passage","w")   #le fichier de passage va contenir la concaténation de tous les fichiers avec une entête
		os.system("chmod u+rwx ceci_est_un_fichier_de_passage")             #au cas ou on a pas les droits nécessaires pour le fechier créé
		file.write(texte)                                             
		file.close()

		#je compile compresseur_huffman.c si besoin
		test_compilation=os.popen("ls | grep ^compresseur_huffman$ | wc -l")   
		for ligne in test_compilation:
			if(ligne=="0\n"):
				os.system("gcc compresseur_huffman.c -o compresseur_huffman")

		#je récupére le nom sans l'extension si c'est un fichier, ou sans le '/' inutile à la fin si c'est dossier

		nom_fichier=re.search("(.+)\..",nom_sans_slash)       
		if(nom_fichier):
			nom=nom_fichier.group(1)
		else:
			nom=nom_sans_slash


		compresseur="./compresseur_huffman ceci_est_un_fichier_de_passage "+nom+".huf"
		os.system(compresseur)                                #je compresse le fichier de pasage
		rm="rm -r "+nom_sans_slash
		os.system(rm)                                        #je supprime le fichier/dossier original après la compression
		os.system("rm ceci_est_un_fichier_de_passage")       #je supprime le fichier de passage 
		print("############################################")
		print("###########  Archivage terminée  ###########") 
		print("############################################")


	#########################################################################################################################
	########################################   désarchiveur   ###############################################################
	#########################################################################################################################

	else:
		#je compile decompresseur_huffman.c si besoin
		test_compilation=os.popen("ls | grep ^decompresseur_huffman$ | wc -l")   
		for ligne in test_compilation:
			if(ligne=="0\n"):
				os.system("gcc decompresseur_huffman.c -o decompresseur_huffman")


		#je me déparasse du '/' inutile à la fin de dossier
	

		#je vérifie que le nom de fichiera l'extension '.huf', sinon l'uilisatuer l'a oublié ou il a entré une mauvaise extension, donc je l'ajoute
		nom_fichier=re.search("(.+)\.(.+)",nom_sans_slash)         
		if(nom_fichier):
			nom=nom_fichier.group(1)+".huf"
		else:
			nom=nom_sans_slash+".huf"

		#je décompresse le fichier compressé dans un fichier de passage qui sera supprimé par la suite et je supprime le fichier compressé
		os.system("./decompresseur_huffman "+nom+" ceci_est_un_fichier_de_passage")
		os.system("rm "+nom)

		#j'ouvre le fihier de passage et je récupère son contenu lignes par lignes dans une liste 
		os.system("chmod u+rwx ceci_est_un_fichier_de_passage")             #au cas ou on a pas les droits nécessaires pour le fechier créé
		file=open("ceci_est_un_fichier_de_passage","r")
		liste_lignes=file.readlines()
		file.close()
		os.system("rm ceci_est_un_fichier_de_passage")

		#je lis l'entête et je récupere taille, fichier_liste comme (nom de fichier , nombre de lignes) 
		tmp_liste=re.search("(\d+)\n",liste_lignes[0])
		num_fichiers=int(tmp_liste.group(1))
		cpt=1
		for i in range(1,num_fichiers+1):
			tmp_liste=re.search("(\d+)\n",liste_lignes[cpt])
			tmp_num_lignes=int(tmp_liste.group(1))
			cpt+=1
			tmp_liste=re.search("(.+)\n",liste_lignes[cpt])
			tmp_nom_fichier=tmp_liste.group(1)
			cpt+=1
			liste_fichier[tmp_nom_fichier]=tmp_num_lignes

		#je récuprère les dossiers nécessaires dans une liste et je les crée
		liste_dossier=[]
		for fichier in liste_fichier:
			liste=fichier.split('/')
			dossier=""
			for x in range(0,len(liste)-1):
				dossier+=liste[x]+"/"
				if dossier not in liste_dossier:
					liste_dossier.append(dossier)
					os.system("mkdir "+dossier)


		#je récupère les lignes et je les écris dans les fichiers après les avoirs créer
		for cle in liste_fichier:
			texte =""
			for x in range(0,liste_fichier[cle]):
				texte+=liste_lignes[cpt]
				cpt+=1
			file=open(cle,"w")
			file.write(texte)
			file.close()

		print("############################################")
		print("##########  désarchivage terminée  #########") 
		print("############################################")



else:
	print("ERREUR: un problème avec les paramètres")
	print("huffman.py  [option]  [fichier]")
	print("Veuillez choisir une des deux options:\n-a : archiver\n-d : désarchiver");
	print("Veuillez entrer le nom de fihier que vous voulez archiver ou désarchiver");