#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h> 
#include <string.h>

struct noeud{
	int pere;
	int fd;
	int fg;
	float freq;
};
struct noeud Arbre[511];
float frequence[256];
int nbOccurence[256];
size_t nbBitsFileCompressed;
void readHead(char* fichier){
	FILE* fr= fopen(fichier,"r");
	if(fr==NULL){
		printf("ERROR: Impossible d'ouvrir le fichier %s\n",fichier);
		return ;
	}
	else{


		int lettre;
		int nbOcc;
		char separateur;
		fscanf(fr,"%ld %c",&nbBitsFileCompressed,&separateur);
		int nbChar=0;
		//printf("nbBitsFileCompressed = %i\n",nbBitsFileCompressed);

		for(int i=0;i<256;i++)
			nbOccurence[i]=0;
		while(separateur!='#'){
			fscanf(fr,"%d %c %d %c",&lettre,&separateur,&nbOcc,&separateur);
			nbOccurence[lettre]+=nbOcc;
			nbChar+=nbOcc;
		}
		for(int i=0;i<256;i++)
			frequence[i]=(float)nbOccurence[i]/nbChar;
		int close=fclose(fr);
		if(close==EOF){
			printf("une erreur survient durant la fermeture du fichier %s\n",fichier);
		}
	}
}

void createArbre(){
	for (int i = 0; i < 511; i++){
		Arbre[i].pere=-1;
		Arbre[i].fd=-1;
		Arbre[i].fg=-1;
		Arbre[i].freq=(i<256)?frequence[i]:0;
	}

	int indexPere=256;
	int indexmin1,indexmin2;
	float fmin1,fmin2;
	do{

		////////// récupérer les index des deux mins //////////
		fmin1=fmin2=1;
		indexmin1=indexmin2=-1;
		for (int i = 0; i < 511; i++){
			if((Arbre[i].freq!=0)&&(Arbre[i].pere==-1)&&(fmin1>Arbre[i].freq)){
				fmin1=Arbre[i].freq;
				indexmin1=i;
			}
		}
		for (int i = 0; i < 511; i++){
			if((Arbre[i].freq!=0)&&(Arbre[i].pere==-1)&&(fmin2>Arbre[i].freq)&&(i!=indexmin1)){
				fmin2=Arbre[i].freq;
				indexmin2=i;
			}
		}

		////////// mettre à jour le tableau Arbre //////////
		if(indexmin1!=-1 && indexmin2!=-1){
			Arbre[indexmin1].pere=indexPere;
			Arbre[indexPere].fd=indexmin1;  
			Arbre[indexmin2].pere=indexPere;
			Arbre[indexPere].fg=indexmin2;
			Arbre[indexPere].freq=Arbre[indexmin1].freq+Arbre[indexmin2].freq;
			indexPere++;	
		} 


	}while((indexmin1!=-1)&&(indexmin2!=-1));
	
}


void write(char* fichierR,char* fichierW){          
	FILE* fw= fopen(fichierW,"w");
	FILE* fr= fopen(fichierR,"rb");
	if(nbBitsFileCompressed){
		int racine;                            //l'index de la racine de l'arbre;
		for(int i=0;i<511;i++){
			if(Arbre[i].freq==1)
				racine=i;
		}
	 	int noeud = racine;                 //pour parcourir l'arbre par l'index de ses noeuds
		if(Arbre[noeud].fd==-1)           //traiter le cas une seule lettre dan sle fichier
			for(int i=0;i<nbBitsFileCompressed;i++)
				fputc(noeud,fw);
		else{
			size_t cpt = nbBitsFileCompressed; 
			while(fgetc(fr)!='#');      //pour aller jusqu'au début du texte (vaut mieux avoir stocker la postion)
			unsigned char* tabOctet = (char*) malloc(((nbBitsFileCompressed/8)+1)*sizeof(unsigned char));
			unsigned char octet_tmp;
			size_t nbOctet = fread(tabOctet,1,(nbBitsFileCompressed/8)+1,fr);
			unsigned char octet_test=0x80;
			for(size_t i=0;i<nbOctet && cpt ;i++){     //pour parcourir les octets
				octet_tmp=tabOctet[i];
				for(int k=0;k<8 && cpt;k++){		  //pour parcourir les bits de chaque octet
					noeud= (octet_tmp & octet_test)?Arbre[noeud].fd:Arbre[noeud].fg;
					octet_tmp<<=1;
					cpt--;
					if(Arbre[noeud].fd==-1){     //c'est une feuile, on l'écrit et on reprend depuis la racine
						fputc(noeud,fw);
						noeud=racine;
					}
				}
			}
		}
	}
	int closeR=fclose(fr);
	if(closeR==EOF){
		printf("une erreur survient durant la fermeture du fichier %s\n",fichierR);
	}
	int closeW=fclose(fw);
	if(closeW==EOF){
		printf("une erreur survient durant la fermeture du fichier %s\n",fichierW);
	}
}


int main(int argc, char** argv){
	if (argc!=3)
	{
		printf("ERROR: nombre de paramètres incorrect !!! \n");
		return 1;
	}
	readHead(argv[1]);
	createArbre();
	write(argv[1],argv[2]);         //argv[1]: nom de ficheir à compresser, argv[2]: nom de ficheir décompressé 
	return 0;
}