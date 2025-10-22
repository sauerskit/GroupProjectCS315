#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"
#include "rle.h"

#define RUNA 256
#define RUNB 257

size_t RLE( const unsigned char *input,
                  symbol_t      *output,
                  size_t        len ) {

    printf( "RLE\n\n" );

    size_t size = 0;

    for( size_t i = 0; i < len; i++ ) {
        //output[i] = (symbol_t)input[i];
    }

    return size;
}
