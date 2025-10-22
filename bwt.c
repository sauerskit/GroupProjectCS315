#define _GNU_SOURCE  // for qsort_r

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"
#include "bwt.h"

#define BLOCK_SIZE ( 500 * 1024 )

int suffixArray( ) {
    printf( "Suffix Array\n" );

    return 0;
}

// bizarre to figure out, but works quite nice
static int cmpSuffix( const void *lhs, const void *rhs, void *context ) {

    // the only types accepted are const void, so we have to recast everything
    const unsigned char *block = context;
    int item1 = *( const int * )lhs;
    int item2 = *( const int * )rhs;
    size_t N = BLOCK_SIZE;
    // recasting complete

    // here we iterate through the 2 suffixes until either a difference, or
    // the end of a block
    while( ( item1 < N ) && 
           ( item2 < N ) && 
           ( block[item1] == block[item2] ) ) {
        item1++;
        item2++;
    }

    // if no difference, return 0
    if( ( item1 == N ) && ( item2 == N ) ) {
        return 0;
    }

    // if item1 reaches end of block (shorter) return -1
    if( item1 == N ) {
        return -1;
    }

    // if item2 reaches end of block (shorter) return 1
    if( item2 == N ) {
        return 1;
    }

    // otherwise, return the difference (like a strcmp)
    return ( int )block[item1] - ( int )block[item2];
    // super slick overall
}

size_t BWT( const unsigned char *input, 
               unsigned char *output, 
               size_t        len, 
               BWTResult     *res ) {

    printf( "BWT\n" );

    size_t size = 0;

    int *suffixArr = malloc( len * sizeof( int ) );
    if( !suffixArr ) {
        perror( "malloc" );
        exit( 1 );
    }

    for( int i = 0; i < len; i++ ) {    // fill suffix Array with numbers
        suffixArr[i] = i;
    }

    // this is sorting suffixArr by the lexigraphical order of the suffixes
    // in our block (input). Super trippy
    qsort_r( suffixArr, len, sizeof( int ), cmpSuffix, ( void * )input );
    // qsort_r arguments:
    // void qsort_r( void *base,
    //               size_t nitems,
    //               size_t size,
    //               int ( *compar )( const void *, const void *, void * ), 
    //               void *arg );
    // wtf is this?? lemme explain: base is your array, nitems = Number Items,
    // size is the size of each item in bytes. In our example, it is sizeofInt
    // because our suffixArr is made of ints. compar is a pointer to a compare
    // function that has 3 arguments that are described in more detail above
    // (however, note the type that they accept), qsort_r passes args for us,
    // and finally arg is for any extra data, in our case, the bytes in our
    // current block.
    // thank you for coming to my TedTalk

    // ok now that that horribleness is over, we have a suffixArr, where the
    // indices of our bytes are organized by the size of the bytes, but we
    // still know where everything is because we just sorted the original
    // indices without touching the actual block itself
    
    int primaryIndex = 0; // the original rotation will be stored here
    
    // this for loop checks each element at suffixArr until '0' is found, and
    // the index where 0 is stored is the index of the original rotation
    for( int i = 0; i < len; i++ ) {
        int suffixStart = suffixArr[i];
        if( suffixStart == 0 ) { // 0 found...
            primaryIndex = i;    // index of original rotation
        }

        int prevIndex;
        if( suffixStart == 0 ) {  // this section just gets the previous index
            prevIndex = len - 1;  // and also deals with wrapping
        } else {
            prevIndex = suffixStart - 1; // lowkey easier than modulus lol
        }

        output[i] = input[prevIndex];
        size++;
        
    }

    res->primaryIndex = primaryIndex;

    printf( "\n" );
    free( suffixArr );
    return size;
}

