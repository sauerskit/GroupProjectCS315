#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"
#include "huffman.h"

// blockMeta struct
// primaryIndex    ( size_t )
// tableSize       table size (shocking) ( size_t )
// runMagic         magic bit for encoding runa or runb first ( int )
// huffman[259]        huffman table ( int )
unsigned char binToByte( int *buffer ) {

    unsigned char byteToAdd = 0;

    int exp = 1; //exponent ( powers of 2 ) 
    for( int k = 7; k >= 0; k-- ) {
        byteToAdd += buffer[k] * exp;
        exp = exp * 2;
    }

    return byteToAdd;
}

int primaryIndex( size_t idx, FILE *outputFile ) {

        
    //printf( "primary index: \n" );

    unsigned char bytes[3];

    bytes[0] = ( idx >> 16 ) & 0xff;
    bytes[1] = ( idx >> 8  ) & 0xff;
    bytes[2] =   idx         & 0xff;

    for( int i = 0; i < 3; i++ ) {
        fputc( bytes[i], outputFile );
        //printf( " %zu ", bytes[i] );
    }

    //printf( "\n" );
    return 0;
}

int tableSizeMeta( size_t size, FILE *outputFile ) {

    //printf( "huffman table size: \nsize: %d\n", size );

    unsigned char byte = (unsigned char)size;
    fputc( byte, outputFile );
    //printf( " %zu ", byte );

    return 0;
}

int huffmanMeta( int huffman[259], size_t size, FILE *outputFile ) {

    //printf( "\nhuffman tree metadata\n" );

    for( int i = 0; i < 259; i++ ) {
        int found = 0;
        for( size_t j = 0; ( j < size ); j++ ) {
            if( i == huffman[j] ) {
                unsigned char byte = (unsigned char)j + 1;
                fputc( byte, outputFile );
                //printf( "%zu ", byte );
                found = 1;
            } else if( ( j == size - 1 ) && ( !found ) ) {
                fputc( 0x00, outputFile );
                //printf( "%zu ", 0x00 );
            }
        }
    }

    //printf( "\n" );
    return 0;
}

int metaData( blockMeta *meta, FILE *outputFile ) {

    //printf( "\nAdding metadata\n" );

    primaryIndex ( meta->primaryIndex, outputFile );
    tableSizeMeta( meta->tableSize,    outputFile );
    huffmanMeta  ( meta->huffman, meta->tableSize, outputFile );

    

    return 1;
}

int write( symbol_t *input, size_t len, blockMeta *meta, FILE *outputFile ) {

    //printf( "\nwrite start.\n" );
    int *buffer = malloc( 300 * sizeof( int ) );
    int idx = 0;


    for( int i = 0; i < len; i++ ) {
        int j = 0;
        while( input[i] != meta->huffman[j] ) {
            //printf( "0" );
            buffer[idx] = 0;
            idx++;
            j++;
        }
        
        //printf( "1" );
        buffer[idx] = 1;
        //break;
        idx++;


        while( idx > 7 ) {  // idx = 7 means 8 digits total ( 1 byte )
            unsigned char byteToAdd = binToByte( buffer );

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

        unsigned char lastByte = binToByte( buffer );
        fputc( lastByte, outputFile );
        //printf( "printing last byte\n" );
    }
    
    //printf( "\nwrite finish.\n" );
    free( buffer );

    return 0;
}


size_t Huffman( symbol_t *input,
                unsigned char *output,
                size_t len,
                blockMeta *meta ) {

    //printf( "Huffman\n\n" );

    int temp1[259] = { 0 };
    int temp2[259] = { 0 };
    for( size_t i = 0; i < len; i++ ) {
        temp1[input[i]] += 1;
    }

    //printf( "huffman table temp1\n" );
        
    for( int i = 0; i < 259; i++ ) { //not 259 because we leave off endofblock char in encoding so it gets the last huffman code possible
        //printf( "%d ", temp1[i] );
    }
    //printf( "\n" );

    int hufflen = 1;

    for( int i = 257; i >= 0; i-- ) { //not 258 because we leave off endofblock char in encoding so it gets the last huffman code possible

        if( temp1[i] > 0 ) {
            int j;
            for( j = 0; ( ( j < hufflen ) && ( temp2[j] >= temp1[i] ) ); j++ );
            if( temp2[j] < temp1[i] ) {
                for( int k = hufflen - 1; k >= j; k-- ) {
                    
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

    //printf( "\n" );
    //printf( "huffman table\n" );
    for( int i = 0; i < hufflen; i++ ) {
        //printf( "%d ", meta->huffman[i] );
    }
    //printf( "\n" );

    meta->tableSize = hufflen;

    //printf( "writing huffman encoded data \n" );

    
    FILE *outputFile = fopen( "output.zip", "ab" );
    if( outputFile ) {
        //printf( "File output.zip opened for writing.\n" );
    }

    if( !outputFile ) {
        perror( "fopen" );
        exit( 1 );
    }

    fseek( outputFile, 0, SEEK_END );
    long pos = ftell( outputFile );
    //printf( "File position before writing: %ld\n", pos );


    metaData( meta, outputFile );
    write( input, len, meta, outputFile );

    fclose( outputFile );
    //printf( "\n" );
    return 0;
}
