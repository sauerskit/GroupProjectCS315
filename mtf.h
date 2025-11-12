#ifndef MTF_H
#define MTF_H

size_t MTF( const unsigned char *input,
               unsigned char *output,
               size_t        len );

size_t UnMTF( unsigned char *output,
              unsigned char *input,
              size_t        len );
#endif
