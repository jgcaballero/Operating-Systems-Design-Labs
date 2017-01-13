##############################################################################
#
#    Makefile for jgcaballero_lab4 (Bank Client/Server)
#    written by Jose Caballero
# 
##############################################################################

##############################################################################
# jgcaballero_lab4 is the name of the executable file
# bankClient.c and bankServer.c, jgcaballero_banking.h needed 
# to create executable.
# Run using make and then run using
# ./bankClient servIPAddr servPortNum command # acctNum value 
# ./bankServer portNum
##############################################################################

#Compiling executables
all: jgcaballero_bankClient jgcaballero_bankServer

#Creates bankClient executable
jgcaballero_bankClient: jgcaballero_bankClient.o 
	gcc jgcaballero_bankClient.o -o jgcaballero_bankClient

#Creating the output of the .c files used for bankClient
jgcaballero_bankClient.o: jgcaballero_bankClient.c
	gcc jgcaballero_bankClient.c -c -g -o jgcaballero_bankClient.o

#Creates bankServer executable
jgcaballero_bankServer: jgcaballero_bankServer.o 
	gcc jgcaballero_bankServer.o -o jgcaballero_bankServer

#Creating the output of the .c files used for bankServer
jgcaballero_bankServer.o: jgcaballero_bankServer.c
	gcc jgcaballero_bankServer.c -c -g -o jgcaballero_bankServer.o

clean:
	rm *.o;rm jgcaballero_bankServer;rm jgcaballero_bankClient; rm *.exe