#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Database/display.c"
#define TAILLE_MAX 30

/*
   struct DATA{
   char artist[30];
   char songname[30];
   char genre;
   char Filename[20];
   int favorite;
   };

   struct Database{
   struct DATA data;
   struct Database *next;
   };

*/

void addrecommendation(struct Database *list,struct DATA music,FILE *stream){

	printf("%s \n",music.artist);
	while(list->next != NULL){
		struct DATA musicinlist = list->data;


		if(strcmp(musicinlist.songname,music.songname) != 0){


			if(musicinlist.genre == music.genre){
                                char *rep1 = musicinlist.FileName;
				fprintf(stream,"%s \n",rep1);
			}
			else
			{

				if(strcmp(musicinlist.artist,music.artist) == 0){
					char *rep1 = musicinlist.FileName;
					fprintf(stream,"%s \n",rep1);
				}
			}
		}
		list = list->next;
	}
}


int main(){

	FILE* fichier = NULL;

	fichier = fopen("song.txt","r");

	struct Database *list = ConstrucDatabase();

	struct Database *song = list;
	if(fichier != NULL)
	{
		char array[100] = "";
		if(fgets(array,100,fichier) != NULL)
		{
			size_t x = (size_t)array[1]-48;
			size_t y = (size_t)array[2]-48;
			size_t z = (size_t)array[3]-48;
			x = x * 100;
			y = y * 10;
			z+=1;
			for(size_t i = 0; i<(x+y+z) && song !=NULL; i++, song=song->next    ){}
		}

		const char* filename = "recommendation.txt";
		FILE *stream = fopen(filename,"w+");

		struct DATA music = song->data;
		addrecommendation(list,music,stream);
		fclose(stream);
		free(list);
	}
	return 0;
}





