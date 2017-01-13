all: josecaballero_argtok

josecaballero_argtok.o: josecaballero_argtok.c
	gcc josecaballero_argtok.c -c -g -o josecaballero_argtok.o

josecaballero_argtok: josecaballero_argtok.o
	gcc josecaballero_argtok.o -o josecaballero_argtok

sudo clean:
	rm -f *.o  *.exe