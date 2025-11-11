#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stddef.h>

//typedef struct {
    //size_t tableSize;
    //int    runMagic;
    //int    huffman[256];   // 256 different bytes possible
//} blockMeta;

size_t Huffman( symbol_t *input,
                unsigned char *output,
//                unsigned char *output2,
                size_t len,
                blockMeta *meta );
                //int round );

#endif
