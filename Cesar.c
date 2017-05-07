/* declaration de fonctionnalites supplementaires */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define NB_CAR_MAX 26

void cesar(FILE*,FILE*);
int indice(FILE*);
/* Tableau ou je range les caractere du fichier a compresser selon leurs occurance */
int main()
{
  FILE* origine=fopen("text_crypte_cesar_1.txt","r");
  FILE* nouveau=fopen("decryptage.txt","w");
  cesar(origine,nouveau);
  int k=0;
  int i,caractere,compteur;
  FILE* vigenere=NULL;
  FILE* fichier_ces=NULL;
  vigenere=fopen("text_crypte_vigenere_petit.txt","r");
  k= indice(vigenere);
  FILE* fichier;
  printf("Longueur Mot de Passe : %d\n\n\n",k);
  char nom_fichier[20];
  char fichier_cesar[20];
  for(i=1;i<=k;i++)
    {
      rewind(vigenere);
       caractere=fgetc(vigenere);
      while((caractere<'a'|| caractere>'z') && caractere !=EOF)
	caractere=fgetc(vigenere);
      compteur=0;
      while(compteur+1<i)
	{
	  caractere=fgetc(vigenere);
	  while((caractere<'a'|| caractere>'z') && caractere !=EOF)
	    caractere=fgetc(vigenere);
	  compteur++;
	}
      sprintf(nom_fichier,"partie_%d.txt",i);
      fichier=fopen(nom_fichier,"w+");
      while(caractere!=EOF)
	{  
	  if(caractere>='a' && caractere<='z' && caractere !=EOF)
	    fprintf(fichier,"%c",caractere);
	  else
	    {
	      while((caractere<'a' || caractere>'z')&& caractere !=EOF)
		caractere=fgetc(vigenere);
	    }
	  compteur=0;
	  while(compteur<k)
	    {
	      caractere=fgetc(vigenere);
	      while((caractere<'a'|| caractere>'z') && caractere !=EOF)
		caractere=fgetc(vigenere);
	      compteur++;
	    }
	}
      fclose(fichier);
    }

  for(i=1;i<=k;i++)
    {
      sprintf(nom_fichier,"partie_%d.txt",i);;
      fichier=fopen(nom_fichier,"r+");
      sprintf(fichier_cesar,"cesar_partie_%d.txt",i);
      fichier_ces=fopen(fichier_cesar,"w+");
      cesar(fichier,fichier_ces);
      remove(nom_fichier);
    }

  //Assemblage fichier

  FILE* cesar[k];
  FILE* decrypte=NULL;
  int num=0;
  for(i=1;i<=k;i++)
    {
      sprintf(fichier_cesar,"cesar_partie_%d.txt",i);
      cesar[i-1]=fopen(fichier_cesar,"r");
    }

  caractere=fgetc(cesar[0]);
  decrypte=fopen("decrypte.txt","w+");
  i=0;
  while(caractere!=EOF)
    {
      if(i>500)
	{
	  fprintf(decrypte,"\n");
	  i=0;
	}
      fprintf(decrypte,"%c",caractere);
      if(num==k-1)
	num=0;
      else
	num++;
      caractere=fgetc(cesar[num]);
      i++;
    }



  return EXIT_SUCCESS;
}



void cesar(FILE* in,FILE* out)
{ 
  rewind(in);
  int tab[NB_CAR_MAX]={0};
  int i,x,k;
  int caractere,decalage;
  caractere=fgetc(in);
  while(caractere!=EOF) 
    {
      if(caractere>='a' && caractere<='a'+25)
	{
	  tab[caractere-'a']++;
	}
      caractere=fgetc(in);
    }
  int max=tab[0];
  int indice=0;
  for(i=1;i<NB_CAR_MAX;i++)
    {
      /*Tri*/
      if(tab[i]>max)
	{
	  indice=i;
	  max=tab[i];
	}
    }
  printf("Caractere : %c\nIndice : %d\n",indice+'a',indice);
  decalage=(26-indice+4)%26;// decalage vers l'avant
  //decalage=(indice-4+26)%26;//decalage vers l'arriere
  printf("Decalage : %d\n\n",decalage);
  rewind(in);
  caractere=fgetc(in);
  while(caractere!=EOF) 
    {
      if(caractere>='a' && caractere<='z')
	{
	  caractere=(caractere-'a'+decalage+26)%26+'a';
	}
      fprintf(out,"%c",caractere);
      caractere=fgetc(in);
    }
  fclose(out);
  fclose(in);
}

int indice (FILE* in)
{
  int i,k,caractere, compteur,nb_lettres;
  int lettre[NB_CAR_MAX];
  double indice;
  FILE* dest=NULL;
  for(i=2;i<=30;i++)
    {
     
      rewind(in);
      dest=fopen("mot_de_passe.txt","w+");
      caractere=fgetc(in);
      while(caractere!=EOF)
	{  
	  compteur = 0;
	  if (caractere>= 'a' && caractere <= 'z')
	    {
	      fprintf(dest, "%c", caractere);
	      nb_lettres++;
	      lettre[caractere-'a']++;
	    }
	  while(compteur<i)
	    {
	      caractere=fgetc(in);
	      if(caractere>='a'&& caractere<='z' && caractere !=EOF)
		compteur++;
	      if(caractere==EOF)
		break;
	    }
	}
      for(k=0;k<NB_CAR_MAX;k++)
	indice=indice+lettre[k]*(lettre[k]-1);
      indice=indice/((long)nb_lettres*(nb_lettres-1));
       printf("Nombre de lettre : %d\nIndice : %f\nNombre de lettres : %d\n\n\n ",i,indice,nb_lettres);
      remove("mot_de_passe.txt");

      if(indice>0.07 && indice<0.08)
	return i;
      else
	{
	  for(k=0;k<NB_CAR_MAX;k++)
	    lettre[k]=0;
	  indice=0;
	  nb_lettres=0;
	}
    }
}
