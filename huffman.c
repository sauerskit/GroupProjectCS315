#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"
#include "huffman.h"

// blockMeta struct
// tableSize       table size (shocking)
// runMagic         magic bit for encoding runa or runb first
// huffman[259]        huffman table

unsigned char intToByte( int *buffer ) {

    unsigned char byteToAdd = 0;

    int exp = 1; //exponent ( powers of 2 ) 
    for( int k = 7; k >= 0; k-- ) {
        //printf( " k = %d ", k );
        byteToAdd += buffer[k] * exp;
        exp = exp * 2;
        //printf( " byte = %zu ", byteToAdd );
    }

    return byteToAdd;
}

int metaData( blockMeta *meta ) {



    return 1;
}

int write( symbol_t *input, size_t len, blockMeta *meta ) {

    printf( "\nwrite start.\n" );

    int *buffer = malloc( 300 * sizeof( int ) );
    int idx = 0;

    FILE *outputFile = fopen( "output.zip", "ab" );
    if( outputFile ) {
        printf( "File output.zip opened for writing.\n" );
    }

    if( !outputFile ) {
        perror( "fopen" );
        exit( 1 );
    }

    fseek( outputFile, 0, SEEK_END );
    long pos = ftell( outputFile );
    printf( "File position before writing: %ld\n", pos );

    for( int i = 0; i < len; i++ ) {
        int j = 0;
        while( input[i] != meta->huffman[j] ) {
            //printf( "0" );
            buffer[idx] = 0;
            //printf( "\nbuffer at %d: %d\n", idx, buffer[idx] );
            idx++;
            j++;
        }
        
        //printf( "1" );
        buffer[idx] = 1;
        //printf( "\nbuffer at %d: %d\n", idx, buffer[idx] );
        idx++;


        while( idx > 7 ) {  // idx = 7 means 8 digits total ( 1 byte )
            unsigned char byteToAdd = intToByte( buffer );

            //printf( " %zu ", byteToAdd );
            fputc( byteToAdd, outputFile );

            for( int k = 8; k < idx; k++ ) {
                buffer[k-8] = buffer[k];
            }

            idx -= 8;
    
        }

    }

    if( idx > 0 ) {
        for( int i = idx; i < 8; i++ ) {
            buffer[i] = 0;
        }

        unsigned char lastByte = intToByte( buffer );
        fputc( lastByte, outputFile );
        printf( "printing last byte\n" );
    }
    
    fclose( outputFile );

    printf( "\nwrite finish.\n" );
    free( buffer );

    printf( "\ntest\n" );
    return 0;
}


size_t Huffman( symbol_t *input,
                unsigned char *output,
               // const unsigned char *output2,
                size_t len,
                blockMeta *meta ) {

    printf( "Huffman\n\n" );

    int temp1[259] = { 0 };
    int temp2[259] = { 0 };
for( size_t i = 0; i < len; i++ ) {
        //meta->huffman[input[i]] += 1;
        temp1[input[i]] += 1;
    }

    printf( "huffman table temp1\n" );
        
    for( int i = 0; i < 259; i++ ) { //not 259 because we leave off endofblock char in encoding so it gets the last huffman code possible
        printf( "%d ", temp1[i] );
    }
    printf( "\n" );

    int hufflen = 1;

    for( int i = 257; i >= 0; i-- ) { //not 258 because we leave off endofblock char in encoding so it gets the last huffman code possible

        if( temp1[i] > 0 ) {
        //printf( "temp1[i]: %d ", temp1[i] );
            int j;
            for( j = 0; ( ( j < hufflen ) && ( temp2[j] >= temp1[i] ) ); j++ );
            //printf( "j: %d\n", j );
            //printf( "hufflen: %d\n", hufflen );
            if( temp2[j] < temp1[i] ) {
            //    printf( "still good\n" );
                for( int k = hufflen - 1; k >= j; k-- ) {
             //       printf( "good %d ", k );
                    
                    temp2[k+1] = temp2[k];
                    meta->huffman[k+1] = meta->huffman[k];
                }
            }
            temp2[j] = temp1[i];
            meta->huffman[j] = i;
            hufflen++;
        }
    }
    
    meta->huffman[hufflen - 1] = 258;

    printf( "\n" );
    printf( "huffman table\n" );
    for( int i = 0; i < hufflen; i++ ) {
        printf( "%d ", meta->huffman[i] );
    }
    printf( "\n" );

    printf( "writing huffman encoded data \n" );

    write( input, len, meta );

    // for addison addition, re-code 0 character runs of length 8, 4, and 16, then huffman encode (second time) those along with patterns of binary 4 bits long.


    printf( "\n" );
    return 0;
}
