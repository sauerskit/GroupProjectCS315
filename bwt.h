#ifndef BWT_H
#define BWT_H

#include <stddef.h> //for size_t

typedef struct {
    size_t primaryIndex;
} BWTResult;

size_t BWT( const unsigned char *input,
               unsigned char *output, 
               size_t        len, 
               BWTResult     *res ); 
        // unsigned char = byte

#endif
