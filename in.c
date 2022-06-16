#include <gtk/gtk.h>
#include <unistd.h>
#include <string.h>
#include "Database/display.c"


typedef struct UserInterface
{
    GtkBuilder* builder;
    GtkWindow* window;
    GtkStack* stack;
    GtkBox* principal;
    GtkGrid* rec;
    GtkGrid* similar_songs;
    GtkGrid* positive_search;
    GtkGrid* fav;

    struct Database* DATABASE;
    struct Database* song;
    struct Database* similar;
    struct Database* reco;

    GtkLabel* name_of_song;
    GtkLabel* artist_name;
    GtkLabel* fav_list_lab;
    GtkLabel* similar_song;
    GtkLabel* rec_label;
    GtkLabel* add_lab;
    GtkLabel* ss_un;
    GtkLabel* ss_deux;
    GtkLabel* ss_trois;
    GtkLabel* ss_quatre;
    GtkLabel* rec_label1;
    GtkLabel* rec_label2;
    GtkLabel* rec_label3;

    //similar songs
    GtkButton* ss_add_fav1;
    GtkButton* ss_add_fav2;
    GtkButton* ss_add_fav3;
    GtkButton* ss_add_fav4;
    GtkButton* ss_play1;
    GtkButton* ss_play2;
    GtkButton* ss_play3;
    GtkButton* ss_play4;

    int nsong;
}UserInterface;


void on_fav_popup(GtkButton* b, gpointer user)
{
    UserInterface* ui = user;
    gtk_button_set_use_underline(b, TRUE);

    struct Database *song = ui->DATABASE->next;
    char fav[500]= "";
    int a = 1;
    for(;song!=NULL;song=song->next, a++)
    {

        if(song->data.favorite == 1)
        {
            strcat(fav,song->data.artist);
            strcat(fav,"\n");
            strcat(fav, song->data.songname);
            strcat(fav,"\n\n");
            printf("%s\n",fav);
        }
    }

    if(strlen(fav)==0)
        strcat(fav,"You haven't \nfaved any song \nfor the moment.");
    gtk_label_set_text((ui->fav_list_lab),fav);
    gtk_label_set_justify(ui->fav_list_lab, GTK_JUSTIFY_CENTER);

    gtk_widget_show(GTK_WIDGET(ui->fav));
    gtk_widget_hide(GTK_WIDGET(ui->principal));
    gtk_window_resize(ui->window, 10,10);
    gtk_widget_hide(GTK_WIDGET(ui->window));
    gtk_window_set_position(ui->window, GTK_WIN_POS_CENTER);
    gtk_widget_show(GTK_WIDGET(ui->window));
}

void on_back(GtkButton* b, gpointer user)
{
    UserInterface* ui = user;
    gtk_button_set_use_underline(b, TRUE);
    gtk_widget_hide(GTK_WIDGET(ui->rec));
    gtk_widget_hide(GTK_WIDGET(ui->similar_songs));
    gtk_widget_hide(GTK_WIDGET(ui->positive_search));
    gtk_widget_hide(GTK_WIDGET(ui->fav));
    gtk_widget_show(GTK_WIDGET(ui->principal));

    gtk_widget_hide(GTK_WIDGET(ui->window));
    gtk_window_set_position(ui->window, GTK_WIN_POS_CENTER);
    gtk_widget_show(GTK_WIDGET(ui->window));

}

void on_rec(GtkButton *b, gpointer user)
{
    UserInterface *ui = user;
    gtk_button_set_use_underline(b, TRUE);
    GtkWidget *p_dialog = NULL;
    p_dialog = gtk_file_chooser_dialog_new ("Ouvrir un fichier", NULL,
            GTK_FILE_CHOOSER_ACTION_OPEN,
            "document-cancel", GTK_RESPONSE_CANCEL,
            "document-open", GTK_RESPONSE_ACCEPT,
            NULL);

    gchar *file_name = NULL;
    if (gtk_dialog_run (GTK_DIALOG (p_dialog)) == GTK_RESPONSE_ACCEPT)
    {

        file_name = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (p_dialog));
    }
    gtk_widget_destroy (p_dialog);

    char* command = calloc(100,sizeof(char));
    //command = strcat(command, "ffmpeg -i dataa/1026.wav -ab 320k test/son.wav");
    //command = strcat(command, "ffmpeg -i recording.mp3 -ab 320k audio/test/son.wav");
    //system(command);
    //free(command);
    
    //fais la reconnaissance de son
    command = strcat(command, "ffmpeg -i ");
    command = strcat(command, file_name);
    command = strcat(command," -ab 320k audio/test/son.wav");
    system(command);
    system("./audio/test/shazam audio/test/son.wav");
    //system("./audio/test/shazam data/1070.wav");///home/diane/Music/gunstest.wav");
    
    //lis le son trouve
    FILE* fichier = fopen("song.txt","r");
    if(fichier != NULL)
    {
        char array[100] = "";
        if(fgets(array,100,fichier) != NULL)
        {
            int x = (int)array[1]-48;
            int y = (int)array[2]-48;
            int z = (int)array[3]-48;
            x = x * 100;
            y = y * 10;
            z+=1;
            struct Database *song = ui->DATABASE;
            for(int i = 0; i<(x+y+z) && song != NULL; i++, song = song->next){}
            gtk_label_set_text(ui->name_of_song, song->data.songname);
            gtk_label_set_justify(ui->name_of_song, GTK_JUSTIFY_CENTER);
            gtk_label_set_text(ui->artist_name, song->data.artist);
            gtk_label_set_justify(ui->artist_name, GTK_JUSTIFY_CENTER);
            ui->song = song;
            ui->nsong = x+y+z-1;
        }
        fclose(fichier);
    }
    gtk_widget_show(GTK_WIDGET(ui->positive_search));
    gtk_widget_hide(GTK_WIDGET(ui->principal));
    gtk_window_resize(ui->window, 10,10);
    gtk_window_set_position(ui->window, GTK_WIN_POS_CENTER);

    gtk_widget_hide(GTK_WIDGET(ui->window));
    gtk_window_set_position(ui->window, GTK_WIN_POS_CENTER);
    gtk_widget_show(GTK_WIDGET(ui->window));


    //system("rm -rf test/son.wav");
    //system("clear");
}

void on_m_clicked(GtkButton* b, gpointer user)
{
    UserInterface* ui = user;
    gtk_button_set_use_underline(b, TRUE);
    FILE* fichier = fopen("similar.txt","r");
    if(fichier != NULL)
    {
        char array[100][100] = {""};
        int i = 0;
        //char str1[300] = "";
        while(i <= 100 && fgets(array[i],100,fichier) != NULL)
        {
            i++;
        }
        int x = 0;
        int y = 0;
        int z = 0;
        struct Database* ss = ui->similar;
        //printf("%s", ss->next->data.songname);
        struct Database* song2 = ui->DATABASE->next;
        for(int j = 0; j < i; j++)
        {
            x = (int) array[j][1] - 48;
            y = (int) array[j][2] - 48;
            z = (int) array[j][3] - 48;
            x = x * 100;
            y = y * 10;
            int a = 0;
            struct Database* song = song2;
            for(; a<(x+y+z) && song; a++, song = song->next){}
            ss->next = calloc(1,sizeof(struct Database));
            ss->next->data = song->data;
            ss = ss->next;
        }
        ss = ui->similar->next;
        char text[100] = "";
        char* mt = "";


        strcat(text, ss->data.songname);
        strcat(text,"\n");
        strcat(text,ss->data.artist);
        gtk_label_set_text(ui->ss_un, text);
        gtk_label_set_justify(ui->ss_un, GTK_JUSTIFY_CENTER);
        strcpy(text,mt);
        ss = ss->next;

        strcat(text, ss->data.songname);
        strcat(text,"\n");
        strcat(text,ss->data.artist);
        gtk_label_set_text(ui->ss_deux, text);
        gtk_label_set_justify(ui->ss_deux, GTK_JUSTIFY_CENTER);
        strcpy(text,mt);
        ss = ss->next;

        strcat(text, ss->data.songname);
        strcat(text,"\n");
        strcat(text,ss->data.artist);
        gtk_label_set_text(ui->ss_trois, text);
        gtk_label_set_justify(ui->ss_trois, GTK_JUSTIFY_CENTER);
        strcpy(text,mt);
        ss = ss->next;

        strcat(text, ss->data.songname);
        strcat(text,"\n");
        strcat(text,ss->data.artist);
        gtk_label_set_text(ui->ss_quatre, text);
        gtk_label_set_justify(ui->ss_quatre, GTK_JUSTIFY_CENTER);
        strcpy(text,mt);
        ss = ss->next;



        fclose(fichier);
        //gtk_label_set_text(ui->similar_song,str1);
        //gtk_label_set_text(ui->similar_song,"green day wake me up when september ends\nEvanescence My Heart is Broken\nAudioslave Like a Stone\n");
    }
    gtk_widget_show(GTK_WIDGET(ui->similar_songs));
    gtk_widget_hide(GTK_WIDGET(ui->positive_search));
    gtk_window_resize(ui->window, 10,10);
    gtk_window_set_position(ui->window, GTK_WIN_POS_CENTER);

    gtk_widget_hide(GTK_WIDGET(ui->window));
    gtk_window_set_position(ui->window, GTK_WIN_POS_CENTER);
    gtk_widget_show(GTK_WIDGET(ui->window));


}

void on_refresh(GtkButton* b, gpointer user)
{
    UserInterface* ui = user;
    gtk_button_set_use_underline(b,TRUE);
    struct Database* reco = ui->reco;
    system("./reco");

    FILE* popol = fopen("newrecommendation.txt","r");
    if(popol)
    {
        for(int pol=0; pol<3; pol++)
        {
            char* str = calloc(20,sizeof(char));
            fgets(str,20,popol);
            char* str2 = calloc(3,sizeof(char));
            str2[0] = str[10];
            str2[1] = str[11];
            str2[2] = str[12];
            int music = atoi(str2);
            printf("(COUCOU\n");
            printf("%s : %s -> %d\n",str,str2,music);
            int i = 0;
            struct Database* s = ui->DATABASE->next;
            for(; s && i<music; s=s->next, i++){}
            reco->next = calloc(1,sizeof(struct Database));
            reco->next->data = s->data;
            reco = reco->next;
        }
    }
    fclose(popol);

    char* lab = calloc(100,sizeof(char));
    char* mt = "";
    reco = ui->reco->next;
    strcat(lab,reco->data.songname);
    strcat(lab,"\n");
    strcat(lab,reco->data.artist);
    reco=reco->next;
    gtk_label_set_text(ui->rec_label1, lab);
    gtk_label_set_justify(ui->rec_label1, GTK_JUSTIFY_CENTER);
    strcpy(lab,mt);

    strcat(lab,reco->data.songname);
    strcat(lab,"\n");
    strcat(lab,reco->data.artist);
    reco=reco->next;
    gtk_label_set_text(ui->rec_label2, lab);
    gtk_label_set_justify(ui->rec_label2, GTK_JUSTIFY_CENTER);
    strcpy(lab,mt);

    strcat(lab,reco->data.songname);
    strcat(lab,"\n");
    strcat(lab,reco->data.artist);
    reco=reco->next;
    gtk_label_set_text(ui->rec_label3, lab);
    gtk_label_set_justify(ui->rec_label3, GTK_JUSTIFY_CENTER);
    strcpy(lab,mt);





    gtk_widget_hide(GTK_WIDGET(ui->rec));
    gtk_widget_show(GTK_WIDGET(ui->rec));

}

void on_rec_popup(GtkButton* b, gpointer user)
{
    UserInterface* ui = user;
    gtk_button_set_use_underline(b, TRUE);
    struct Database* reco = ui->reco;
    system("./recommendation && ./reco");

    FILE* popol = fopen("newrecommendation.txt","r");
    if(popol)
    {
        for(int pol=0; pol<3; pol++)
        {
            char* str = calloc(17,sizeof(char));
            fgets(str,17,popol);
            char* str2 = calloc(3,sizeof(char));
            str2[0] = str[10];
            str2[1] = str[11];
            str2[2] = str[12];
            int music = atoi(str2);
            printf("%s : %s - > %d",str, str2, music);
            int i = 0;
            struct Database* s = ui->DATABASE->next;
            for(; s && i<music; s=s->next, i++){}
            reco->next = calloc(1,sizeof(struct Database));
            reco->next->data = s->data;
            reco = reco->next;
        }
    }
    fclose(popol);

    char* lab = calloc(100,sizeof(char));
    char* mt = "";
    reco = ui->reco->next;
    strcat(lab,reco->data.songname);
    strcat(lab,"\n");
    strcat(lab,reco->data.artist);
    reco=reco->next;
    gtk_label_set_text(ui->rec_label1, lab);
    gtk_label_set_justify(ui->rec_label1, GTK_JUSTIFY_CENTER);
    strcpy(lab,mt);

    strcat(lab,reco->data.songname);
    strcat(lab,"\n");
    strcat(lab,reco->data.artist);
    reco=reco->next;
    gtk_label_set_text(ui->rec_label2, lab);
    gtk_label_set_justify(ui->rec_label2, GTK_JUSTIFY_CENTER);
    strcpy(lab,mt);

    strcat(lab,reco->data.songname);
    strcat(lab,"\n");
    strcat(lab,reco->data.artist);
    reco=reco->next;
    gtk_label_set_text(ui->rec_label3, lab);
    gtk_label_set_justify(ui->rec_label3, GTK_JUSTIFY_CENTER);
    strcpy(lab,mt);





    gtk_widget_hide(GTK_WIDGET(ui->rec));
    gtk_widget_show(GTK_WIDGET(ui->rec));
    gtk_widget_hide(GTK_WIDGET(ui->similar_songs));
    gtk_window_resize(ui->window, 10,10);
    gtk_window_set_position(ui->window, GTK_WIN_POS_CENTER);

    gtk_widget_hide(GTK_WIDGET(ui->window));
    gtk_window_set_position(ui->window, GTK_WIN_POS_CENTER);
    gtk_widget_show(GTK_WIDGET(ui->window));
}

void on_song_found(GtkButton *b, gpointer user)
{
    UserInterface* ui =user;
    gtk_button_set_use_underline(b, TRUE);

    gtk_widget_hide(GTK_WIDGET(ui->similar_songs));
    gtk_widget_hide(GTK_WIDGET(ui->rec));
    gtk_widget_show(GTK_WIDGET(ui->positive_search));
    gtk_window_resize(ui->window, 10,10);
    gtk_window_set_position(ui->window, GTK_WIN_POS_CENTER);

    gtk_widget_hide(GTK_WIDGET(ui->window));
    gtk_window_set_position(ui->window, GTK_WIN_POS_CENTER);
    gtk_widget_show(GTK_WIDGET(ui->window));
}

void on_ss_popup(GtkButton* b, gpointer user)
{
    UserInterface* ui = user;
    gtk_button_set_use_underline(b, TRUE);

    gtk_widget_hide(GTK_WIDGET(ui->rec));
    gtk_widget_show(GTK_WIDGET(ui->similar_songs));
    gtk_window_resize(ui->window, 10,10);
    gtk_window_set_position(ui->window, GTK_WIN_POS_CENTER);

    gtk_widget_hide(GTK_WIDGET(ui->window));
    gtk_window_set_position(ui->window, GTK_WIN_POS_CENTER);
    gtk_widget_show(GTK_WIDGET(ui->window));
}

void rec_on_play1(GtkButton*b, gpointer user)
{
    UserInterface* ui = user;
    gtk_button_set_use_underline(b, TRUE);
    char command[400] = "";
    struct Database* ss = ui->reco->next;
    strcat(command, "play ");
    strcat(command, ss->data.FileName);
    strcat(command," trim 00:30 0:05");
    system(command);
}

void rec_on_play2(GtkButton*b, gpointer user)
{
    UserInterface* ui = user;
    gtk_button_set_use_underline(b, TRUE);
    char command[400] = "";
    struct Database* ss = ui->reco->next->next;
    strcat(command, "play ");
    strcat(command, ss->data.FileName);
    strcat(command," trim 00:30 0:05");
    system(command);
}

void rec_on_play3(GtkButton*b, gpointer user)
{
    UserInterface* ui = user;
    gtk_button_set_use_underline(b, TRUE);
    char command[400] = "";
    struct Database* ss = ui->reco->next->next->next;
    strcat(command, "play ");
    strcat(command, ss->data.FileName);
    strcat(command," trim 00:30 0:05");
    system(command);
}

void ss_on_play1(GtkButton*b,gpointer user)
{
    UserInterface* ui = user;
    gtk_button_set_use_underline(b, TRUE);
    char command[400] = "";
    struct Database* ss= ui->similar->next;
    strcat(command, "play ");
    strcat(command, ss->data.FileName);
    strcat(command," trim 00:30 0:05");
    system(command);
}

void ss_on_play2(GtkButton*b,gpointer user)
{
    UserInterface* ui = user;
    gtk_button_set_use_underline(b, TRUE);
    char command[400] = "";
    struct Database* ss= ui->similar->next->next;
    strcat(command,"play ");
    strcat(command,ss->data.FileName);
    strcat(command," trim 00:30 0:05");
    system(command);
}

void ss_on_play3(GtkButton*b,gpointer user)
{
    UserInterface* ui = user;
    gtk_button_set_use_underline(b, TRUE);
    char command[400] = "";
    struct Database* ss=ui->similar->next->next->next;
    strcat(command,"play ");
    strcat(command,ss->data.FileName);
    strcat(command," trim 00:30 0:05");
    system(command);
}

void ss_on_play4(GtkButton*b, gpointer user)
{
    UserInterface* ui = user;
    gtk_button_set_use_underline(b, TRUE);
    struct Database* ss = ui->similar->next->next->next->next;
    char* empty = "";
    char* command = calloc(100,sizeof(char));
    command = strcpy(command,empty);
    command = strcat(command,"play ");
    command = strcat(command,ss->data.FileName);
    command = strcat(command," trim 00:30 0:05");
    system(command);
    free(command);
}
/*
   void on_play(GtkButton* b, gpointer user)
   {
   UserInterface* ui = user;
   struct Database* ss = ui->similar->next;
   char command[100] = "";
   strcat(command, "play ");
   if(b == ui->ss_play1)
   {
   strcat(command, ss->data.FileName);
   }
   ss=ss->next;
   if(b == ui->ss_play2)
   {
   strcat(command, ss->data.FileName);
   }
   ss=ss->next;
   if(b == ui->ss_play3)
   {
   strcat(command, ss->data.FileName);
   }
   ss=ss->next;
   if(b == ui->ss_play4)
   {
   strcat(command, ss->data.FileName);
   }
   strcat(command," trim 00:30 0:09");
   system(command);
   }*/

void on_play_found(GtkButton* b, gpointer user)
{
    UserInterface* ui = user;
    gtk_button_set_use_underline(b, TRUE);
    char command[100] = "";
    strcat(command,"play ");
    strcat(command,ui->song->data.FileName);
    strcat(command, " trim 00:30 0:09");
    system(command);
    system("clear");
}

void rec_on_add1(GtkButton* b, gpointer user)
{
    UserInterface* ui = user;
    gtk_button_set_use_underline(b, TRUE);
    struct Database* song = ui->DATABASE->next;
    for(;song && strcmp(song->data.songname,ui->reco->next->data.songname) != 0; song=song->next){}
    if(song->data.favorite == 0)
        song->data.favorite = 1;
    else
        song->data.favorite = 0;
}

void rec_on_add2(GtkButton* b, gpointer user)
{
    UserInterface* ui = user;
    gtk_button_set_use_underline(b, TRUE);
    struct Database* ss = ui->reco->next->next;
    struct Database* song = ui->DATABASE->next;
    for(;song && strcmp(song->data.songname,ss->data.songname) != 0; song=song->next){}
    if(song->data.favorite == 0)
        song->data.favorite = 1;
    else
        song->data.favorite = 0;
}
void rec_on_add3(GtkButton* b, gpointer user)
{
    UserInterface* ui = user;
    gtk_button_set_use_underline(b, TRUE);
    struct Database* ss = ui->reco->next->next->next;
    struct Database* song = ui->DATABASE->next;
    for(;song && strcmp(song->data.songname,ss->data.songname) != 0; song=song->next){}
    if(song->data.favorite == 0)
        song->data.favorite = 1;
    else
        song->data.favorite = 0;
}


void ss_on_add1(GtkButton* b, gpointer user)
{
    UserInterface* ui = user;
    gtk_button_set_use_underline(b, TRUE);
    struct Database* song = ui->DATABASE->next;
    for(;song && strcmp(song->data.songname,ui->similar->next->data.songname) != 0; song=song->next){}
    if(song->data.favorite == 0)
        song->data.favorite = 1;
    else
        song->data.favorite = 0;
}

void ss_on_add2(GtkButton* b, gpointer user)
{
    UserInterface* ui = user;
    gtk_button_set_use_underline(b, TRUE);
    struct Database* ss = ui->similar->next->next;
    struct Database* song = ui->DATABASE->next;
    for(;song && strcmp(song->data.songname,ss->data.songname) != 0; song=song->next){}
    if(song->data.favorite == 0)
        song->data.favorite = 1;
    else
        song->data.favorite = 0;
}

void ss_on_add3(GtkButton* b, gpointer user)
{
    UserInterface* ui = user;
    gtk_button_set_use_underline(b, TRUE);
    struct Database* ss = ui->similar->next->next->next;
    struct Database* song = ui->DATABASE->next;
    for(;song && strcmp(song->data.songname,ss->data.songname) != 0; song=song->next){}
    if(song->data.favorite == 0)
        song->data.favorite = 1;
    else
        song->data.favorite = 0;
}

void ss_on_add4(GtkButton* b, gpointer user)
{
    UserInterface* ui = user;
    gtk_button_set_use_underline(b, TRUE);
    struct Database* ss = ui->similar->next->next->next->next;
    struct Database* song = ui->DATABASE->next;
    for(;song && strcmp(song->data.songname,ss->data.songname) != 0; song=song->next){}
    if(song->data.favorite == 0)
        song->data.favorite = 1;
    else
        song->data.favorite = 0;
}


void on_add_fav(GtkButton* b, gpointer user)
{
    UserInterface* ui = user;
    gtk_button_set_use_underline(b, TRUE);
    if(ui->song->data.favorite == 0)
    {
        ui->song->data.favorite = 1;
        gtk_label_set_text((ui->add_lab), "remove fav");
        gtk_widget_show(GTK_WIDGET(ui->positive_search));
    }
    else
    {
        ui->song->data.favorite = 0;
        gtk_label_set_text((ui->add_lab), "add fav");
        gtk_widget_show(GTK_WIDGET(ui->positive_search));
    }
}

int main(int argc, char* argv[])
{
/*
char* command = calloc(100,sizeof(char));
      char* pref = "ffmpeg -i Database/";
      char* mid = " -ab 320k data/";
      char* suff = ".wav";
      size_t x = 0;
      size_t y = 0;
      size_t z = 0;
      char* empty = "";
      for(size_t i  = 1000; i <= 1199; i++)
      {
      y = i % 10;
      x = ((i-y) % 100)/10;
      z = (i%1000)/100;
      //printf("i = %ld, i mod 1000 = %ld", i, z);
      char name[] = {'1',48 + (char)(z),48 + (char)(x),48 + (char)(y),0};
      command = strcat(command,pref);
      command = strcat(command,name);
      command = strcat(command,".mp3");
      command = strcat(command,mid);
      command = strcat(command,name);
      command = strcat(command,suff);
      printf("%s %ld\n",name,z);
      system(command);
      command = strcpy(command,empty);
      }
  */
    gtk_init(&argc,&argv);

    GtkBuilder* builder = gtk_builder_new();

    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "in.glade", &error)==0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    //Window
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));

    //Stack
    GtkStack* stack = GTK_STACK(gtk_builder_get_object(builder, "stack"));

    //Grid
    GtkBox* principal = GTK_BOX(gtk_builder_get_object(builder,"principal"));
    GtkGrid* rec = GTK_GRID(gtk_builder_get_object(builder,"rec"));
    GtkGrid* similar_songs = GTK_GRID(gtk_builder_get_object(builder,"similar_songs"));
    GtkGrid* positive_search = GTK_GRID(gtk_builder_get_object(builder,"positive_search"));
    GtkGrid* fav = GTK_GRID(gtk_builder_get_object(builder,"fav"));

    //Label
    GtkLabel *fav_list_lab = GTK_LABEL(gtk_builder_get_object(builder,"fav_list_lab"));
    GtkLabel* name_of_song = GTK_LABEL(gtk_builder_get_object(builder,"name_of_song"));
    GtkLabel* artist_name = GTK_LABEL(gtk_builder_get_object(builder,"artist_name"));
    GtkLabel *similar_song = GTK_LABEL(gtk_builder_get_object(builder,"similar_song"));
    GtkLabel *rec_label = GTK_LABEL(gtk_builder_get_object(builder,"rec_label"));
    GtkLabel *add_lab = GTK_LABEL(gtk_builder_get_object(builder,"add_lab"));
    GtkLabel* ss_un = GTK_LABEL(gtk_builder_get_object(builder,"ss_un"));
    GtkLabel* ss_deux = GTK_LABEL(gtk_builder_get_object(builder,"ss_deux"));
    GtkLabel* ss_trois = GTK_LABEL(gtk_builder_get_object(builder,"ss_trois"));
    GtkLabel* ss_quatre = GTK_LABEL(gtk_builder_get_object(builder,"ss_quatre"));

    GtkLabel* rec_label1 = GTK_LABEL(gtk_builder_get_object(builder,"rec_label1"));
    GtkLabel* rec_label2 = GTK_LABEL(gtk_builder_get_object(builder,"rec_label2"));
    GtkLabel* rec_label3 = GTK_LABEL(gtk_builder_get_object(builder,"rec_label3"));


    //Button Quit
    GtkButton* quit = GTK_BUTTON(gtk_builder_get_object(builder,"quit"));
    GtkButton* quit2 = GTK_BUTTON(gtk_builder_get_object(builder,"quit2"));
    GtkButton* quit3 = GTK_BUTTON(gtk_builder_get_object(builder,"quit3"));
    GtkButton* quit4 = GTK_BUTTON(gtk_builder_get_object(builder,"quit4"));
    GtkButton* quit5 = GTK_BUTTON(gtk_builder_get_object(builder,"quit5"));

    //Transition Button
    GtkButton* fav_popup = GTK_BUTTON(gtk_builder_get_object(builder,"fav_popup"));
    GtkButton* back_rec = GTK_BUTTON(gtk_builder_get_object(builder,"back_rec"));
    GtkButton* back_sim = GTK_BUTTON(gtk_builder_get_object(builder, "back_sim"));
    GtkButton* back_fav = GTK_BUTTON(gtk_builder_get_object(builder,"back_fav"));
    GtkButton* back_ps = GTK_BUTTON(gtk_builder_get_object(builder,"back_ps"));
    GtkButton* record = GTK_BUTTON(gtk_builder_get_object(builder,"record"));
    GtkButton* more = GTK_BUTTON(gtk_builder_get_object(builder,"more"));
    GtkButton* rec_popup = GTK_BUTTON(gtk_builder_get_object(builder,"rec_popup"));
    GtkButton* song_found = GTK_BUTTON(gtk_builder_get_object(builder,"song_found"));
    GtkButton* song_found2 = GTK_BUTTON(gtk_builder_get_object(builder, "song_found2"));
    GtkButton* ss_popup = GTK_BUTTON(gtk_builder_get_object(builder, "ss_popup"));

    //play buttons
    GtkButton* play_found = GTK_BUTTON(gtk_builder_get_object(builder,"play_found"));
    GtkButton* ss_play1 = GTK_BUTTON(gtk_builder_get_object(builder,"ss_play1"));
    GtkButton* ss_play2 = GTK_BUTTON(gtk_builder_get_object(builder,"ss_play2"));
    GtkButton* ss_play3 = GTK_BUTTON(gtk_builder_get_object(builder,"ss_play3"));
    GtkButton* ss_play4 = GTK_BUTTON(gtk_builder_get_object(builder,"ss_play4"));

    GtkButton* rec_play1 = GTK_BUTTON(gtk_builder_get_object(builder,"rec_play1"));
    GtkButton* rec_play2 = GTK_BUTTON(gtk_builder_get_object(builder,"rec_play2"));
    GtkButton* rec_play3 = GTK_BUTTON(gtk_builder_get_object(builder,"rec_play3"));


    //add fav buttons
    GtkButton* add_fav = GTK_BUTTON(gtk_builder_get_object(builder, "add_fav"));
    GtkButton* ss_add_fav1 = GTK_BUTTON(gtk_builder_get_object(builder,"ss_add_fav1"));
    GtkButton* ss_add_fav2 = GTK_BUTTON(gtk_builder_get_object(builder,"ss_add_fav2"));
    GtkButton* ss_add_fav3 = GTK_BUTTON(gtk_builder_get_object(builder,"ss_add_fav3"));
    GtkButton* ss_add_fav4 = GTK_BUTTON(gtk_builder_get_object(builder,"ss_add_fav4"));

    GtkButton* rec_add_fav1 = GTK_BUTTON(gtk_builder_get_object(builder,"rec_add_fav1"));
    GtkButton* rec_add_fav2 = GTK_BUTTON(gtk_builder_get_object(builder,"rec_add_fav2"));
    GtkButton* rec_add_fav3 = GTK_BUTTON(gtk_builder_get_object(builder,"rec_add_fav3"));

    GtkButton* refresh_reco = GTK_BUTTON(gtk_builder_get_object(builder,"refresh_reco"));
    struct Database *DATABASE = RecoverDatabase();

    //Creating the pointer
    UserInterface ui =
    {
        .builder = builder,
        .window = window,
        .stack = stack,
        .principal = principal,
        .rec = rec,
        .similar_songs = similar_songs,
        .positive_search = positive_search,
        .fav = fav,

        .DATABASE = DATABASE,
        .song = NULL,
        .similar = calloc(1,sizeof(struct Database)),
        .reco = calloc(1,sizeof(struct Database)),

        .name_of_song = name_of_song,
        .artist_name = artist_name,
        .fav_list_lab = fav_list_lab,
        .similar_song = similar_song,
        .rec_label = rec_label,
        .add_lab = add_lab,
        .ss_un = ss_un,
        .ss_deux = ss_deux,
        .ss_trois = ss_trois,
        .ss_quatre = ss_quatre,
        .rec_label1 = rec_label1,
        .rec_label2 = rec_label2,
        .rec_label3 = rec_label3,

        .ss_add_fav1 = ss_add_fav1,
        .ss_add_fav2 = ss_add_fav2,
        .ss_add_fav3 = ss_add_fav3,
        .ss_add_fav4 = ss_add_fav4,

        .ss_play1 = ss_play1,
        .ss_play2 = ss_play2,
        .ss_play3 = ss_play3,
        .ss_play4 = ss_play4,

        .nsong = 0,
    };



    //Connect destroy
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(quit, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(quit2, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(quit3, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(quit4, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(quit5, "clicked", G_CALLBACK(gtk_main_quit), NULL);

    //Connect transition between pages
    g_signal_connect(fav_popup, "clicked", G_CALLBACK(on_fav_popup),&ui);
    g_signal_connect(back_rec, "clicked", G_CALLBACK(on_back),&ui);
    g_signal_connect(back_sim, "clicked", G_CALLBACK(on_back),&ui);
    g_signal_connect(back_ps, "clicked", G_CALLBACK(on_back),&ui);
    g_signal_connect(back_fav, "clicked", G_CALLBACK(on_back),&ui);
    g_signal_connect(record, "clicked", G_CALLBACK(on_rec),&ui);
    g_signal_connect(more, "clicked", G_CALLBACK(on_m_clicked),&ui);
    g_signal_connect(rec_popup, "clicked", G_CALLBACK(on_rec_popup),&ui);
    g_signal_connect(song_found,"clicked",G_CALLBACK(on_song_found),&ui);
    g_signal_connect(song_found2,"clicked",G_CALLBACK(on_song_found),&ui);
    g_signal_connect(ss_popup,"clicked",G_CALLBACK(on_ss_popup),&ui);
    //Play Buttons
    g_signal_connect(play_found,"clicked",G_CALLBACK(on_play_found),&ui);
    g_signal_connect(ss_play1,"clicked",G_CALLBACK(ss_on_play1),&ui);
    g_signal_connect(ss_play2,"clicked",G_CALLBACK(ss_on_play2),&ui);
    g_signal_connect(ss_play3,"clicked",G_CALLBACK(ss_on_play3),&ui);
    g_signal_connect(ss_play4,"clicked",G_CALLBACK(ss_on_play4),&ui);

    g_signal_connect(rec_play1,"clicked",G_CALLBACK(rec_on_play1),&ui);
    g_signal_connect(rec_play2,"clicked",G_CALLBACK(rec_on_play2),&ui);
    g_signal_connect(rec_play3,"clicked",G_CALLBACK(rec_on_play3),&ui);

    //Add_fav buttons
    g_signal_connect(add_fav, "clicked", G_CALLBACK(on_add_fav), &ui);
    g_signal_connect(ss_add_fav1,"clicked",G_CALLBACK(ss_on_add1),&ui);
    g_signal_connect(ss_add_fav2,"clicked",G_CALLBACK(ss_on_add2),&ui);
    g_signal_connect(ss_add_fav3,"clicked",G_CALLBACK(ss_on_add3),&ui);
    g_signal_connect(ss_add_fav4,"clicked",G_CALLBACK(ss_on_add4),&ui);

    g_signal_connect(rec_add_fav1,"clicked",G_CALLBACK(rec_on_add1),&ui);
    g_signal_connect(rec_add_fav2,"clicked",G_CALLBACK(rec_on_add2),&ui);
    g_signal_connect(rec_add_fav3,"clicked",G_CALLBACK(rec_on_add3),&ui);

    g_signal_connect(refresh_reco,"clicked",G_CALLBACK(on_refresh),&ui);

    gtk_widget_show(GTK_WIDGET(window));
    gtk_widget_hide(GTK_WIDGET(fav));
    gtk_widget_hide(GTK_WIDGET(rec));
    gtk_widget_hide(GTK_WIDGET(similar_songs));
    gtk_widget_hide(GTK_WIDGET(positive_search));

    for(struct Database* s = DATABASE; s; s=s->next)
        printf("%d %s \n",s->data.genre, s->data.songname);

    gtk_main();
    system("clear");
}
