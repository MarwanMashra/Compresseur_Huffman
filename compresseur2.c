#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h> 

struct noeud{
	int pere;
	int fd;
	int fg;
	float freq;
};
int nbOccurence[256];
size_t nbBitsFileCompressed=0;         //nombre de bits écrit dans le fichier compressé (on va le passer dans l'entête)
size_t nbOccTotale=0;                  //nombre de charactères dans le fichier d'origine
size_t taille_apres_compression;
void calculFrequence(char* fichier,float* frequence){
	for(int i=0;i<256;i++)
		nbOccurence[i]=0;
	FILE* fd= fopen(fichier,"r");

	if(!fd){
		printf("ERROR: impossible d'ouvrir le fichier %s\n",fichier);
		return;
	}
	int c;
	while((c=fgetc(fd))!=EOF){
		nbOccurence[c]++;
		nbOccTotale++;
	}
	printf("\n\n##########  La probabilité d’apparition  ##########\n");
	for(int i=0;i<256 && nbOccTotale;i++){
		frequence[i]=(float)nbOccurence[i]/nbOccTotale;
		if(frequence[i]!=0)printf("Le caractère %c a une probabilité d’apparition de %f\n",i,frequence[i]);
		
	}
	int close=fclose(fd);
	if(close==EOF){
		printf("une erreur survient durant la fermeture du fichier %s\n",fichier);
	}
}
void creatArbre(float* frequence,struct noeud* Arbre){
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

	printf("\n\n##########  L'arbre  ##########\n");
	for(int i=0;i<511;i++){
		if(Arbre[i].freq!=0)
		printf("Le caractère %c : le père(%i), le fd(%i), le fg(%i)\n",i,Arbre[i].pere,Arbre[i].fd,Arbre[i].fg);
	}
}
void geneCodeHuff(char** lettreCode,struct noeud* Arbre){
	lettreCode[256]=0;
	int index;
	int nbBit;
	int stop=0;
	int nbchar=0;
	int som_longueur=0;
	printf("\n\n##########  Le code des caractères  ##########\n");
	for(int i=0;i<256 && !(stop);i++){
		if(Arbre[i].freq!=0){
			nbBit=1;
			index=i;
			while(Arbre[index].pere!=-1){
				nbBit++;
				index=Arbre[index].pere;
			}
			if(Arbre[index].fd==-1){       //traiter le cas ou y a une seule lettre dans le ficheir
				nbBit++;
				lettreCode[i]=(char*) malloc(2*sizeof(char));
				lettreCode[i][1]='\0';
				lettreCode[index][0]='0';
				stop=1;            //si y a une seule lettre, on peut s'arrêter 
			}
			else{
				lettreCode[i]=(char*) malloc(nbBit*sizeof(char));
				lettreCode[i][nbBit-1]='\0';
				index=i;
				for(int k=nbBit-2;k>=0;k--){
					lettreCode[i][k]=(index==Arbre[Arbre[index].pere].fd)?'1':'0';
					index=Arbre[index].pere;
				}
			}
			printf("codeChar(%c) = %s\n",i,lettreCode[i]);
			
			//printf("%i\n",nbBitsFileCompressed );
			nbchar++;
			som_longueur+=(nbBit-1);
			nbBitsFileCompressed+=(nbBit-1)*nbOccurence[i];
		}
	}
	int lognueur_moyenne=som_longueur/nbchar;
	printf("La longueur moyenne des codes des caractères est : %i \n",lognueur_moyenne);
}
/////////////////////// Cette méthode d'écrire dans le fichier compressé n'utilise pas les opérateurs binaires ////////////////////////////////
/////////////////////// Elle a été remplacé par une autre méthode pour plud rapide et plus élégant selon M.Pompidor ;) ///////////////////////



/*int binaireToDecimal(char* C){
	int x=1;
	int res=0;
	for(int i=7;i>=0;i--){
		if(C[i]=='1') res+=x;
		x*=2;
	}
	return res;


void creationFile(char** lettreCode,char* fichierR,char* fichierW,struct noeud* Arbre){
	FILE* fw= fopen(fichierW,"w");
	FILE* fr= fopen(fichierR,"rb");
	if(!fr){
		printf("ERROR: impossible d'ouvrir le fichier %s\n",fichierR);
		return;
	}
	fprintf(fw,"%ld",nbBitsFileCompressed);
	for(int i=0;i<256;i++){
		if(nbOccurence[i]!=0)
			fprintf(fw,":%i:%i",i,nbOccurence[i]);
	}
	fprintf(fw,"#");

	int nbchar=0;
		char* codeBits=(char*) malloc (sizeof(char)*7);
		int cpt=0;
		int nbBits=0;
		int c=fgetc(fr);
		while(c!=EOF){

			while((nbBits<8)&&(lettreCode[c][nbchar]!='\0')){
				codeBits[nbBits]=lettreCode[c][nbchar]; 			
				nbBits++;
				nbchar++;
			}
			if(lettreCode[c][nbchar]=='\0') {
				nbchar=0;
				c=fgetc(fr);
				if((c==EOF)&&(nbBits<8)){
					for (int i=nbBits;i<8;i++){
						codeBits[i]='0';
						nbBits++;
					}
					printf("dernier lettre\n");
				}
				
				//printf("%i : Fin %c",tailleFichier,c);
				//if(c==EOF) printf(" FIN\n");
				//else printf(" Deb %c\n",c);			
			}
			//if((c==EOF)||(nbBits==8))printf("%i : Tab : %i\n",tailleFichier,nbBits );
			if(nbBits==8){
				printf("Char N%i :  %i\n",++cpt,binaireToDecimal(codeBits) );
				fputc(binaireToDecimal(codeBits),fw);
				nbBits=0;
			}

			// il faut remplir le reste du tableau si nbBits<8

			//printer le tableau codeBits
			//printf("%i : %s\n",++cptM,codeBits);

		}
		printf("%i\n",nbBitsFileCompressed );

}

*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void creationFile(char** lettreCode,char* fichierR,char* fichierW,struct noeud* Arbre){
	FILE* fw= fopen(fichierW,"w");
	FILE* fr= fopen(fichierR,"rb");
	if(!fr){
		printf("ERROR: impossible d'ouvrir le fichier %s\n",fichierR);
		return;
	}
	fprintf(fw,"%ld",nbBitsFileCompressed);
	for(int i=0;i<256;i++){
		if(nbOccurence[i]!=0)
			fprintf(fw,":%i:%i",i,nbOccurence[i]);
	}
	fprintf(fw,"#");

	unsigned char tab_octet[nbOccTotale];         //pour stocker tous les octets du fichier original
	size_t nb_octet=fread(tab_octet,1,nbOccTotale,fr);
	unsigned char octet=0x80;                              //octet pour écrire dans l'autre octet
	unsigned char c='\0';                        //octet à écrire dans le fichier 
	int indexC=0;					             //pour lire dans le tableau de code des lettres
	size_t cmp_octet= 0;                         //pour parcourir le tableau tab_octet
	while(cmp_octet < nb_octet){
		while(lettreCode[tab_octet[cmp_octet]][indexC]!='\0' && octet){
			if(lettreCode[tab_octet[cmp_octet]][indexC]=='1')
				c|=octet;
			octet>>=1;
			indexC++;
		}
		if(lettreCode[tab_octet[cmp_octet]][indexC]=='\0'){          //on a terminé de lire le code de la lettre
			cmp_octet++;                                             //on prend un nouveau octet (une nouvelle lettre)
			indexC=0;
		}
		if(!octet || cmp_octet==nb_octet){                    
			fputc(c,fw);                                            //on écrit l'octet dans le fichier compressé
			octet=0x80;
			c='\0'; 
		}
	}
	taille_apres_compression=ftell(fw);
	printf("la taille du fichier original est %ld\n",nbOccTotale);
	printf("Le taille du fichier compressé est %ld\n",taille_apres_compression);
	int gain = (1-((float)taille_apres_compression/nbOccTotale))*100;
	char percent='%';
	printf("Le gain est de %d%c\n",gain,percent);
	int closeR=fclose(fr);
	int closeW=fclose(fw);
	if(closeR==EOF){
		printf("une erreur survient durant la fermeture du fichier %s\n",fichierR);
	}
	if(closeW==EOF){
		printf("une erreur survient durant la fermeture du fichier %s\n",fichierW);
	}

}



int main(int argc, char** argv){
	if(argc!=3){
		printf("ERROR: veuillez entrer le nom de ficbhier à compresser et le nom de fichier compressé\n");
		return 1;
	}
	struct noeud Arbre[511];
	float frequence[256];
	char** lettreCode= (char**) malloc(257*sizeof(char*));
 	calculFrequence(argv[1],frequence);
	creatArbre(frequence,Arbre);
	geneCodeHuff(lettreCode,Arbre);
	creationFile(lettreCode,argv[1],argv[2],Arbre);
	return 0;
}


