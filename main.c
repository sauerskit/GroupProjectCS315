#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"
#include "bwt.h"
#include "mtf.h"
#include "rle.h"
#include "huffman.h"

#define BLOCK_SIZE ( 500 * 1024 ) // 500 kilobyte block sizes

int main( int argc, char *argv[] ) {
    
    if( argc != 3 ) {
        printf( "Usage:\n./compression [file] [1/2] "
                "(1 for compress, 2 for decompress)\n" );
        return 1;
    }
    
    int compress = atoi( argv[2] );
    if( ( compress < 1 ) || ( compress > 2 ) ) {
        printf( "Invalid option for compress/decompress."
                " Choose 1 for compress and 2 for decompress\n" );
        return 1;
    }

    FILE *inputFile  = fopen( argv[1], "rb" );
    FILE *outputFile = fopen( "output.bwt", "wb" ); // .azip Addison ZIP

    if( !inputFile ) {
        perror( "fopen" );
        return 1;
    }

    // bufferA and bufferB so that I can reuse buffers for input/output after
    // each transform
    unsigned char *bufferA = malloc( BLOCK_SIZE * sizeof( symbol_t ) );
    // size of buffer in int16's because symbolic over byte compression

    if( !bufferA ) {
        perror( "malloc" );
        return 1;
    }

    unsigned char *bufferB = malloc( BLOCK_SIZE * sizeof( symbol_t ) );

    if( !bufferB ) {
        perror( "malloc" );
        return 1;
    }

    BWTResult bwtRes;
    size_t read;  // 8 bytes usually
    size_t blockNumber = 0;

    
    switch( compress ) {
        case 1:
            printf( "Compressing!\n" );

            while( ( read = fread( bufferA, 1, BLOCK_SIZE, inputFile ) ) > 0 ) {
                printf( "processing block %zu with %zu bytes\n", 
                                                    blockNumber, 
                                                    read );

                size_t sizeA = BWT( bufferA, bufferB, read, &bwtRes );
                // printf( "Primary index: %zu\n", bwtRes.primaryIndex );

                // for( size_t i = 0; i < read; i++ ) {
                    //putchar( bufferB[i] );
                //}
                //putchar( '\n' );
                //putchar( '\n' );


                size_t sizeB = MTF( bufferB, bufferA, sizeA );


                size_t sizeC = RLE( bufferA, bufferB, sizeB );
                size_t sizeD = Huffman( );

                fwrite( &bwtRes.primaryIndex, sizeof( size_t ), 1, outputFile );
                fwrite( bufferB, 1, sizeC, outputFile );
    
            }
        
            break;
            
        case 2:
            
            printf( "Decompressing!\n" );
            
            break;
    }

    blockNumber++;

    free( bufferA );
    free( bufferB );

    fclose( inputFile  );
    fclose( outputFile );
    return 0;
}
