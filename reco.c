#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int first()
{
	FILE *fichier = NULL;
	fichier = fopen("recommendation.txt","r");

	//const char* filename = "newrecommendation.txt";
	//FILE *stream = fopen(filename,"w+");
	int caractereActuel= 0;
	int i = 0;
	if(fichier != NULL)
	{
		while(caractereActuel != EOF){
			//printf("%c",caractereActuel);
			if(caractereActuel == '\n')
				i += 1;
			//fprintf(stream,"%c",caractereActuel);
			caractereActuel = fgetc(fichier);

		}
	}
	//printf("%d", i);
	fclose(fichier);
	return i;

}

int main()
{
	int i = first();
	int caractereActuel= 0;


	const char* filename = "newrecommendation.txt";
	FILE *stream = fopen(filename,"w+");

	srand(time(NULL));
	int r = rand() % i;
	int r1 = r;
	while(r1 == r)
	{
		r1 = rand() % i;
	}
	int i1 = 0;

	int r2 = r1;
	while(r2 == r1)
	{
		r2 = rand() % i;
		if( r2 == r)
			r2 = r1;
	}
	printf("%d , %d , %d \n",r,r1,r2);

	FILE *fichier = NULL;
	fichier = fopen("recommendation.txt","r");
	if(fichier != NULL)
	{
		caractereActuel = fgetc(fichier);
		while(caractereActuel != EOF){
			if( i1 == r || i1 == r1 || i1 == r2)
			{
				fprintf(stream,"%c",caractereActuel);
				printf("%c",caractereActuel);
			}
			if(caractereActuel == '\n')
				i1 += 1;
			caractereActuel = fgetc(fichier);

		}
	}
	fclose(fichier);
	fclose(stream);


}
