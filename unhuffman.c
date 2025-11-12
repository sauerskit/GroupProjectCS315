#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"
#include "unhuffman.h"

// blockMeta struct
// primaryIndex    ( size_t )
// tableSize       table size (shocking) ( size_t )
// runMagic         magic bit for encoding runa or runb first ( int )
// huffman[259]        huffman table ( int )

int unMeta( FILE *inputFile, blockMeta *meta, unsigned char *input ) {

    unsigned char bytes[3];

    size_t bytesRead = fread( bytes, 1, 3, inputFile ); // primaryIndex
    if( bytesRead == 0 ) {      
        //printf( "\n\nend of file\n" );
        return -1;
    }      

    int idx = ( bytes[0] << 16 ) | ( bytes[1] << 8 ) | bytes[2];

    //printf( "primary index: %d\n", idx );
    meta->primaryIndex = idx;
    
                            
    if( fread( bytes, 1, 1, inputFile ) < 1 ) { // length of table
        //printf( stderr, "unexpected eof while reading metadata\n" );
                                                                 
    }      
    
    int tsize = bytes[0];
    meta->tableSize = tsize;
    //printf( "table size: %d\n", meta->tableSize );    

    for( int i = 0; i < 259; i++ ) {    // huffman tree

        fread( bytes, 1, 1, inputFile );
            
        if( bytes[0] > 0 ) {
            meta->huffman[bytes[0] - 1] = i;
        }
    }

    //printf( "\nhuffman table reconstructed:\n" );
    for( int i = 0; i < tsize; i++ ) {
        //printf("%d ", meta->huffman[i] );
    }

//    printf( "\n" );

    return 0;
}

size_t Unhuffman( unsigned char *input,
                  symbol_t *output,
                  blockMeta *meta,
                  FILE *inputFile ) {

    int status = unMeta( inputFile, meta, input );
    
    if( status == -1 ) {
        return -1;
    }

    unsigned char byte;
    int bit;
    int leftovers = 0;
    int symIdx = 0;

    //printf( "\nReading Huffman bits:\n" );
    while( 1 ) {
        fread( &byte, 1, 1, inputFile );

        for( int i = 0; i < 8; i++ ) {
            int bit = ( byte >> ( 7 - i ) ) & 1;
            if( bit == 0 ) {
                leftovers++;
            } else if ( ( bit == 1 ) && ( meta->huffman[leftovers] != 258 ) ) {
                output[symIdx] = meta->huffman[leftovers];

                //printf( "%d ", output[symIdx] );
                if( output[symIdx] == 258 ) {
                    return symIdx;
                }
                
                leftovers = 0;
                symIdx++;
            } else if (meta->huffman[leftovers] == 258 ) {
                return symIdx;
            }
        }
    }

    

    return 0; // TODO: RETURN LENGTH OF MESSAGE
}
