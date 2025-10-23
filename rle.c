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

    symbol_t symbol;
    int size = 0;
    size_t i = 0;

    while( i < len ) {

        int zeroRun = -1;
        symbol = ( symbol_t )input[i];
        output[size] = symbol;
        size++;
        printf( "element: %d\n", input[i] );

        do {
            zeroRun += ( input[i] == 0 ) ? 1 : 0;
        } while( input[++i] == 0 && i < len );
        
        if( zeroRun > -1 ) {
            int mod = zeroRun % 2;
            printf( "zeroRun: %d| ",  zeroRun );
            printf( "mod: %d\n", mod );
            output[size] = ( mod ) ? RUNB : RUNA;
            size++;
            while( zeroRun > 1 ) {
                zeroRun = zeroRun / 2;
                mod = zeroRun % 2;
                printf( "zeroRun: %d| ",  zeroRun );
                printf( "mod: %d\n", mod );
                output[size] = ( mod ) ? RUNB : RUNA;
                size++;
            }
        }
    }

    printf( "\n" );
    for( i = 0; i < size; i++ ) {
        printf( "%d ", output[i] );
    }
    
    printf( "\n\n" );

    return size;
}
