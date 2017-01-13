##############################################################################
#
#    Makefile for jgcaballero_lab2.c
#    written by Jose Caballero
# 
##############################################################################

##############################################################################
# jgcaballero_lab2 is the name of the executable file
# jgcaballero_argtok.c/h, jgcaballero_exec.c/h and jgcaballero_lab2.c needed 
# to create executable.
# Run using make and then run using ./jgcaballero_lab2
##############################################################################

all: jgcaballero_lab2

#Compiling executable
jgcaballero_lab2: jgcaballero_argtok.o jgcaballero_exec.o jgcaballero_lab2.o
	gcc jgcaballero_argtok.o jgcaballero_exec.o jgcaballero_lab2.o -o  jgcaballero_lab2

#Creating the output of the .c files used for jgcaballero_exec.o
jgcaballero_exec.o: jgcaballero_exec.o
	gcc -c jgcaballero_exec.c 

#Creating the output of the .c files used for jgcaballero_lab2.o
jgcaballero_lab2.o: jgcaballero_lab2.o
	gcc -c jgcaballero_lab2.c 

#Creating the output of the .c files used for jgcaballero_argtok.o
jgcaballero_argtok.o: jgcaballero_argtok.o
	gcc -c jgcaballero_argtok.c 

sudo clean:
	rm -f *.o  *.exe
