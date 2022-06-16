#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "fft.h"
#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"


int16_t *wavRead_int16(char *filename, uint32_t *sampleRate, uint64_t *totalSampleCount) {
    unsigned int channels;
    int16_t *buffer = drwav_open_and_read_file_s16(filename, &channels, sampleRate, totalSampleCount);
    if (buffer == 0) {
        fprintf(stderr, "ERROR\n");
        exit(1);
    }
    if (channels == 2) {
        int16_t *bufferSave = buffer;
        for (uint64_t i = 0; i < *totalSampleCount; i += 2) {
            *bufferSave++ = (int16_t) ((buffer[i] + buffer[i + 1]) >> 1);
        }
        *totalSampleCount = *totalSampleCount >> 1;
    }
    return buffer;
}


unsigned long hash(unsigned char *str) {
    unsigned long hash = 5381;
    int c;
    c = *str++;
    while (c)
    {
        hash = ((hash << 5) + hash) + c;
        c = *str++;
    }
    return hash;
}

int generateHashes(char *input_file, int **hashtable, int songid, size_t N, int freqbandWidth, int maxElems) {
    printf("reading %s \n", input_file);
    uint32_t sampleRate = 0;
    uint64_t samplesize = 0;
    int16_t *pcmdata = wavRead_int16(input_file, &sampleRate, &samplesize);
    float *inputBuffer = (float *) calloc(sizeof(float), N);
    fft_complex *outBuffer = (fft_complex *) calloc(sizeof(fft_complex), N);
    size_t sect = 0;
    int cnt = 0;
    int numHashes = 0;
    for (uint64_t i = 0; i < samplesize; i++)
    {
        if (sect < N)
        {
            inputBuffer[sect] = (float) pcmdata[i];
            sect++;
        }
        else
        {
            sect = 0;
            i -= 1;
            cnt++;
            fft_plan plan = fft_plan_dft_r2c_1d(N, inputBuffer, outBuffer, 0);
            fft_execute(plan);
            fft_destroy_plan(plan);
            int freq1 = 0, freq2 = 0, freq3 = 0, freq4 = 0, freq5 = 0;
            int pt1 = 0, pt2 = 0, pt3 = 0, pt4 = 0, pt5 = 0;
            int freqbandWidth2 = freqbandWidth * 2;
            int freqbandWidth3 = freqbandWidth * 3;
            int freqbandWidth4 = freqbandWidth * 4;
            int freqbandWidth5 = freqbandWidth * 5;
            int freqbandWidth6 = freqbandWidth * 6;
            for (int k = freqbandWidth; k < freqbandWidth6; k++)
            {
                int freq = (outBuffer[k].real > 0) ? (int) outBuffer[k].real : (int) (0 - outBuffer[k].real);
                int Magnitude = (int) (log10f((freq + 1)) * 1000);
                if (k >= freqbandWidth && k < freqbandWidth2 && Magnitude > freq1)
                {
                    freq1 = Magnitude;
                    pt1 = k;
                }
                else if (k >= freqbandWidth2 && k < freqbandWidth3 && Magnitude > freq2)
                {
                    freq2 = Magnitude;
                    pt2 = k;
                }
                else if (k >= freqbandWidth3 && k < freqbandWidth4 && Magnitude > freq3)
                {
                    freq3 = Magnitude;
                    pt3 = k;
                }
                else if (k >= freqbandWidth4 && k < freqbandWidth5 && Magnitude > freq4)
                {
                    freq4 = Magnitude;
                    pt4 = k;
                }
                else if (k >= freqbandWidth5 && k < freqbandWidth6 && Magnitude > freq5)
                {
                    freq5 = Magnitude;
                    pt5 = k;
                }
            }
            unsigned char buffer[60];
            sprintf((char*)buffer, "%d%d%d%d%d", pt1, pt2, pt3, pt4, pt5);
            unsigned long hashresult = hash(buffer) % maxElems;
            int key = (int) hashresult;
            if (key < 0)
                printf("Invalid key %d\n", key);
            hashtable[key][songid]++;
            numHashes++;
        }
    }
    free(pcmdata);
    free(inputBuffer);
    free(outBuffer);
    return numHashes;
}

int main(int argc, char *argv[]) {
    printf("Audio Processing\n");
    printf("shazam audio hash\n");
    int N = 512;
    int freqbandWidth = 50;
    long unsigned int maxSongs = 200;
    size_t maxElems = 4999733;
    int **hashTable;
    long unsigned int i = 0, n = 0;
    float count = 0;
    long unsigned int numsongs = 0;
    char filenames[maxSongs + 1][30];
    int filesizes[maxSongs + 1];
    int songScores[maxSongs + 1];
    float songMatch[maxSongs + 1];
    printf("running... \n");
    if (argc < 2) {
        printf("no excerpt file to open \n");
        exit(1);
    }
    hashTable = (int **) calloc(maxElems, sizeof(int *));
    for (i = 0; i < maxElems; i++)
        hashTable[i] = (int *) calloc(maxSongs + 1, sizeof(int));
    printf("Generating hashes for original files.. \n");


    char* file = calloc(100, sizeof(char));
    char* directory = "data/";
    char* extension = ".wav";
    char* empty = "";
    size_t x = 0;
    size_t y = 0;
    size_t z = 0;

    for(size_t i = 1000; i<1200; i++) //change when data base grosse
    {
        y = i%10;
        x = ((i-y)%100)/10;
        z = (i%1000)/100;
        file = strcpy(file,empty);
        char name[] = {'1',48+(char)(z),48 + (char)(x),48 + (char)(y),0};
        file = strcat(file, directory);
        file = strcat(file,name);
        file = strcat(file,extension);

        numsongs++;
        filesizes[numsongs] = generateHashes(file,hashTable,numsongs, N, freqbandWidth, maxElems);
        songScores[numsongs] = 0;
        printf("%ld:%d hashes for %s\n", numsongs, filesizes[numsongs],
                file);
        strcpy(filenames[numsongs], name);
    }

/*
    //ici
    //SAVING HASH
    FILE* save = fopen("audio/test/hash.txt","w");
    for(size_t x = 0; x < maxElems; x++)
    {
        for(long unsigned int y = 0; y<maxSongs+1; y++)
        {
            if(hashTable[x][y]>0)
            {
                fprintf(save,"%ld\n%ld\n%d\n",x,y,hashTable[x][y]);
            }
        }
    }
    fclose(save);

    FILE* save2 = fopen("audio/test/filesizes.txt","w");
    for(size_t x = 0; x<numsongs+1; x++)
    {
        fprintf(save2,"%d\n",filesizes[x]);
    }
    fclose(save2);
*/

/*
    //READING HASH
    FILE* save = fopen("audio/test/hash.txt","r");
    //save = fopen("audio/test/hash.txt","r");
    if(save!=NULL)
    {
        char* redx = calloc(10,sizeof(char));
        char* redy = calloc(10,sizeof(char));
        char* redz = calloc(10,sizeof(char));
        int x = 0;
        int y = 0;
        int z = 0;
        while(fgets(redx,10,save))
        {
            fgets(redy,10,save);
            fgets(redz,10,save);
            x = atoi(redx);
            y = atoi(redy);
            z = atoi(redz);
            hashTable[x][y] = z;
            printf("%d %d %d\n",x,y,z);
        }
    }
    fclose(save);

    FILE* save2 = fopen("audio/test/filesizes.txt","r");
    int xsave2 = 0;
    char* red = calloc(10,sizeof(char));
    if(save2!=NULL)
    {
        while(fgets(red,10,save2))
        {
            filesizes[xsave2] = atoi(red);
            xsave2++;
        }
    }
    fclose(save2);*/

    printf("Generating hashes for recorded file.. \n");
    generateHashes(argv[1], hashTable, 0, N, freqbandWidth, maxElems);
    printf("Calculating score.. \n");
    for (i = 0; i < maxElems; i++) {
        if (hashTable[i][0] > 0) {
            for (n = 1; n <= maxSongs; n++) {
                if (hashTable[i][n] >= hashTable[i][0])
                    songScores[n] = songScores[n] + hashTable[i][0];
                else
                    songScores[n] = songScores[n] + hashTable[i][n];;
            }
        }
    }
    float count_5 = 0;
    int n_5 = 0,n_4 = 0 ,n_3= 0 ,n_2 = 0;
    float count_4 = 0;
    float count_3 = 0;
    float count_2 = 0;
    for (i = 1; i <= numsongs; i++) {
        songMatch[i] = ((float) songScores[i]) / ((float) filesizes[i]);
        printf("Score for %s = %f\n", filenames[i], songMatch[i]);
        if (songMatch[i] > count_5) {
            if(songMatch[i] > count_4)
            {
                if(songMatch[i] > count_3)
                {
                    if(songMatch[i] > count_2)
                    {
                        if(songMatch[i] > count)
                        {
                            count = songMatch[i];
                            n_2 = n;
                            n = i;
                            continue;
                        }
                        count_2 = songMatch[i];
                        n_3 = n_2;
                        n_2 = i;
                        continue;
                    }
                    count_3 = songMatch[i];
                    n_4 = n_3;
                    n_3 = i;
                    continue;
                }
                count_4 = songMatch[i];
                n_5 = n_4;
                n_4 = i;
                continue;
            }
        }
    }
    printf("Best Score: %s\n", filenames[n]);
    printf("%i\n",n_5);
    printf("Second: %s\n",filenames[n_2]);
    printf("Third: %s\n",filenames[n_3]);
    printf("Fourth: %s\n",filenames[n_4]);
    printf("Fifth: %s\n",filenames[n_5]);
    FILE *strea = fopen("song.txt","w");
    fprintf(strea, "%s\n",filenames[n]);
    fclose(strea);

    FILE *stream = fopen("similar.txt","w");
    fprintf(stream,"%s\n%s\n%s\n%s\n",filenames[n_2],
            filenames[n_3],filenames[n_4],filenames[n_5]);
    fclose(stream);

    for (i = 0; i < maxElems; i++)
        free(hashTable[i]);
    free(hashTable);


    return 0;
}
