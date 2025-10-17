#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE ( 900 * 1024 )

int main( int argc, char *argv[] ) {
    
    if( !argv[1] ) {
        printf( "./output [file]\n" );
        return 0;
    }

    FILE *inputFile = fopen( argv[1], "rb" );

    if( !inputFile ) {
        perror( "fopen" );
        return 1;
    }

    unsigned char *block = malloc( BLOCK_SIZE );

    if( !block ) {
        perror( "malloc" );
        return 1;
    }

    size_t read;
    size_t blockNumber = 0;

    while( ( read = fread( block, 1, BLOCK_SIZE, inputFile ) ) > 0 ) {
        printf( "processing block %zu with %zu bytes\n", blockNumber, read );

    

    }

    free( block );
    fclose( inputFile );
    return 0;
}
