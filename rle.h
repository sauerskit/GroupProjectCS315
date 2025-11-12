#ifndef RLE_H
#define RLE_H

size_t RLE( const unsigned char *input,
                  symbol_t      *output,
                  size_t        len );

size_t UnRLE( unsigned char *output,
              symbol_t      *input,
              size_t        len );
#endif
