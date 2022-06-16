#ifndef SHAZAM_H
#define SHAZAM_H

int16_t *wavRead_int16(char *filename, uint32_t *sampleRate, uint64_t *totalSampleCount);
unsigned long hash(unsigned char *str);
int generateHashes(char *input_file, int **hashtable, int songid, size_t N, int freqbandWidth, int maxElems);

#endif
