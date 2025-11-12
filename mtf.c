#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"
#include "mtf.h"


size_t UnMTF(   unsigned char *output, 
                unsigned char *input, 
                size_t        len ) {

    
    //printf( "\n\nUnMTF\n\n" );

    //printf( "length: %zu\n", len );
    //printf( "length: %zu\n", len );

    unsigned char list[256];
    for( int i = 0; i < 256; i++ ) {
        list[i] = i;
    }

    int symbol;
    for( size_t i = 0; i < len; i++ ) {
        symbol = list[input[i]];
        //printf( "%d ", symbol );
        output[i] = symbol;
        
        for( int j = input[i]; j > 0; j-- ) {
            list[j] = list[j - 1];
        }
        list[0] = symbol;
        
    }

    return len; //TODO RETURN LENGTH
}

size_t MTF( const unsigned char *input, 
               unsigned char *output, 
               size_t        len ) {
    //printf( "MTF\n\n" );

    size_t size = 0;

    unsigned char list[256];
    for( int i = 0; i < 256; i++ ) {
        list[i] = i;
    }

    int index;
    int symbol;
    for( size_t i = 0; i < len; i++ ) {
        for( index = 0; list[index] != input[i]; index++ );
        
        symbol = list[index];
        //printf( "%d ", index );
        output[i] = index;
        size++;

        for( int j = index - 1; j >= 0; j-- ) {
            list[j+1] = list[j];
        }
        list[0] = symbol;

    }

    //printf( "\n\n" );

    return size;
}
