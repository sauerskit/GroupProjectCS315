#ifndef BWT_H
#define BWT_H

#include <stddef.h> //for size_t

//typedef struct {
    //size_t primaryIndex;
//} BWTResult;

size_t BWT( const unsigned char *input,
               unsigned char *output, 
               size_t        len, 
               blockMeta     *res ); 
        // unsigned char = byte

size_t UnBWT( unsigned char *output,
            unsigned char *input, 
            size_t        len, 
            blockMeta     *res ); 

#endif
