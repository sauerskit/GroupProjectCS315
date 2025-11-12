#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"
#include "rle.h"

#define RUNA 256
#define RUNB 257

size_t UnRLE(       unsigned char *output,
                    symbol_t      *input,
                    size_t        len ) {


    //printf( "\n\nUnRLE\n\n" );

    //printf( "len=%zu ", len );

    int idx = 0;
    int zeroRun = 0;
    int done = 0;

    for( int i = 0; i < len; i++ ) {

        if( input[i] < RUNA ) {
            output[idx] = ( const char )input[i];
            //printf( "%d ", input[i] );
            idx++;

        } else if( input[i] >= RUNA ) {

            int exp = 1;
            int zeroRun = 1;

            do {
                if( input[i] == 258 ) {
                    //printf( "DONE!\n" );
                    done = 1;
                }

                if( input[i] < 258 ) {
                    zeroRun += exp * ( ( input[i] == 257 ) ? 1 : 0 );
                    exp = exp * 2;
                    i++;
                }
            } while ( ( input[i] >= RUNA ) && ( input[i] <= RUNB ) );

            for( int j = 0; j < zeroRun; j++ ) {
                output[idx] = 0;
                //printf( "0 " );
                idx++;
            }
            if( i < len ) {
                //printf( "|%d ", input[i] );
                output[idx] = input[i];
                idx++;
            }
        }
    }

    //printf( "\n" );
    return idx;
}



size_t RLE( const unsigned char *input,
                  symbol_t      *output,
                  size_t        len ) {

    //printf( "RLE\n\n" );

    symbol_t symbol;
    int size = 0;
    size_t i = 0;

    while( i < len ) {

        int zeroRun = -1;
        symbol = ( symbol_t )input[i];
        output[size] = symbol;
        size++;

        do {
            zeroRun += ( input[i] == 0 ) ? 1 : 0;
        } while( input[++i] == 0 && i < len );
        
        if( zeroRun > -1 ) {
            int mod = zeroRun % 2;
            output[size] = ( mod ) ? RUNB : RUNA;
            size++;
            while( zeroRun > 1 ) {
                zeroRun = zeroRun / 2;
                mod = zeroRun % 2;
                output[size] = ( mod ) ? RUNB : RUNA;
                size++;
            }
        }
    }

    output[size] = 258;
    size++;

    //printf( "\n" );
    for( i = 0; i < size; i++ ) {
        //printf( "%d ", output[i] );
    }
    
    //printf( "size: %d\n\n", size );

    return size;
}
