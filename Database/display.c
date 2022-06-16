#include <stdio.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>
//#include <glib.h>

struct DATA{
    char artist[30];
    char songname[30];
    int genre;
    char FileName[20];
    int favorite;
};

struct Database{
    struct DATA data;
    struct Database *next;
};

void getData(char Filename[],struct DATA *Data,struct Database *database){
    
    FILE *file;
    file = fopen(Filename, "rb");
    if(!file)
        errx(1,"couldn't open %s mp3 file\n",Filename);
    strcpy(Data->FileName,Filename);
    struct Database *result = calloc(1,sizeof(struct Database));
    char data[30] = "                              ";
    fseek(file,-125,SEEK_END);
    fflush(file);
    fgets(data,30,file);
    strcpy(Data->songname,data);
    fseek(file,-95,SEEK_END);
    fflush(file);
    fgets(data,30,file);
    strcpy(Data->artist,data);
    int a = 0;
    char c[4]="1234";
    int found = 0;
    while(found == 0){
    	for(;c[0] != '(' ;a++){
        fseek(file,a,SEEK_SET);
        fflush(file);
        fgets(c,4,file);
    	}
	//printf("%s \n",c);
	if(c[1] >= '0' && c[1] <= '9'){
    		if(c[2] != ')')
        		Data->genre = (int)((c[1]-48)*10+c[2]-48);
    		else
        		Data->genre = (int)(c[1]-48);
		found = 1;
	}else{
		a++;
		fseek(file,a,SEEK_SET);
		fflush(file);
		fgets(c,4,file);
	}
    }
    result->data = *Data;
    result->next = NULL;
    for(;database->next != NULL;database = database->next){ 
    }
    database->next = result;
}
void increment(char Name[]){
    if(Name[12] == '9'){
        Name[12] = '0';
        if(Name[11] == '9'){
            Name[11] = '0';
            if(Name[10] == '9'){
                Name[10] = '0';
                Name[9] ++;
            }else{
                Name[10] ++;
            }
        }
        else{
            Name[11] ++;
        }
    }
    else{
        Name[12] ++;
        }
}
int getlistlength(){
	char Filename[] = "Database/1000.mp3";
	int result = 0;
	FILE* file;
	file = fopen(Filename,"r");
	while(file){
		result ++;
		increment(Filename);
		file = fopen(Filename,"r");
		//printf("%s,\n",Filename);
	}
	return result;
}

void SaveDatabase(struct Database* base){
	FILE* save;
	base = base->next;
	save = fopen("database.txt","w");
	if(!save){
		errx(1,"database.txt couldn't open");
	}
	for(;base!=NULL;base=base->next){
		fprintf(save,"%s, ",base->data.artist);
		fprintf(save,"%s, ",base->data.songname);
		fprintf(save,"%i, %i;\n",base->data.genre,base->data.favorite);
	}
}
void freelist(struct Database *database){
    struct Database *holder = database;
    database = database->next;
    free(holder);
    while(database != NULL){
        holder = database;
        database = database->next;
        free(holder);
    }
}
void Build(struct Database* base, FILE* file,int i,char Filename[]){
	struct Database *next = calloc(1,sizeof(struct Database));
	next->next = NULL;
	struct DATA part;
	strcpy(part.FileName,Filename);
	char data[30];
	int place = 0;
	char c[4] = "1234";
	for(int j = 0; j < i; j++){
		while(c[0] != ';'){
			fseek(file,place,SEEK_SET);
			fgets(c,4,file);
			place++;
		}
		c[0] = 'E';
		place++;
	}
	int offset = 0;
	while(c[0] != ','){
		fseek(file,place+offset,SEEK_SET);
		fgets(c,4,file);
		offset++;
	}
	c[0] = 'E';
	fseek(file,place,SEEK_SET);
	fgets(data,offset,file);
	strcpy(part.artist,data);
	
	place = place+offset+1;
	offset = 0;
	while(c[0] != ','){
		fseek(file,place+offset,SEEK_SET);
		fgets(c,4,file);
		offset++;
	}

	fseek(file,place,SEEK_SET);
	fgets(data,offset,file);
	strcpy(part.songname,data);

	char holder[10];

	fseek(file,place+offset+1,SEEK_SET);
	fgets(holder,10,file);
	if(holder[1] <= '9' && holder[1] >= '0'){
		part.genre = (int) (10*(holder[0]-'0')+holder[1]-'0');
	}else{
		part.genre = (int) (holder[0]-'0');
	}

	fseek(file,place+offset+4,SEEK_SET);
	fgets(holder,4,file);
	if(part.genre < 10){
		part.favorite = (int) (holder[0]-'0');
	}else{
		part.favorite = (int) (holder[1]-'0');
	}
	for(;base->next != NULL;base = base->next){}
	base->next = next;
	next->data = part;
}

struct Database* RecoverDatabase(){
	int length = getlistlength();
	struct Database *DATABASE = calloc(1,sizeof(struct Database));
	DATABASE->next = NULL;
	FILE* file;
	char Songname[] = "Database/1000.mp3";
	file = fopen("database.txt","r");
	if(!file)
		errx(1,"Couldn't open the database.txt file in Recover fun");
	for(int i = 0;i<length;i++){
		Build(DATABASE,file,i,Songname);
		increment(Songname);
	}
	SaveDatabase(DATABASE);
	printf("Database recovered !\n");
	return DATABASE;
}
struct Database* ConstrucDatabase(){
    struct Database *DATABASE = calloc(1,sizeof(struct Database));
    DATABASE->next = NULL;
    char Songname[] = "Database/1000.mp3";
    struct DATA *Data = calloc(1,sizeof(struct DATA));
    int length = getlistlength();
    printf("length : %i\n",length);
    length = 200;
    for(int i = 0;i<length; i++){
        getData(Songname,Data,DATABASE);
        printf("artist : %s\n",Data->artist);
        printf("name : %s\n",Data->songname);
        printf("genre : %i\n",Data->genre);
        increment(Songname);
    }
    SaveDatabase(DATABASE);
    free(Data);
    return DATABASE;
}
/*
int main()
{
    struct Database *DATABASE = RecoverDatabase();
    //struct Database *DATABASE = ConstrucDatabase();
    freelist(DATABASE);
    //free(DATABASE);
    return 0;
}*/
