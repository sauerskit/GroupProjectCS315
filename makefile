build:
	gcc -c main.c -o main.o
	gcc -c bwt.c -o bwt.o
	gcc -c mtf.c -o mtf.o
	gcc -c rle.c -o rle.o
	gcc -c huffman.c -o huffman.o
	gcc main.o bwt.o mtf.o rle.o huffman.o -o compression
	rm -f *.o

clean:
	rm -f *.o cstegtool
