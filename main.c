#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"
#include "bwt.h"
#include "mtf.h"
#include "rle.h"
#include "huffman.h"
#include "unhuffman.h"

#define BLOCK_SIZE ( 500 * 1024 ) // 300 kilobyte block sizes

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

    unsigned char *bufferA = calloc( 1, BLOCK_SIZE );

    if( !bufferA ) {
        perror( "malloc" );
        return 1;
    }

    unsigned char *bufferB = malloc( BLOCK_SIZE );
    if( !bufferB ) {
        perror( "malloc" );
        return 1;
    }

    // size of buffer in int16's because symbolic over byte compression
    symbol_t *symBuffer = malloc( BLOCK_SIZE * sizeof( symbol_t ) );

    if( !bufferB ) {
        perror( "malloc" );
        return 1;
    }

    //BWTResult bwtRes;
    blockMeta *meta = malloc( sizeof( blockMeta ) ); // set all values to 0

    size_t read;  // size_t is 8 bytes usually
    size_t blockNumber = 0;

    
    switch( compress ) {
        case 1:
            printf( "Compressing!\n" );

            while( ( read = fread( bufferA, 1, BLOCK_SIZE, inputFile ) ) > 0 ) {
                printf( "processing block %zu with %zu bytes\n", 
                                                    blockNumber, 
                                                    read );

                size_t sizeA = BWT( bufferA, bufferB, read, meta );
                //printf( "Primary index: %zu\n", meta->primaryIndex );

                for( size_t i = 0; i < read; i++ ) {
                    //putchar( bufferB[i] );
                }
                putchar( '\n' );
                putchar( '\n' );


                size_t sizeB = MTF( bufferB, bufferA, sizeA );

                size_t sizeC = RLE( bufferA, symBuffer, sizeB );

                size_t sizeD = Huffman( symBuffer, bufferB, sizeC, meta );
    
                blockNumber++;
            }
        
            break;
            
        case 2:
            
            printf( "Decompressing!\n\n" );

            FILE *outputFile = fopen( "output.zip", "ab" );
            while( 1 ) {

                size_t sizeA = Unhuffman( bufferA, symBuffer, meta, inputFile );
                if( sizeA == -1 ) {
                    printf( "\nEnd of File!\n" );
                    break;
                    //return 0;
                }
                 
                size_t sizeB = UnRLE( bufferA, symBuffer, sizeA );
                size_t sizeC = UnMTF( bufferB, bufferA, sizeB );
                size_t sizeD = UnBWT( bufferA, bufferB, sizeC, meta );
    
                fwrite( bufferA, 1, sizeD, outputFile );

            }
            fclose( outputFile );
            break;
    }
        


    free( bufferA );
    free( bufferB );

    fclose( inputFile  );
    return 0;
}
