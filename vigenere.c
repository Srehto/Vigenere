/* declaration de fonctionnalites supplementaires */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define NB_CAR_MAX 26 /* Nombre de lettres dans l'alphabet*/
#define INDICE_MAX 30  /* Taille maximal du mot de passe possible*/
#define TAILLE_NOM_FICHIER_MAX 50 /* Taille maximal du nom du fichier en entré et en sortie */

/* declaration des fonctions */
void cesar(FILE*,FILE*);
int indice(FILE*);
int vigenere(char*, char*);

/* fonction principale*/
int main()
{
  int fin;
  printf("\n*********************************\n");
  printf("*    Decryptage de Vigenere     *\n");
  printf("*********************************\n");
  printf("\nQuel fichier voulez vous decrypter ? : ");
  char nom_fichier[TAILLE_NOM_FICHIER_MAX];
  char fichier_dest[TAILLE_NOM_FICHIER_MAX];
  scanf("%s",nom_fichier);
  printf("Nom du fichier en sortie ? : ");
  scanf("%s",fichier_dest);

  fin=vigenere(nom_fichier,fichier_dest);

  /* Conclusion en fonction de la valeur de fin retournée*/
  if(fin==-1)
    printf("Décryptage impossible !  Impossible de trouver un indice de congruance cohérant !\n");
  else if(fin==1)
    printf("Decryptage terminé ! Vous pouvez retrouver votre texte decrypté dans le fichier %s !\n",fichier_dest);
  else if(fin==0)
    printf("Décryptage impossible, fichier %s introuvable !\n",nom_fichier);
  else
    printf("Décryptage impossible ! Erreur inconnue !\n");

  return EXIT_SUCCESS;
}

/* Définition des fonctions */

/* Fonction qui va décrypter un fichier selon le nombre de César. Prend en entrée le fichier a décrypter, in, et le fichier de sortie ou sera écrit le texte décrypté, out.*/
void cesar(FILE* in,FILE* out)
{ 
  rewind(in);
  int tab[NB_CAR_MAX]={0};
  int i;
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
  //printf("Caractere : %c\nIndice : %d\n",indice+'a',indice);
  decalage=(26-indice+4)%26;// decalage vers l'avant
  //printf("Decalage : %d\n\n",decalage);
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

/* Fonction qui va retourner la taille du mot de passe du fichier en entrée*/
int indice (FILE* in)
{
  int i,k,caractere, compteur,nb_lettres;
  int lettre[NB_CAR_MAX];
  double indice;
  FILE* dest=NULL;
  for(i=2;i<=INDICE_MAX;i++)
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
      // printf("Nombre de lettre : %d\nIndice : %f\nNombre de lettres : %d\n\n\n ",i,indice,nb_lettres);
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
  return 0;
}

/* Fonction qui décrypte un texte selon la méthode de Vigenère. Prend en entré le fichier a décrypter et le nom du fichier de sortie. Retourne un code quand la fonction se termine, pour determiner si le décryptage c'est bien passé ou pas*/
int vigenere(char* in, char* out)
{
  int k=0;
  int i,caractere,compteur;
  FILE* vigenere=NULL;
  FILE* fichier_ces=NULL;
  vigenere=fopen(in,"r");
  if(vigenere!=NULL)
    {
      k= indice(vigenere);
      if(k==0)
	return -1;
      FILE* fichier;
      //printf("Longueur Mot de Passe : %d\n\n\n",k);
      char nom_fichier[TAILLE_NOM_FICHIER_MAX];
      char fichier_cesar[TAILLE_NOM_FICHIER_MAX];
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

      /*Assemblage des k fichiers*/
      FILE** cesar;
      cesar=(FILE**)malloc(k*sizeof(FILE*)); /* Allocation d'un tableau de k fichiers */
      FILE* decrypte=NULL;
      int num;
      for(i=1;i<=k;i++)
	{
	  sprintf(fichier_cesar,"cesar_partie_%d.txt",i);
	  cesar[i-1]=fopen(fichier_cesar,"r");
	}
      caractere=fgetc(cesar[0]);
      decrypte=fopen(out,"w+");
      i=0;
      num=0;
      while(caractere!=EOF)
	{
	  if(i>500)
	    {
	      fprintf(decrypte,"\n"); /* Un saut de ligne tous les 500 caracteres */
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
      /* Suppressions de tous les fichiers plus utilisés*/
      for(i=1;i<=k;i++)
	{
	  sprintf(fichier_cesar,"cesar_partie_%d.txt",i);
	  remove(fichier_cesar);
	}
      fclose(decrypte);
      fclose(vigenere);
      return 1;
    }
  else
    return 0;
}
