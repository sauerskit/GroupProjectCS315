#ifndef COMPRESS_H
#define COMPRESS_H

#include <stdint.h>

typedef uint16_t symbol_t;

typedef struct {
    size_t primaryIndex; //3 bytes to store
    size_t tableSize;
    int    runMagic;
    int    huffman[259];
//    int    huffman2[259];
} blockMeta;

#endif
