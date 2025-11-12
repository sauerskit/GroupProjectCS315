#ifndef UNHUFFMAN_H
#define UNHUFFMAN_H

#include <stddef.h>

//typedef struct {
    //size_t tableSize;
    //int    runMagic;
    //int    huffman[256];   // 256 different bytes possible
//} blockMeta;

size_t Unhuffman( unsigned char *input,
                  symbol_t *output,
                  //size_t len,
                  blockMeta *meta,
                  FILE *inputFile );

#endif
