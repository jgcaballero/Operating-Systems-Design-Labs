/* UT El Paso EE 4374 Lab 4
   Socket Programming Client/Server application
   Author: Jose Caballero
   04/26/2016

   The purpose of this lab was to create a banking server application that accepts a client connection. The server allows 
   the client such as checking balances, deposits, and withdrawal  for an specific account number. Using the send/recv 
   function, a packet containing a bank object was sent and received from the client to the server and vice-versa. 
 
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "jgcaballero_banking.h"

/* Global variables that are shared */
sBANK_PROTOCOL bankData;

#define NUM_ACCTS   100

typedef struct
{
    unsigned int 	balance;
    pthread_mutex_t mutex;
} sBANK_ACCT_DATA;

sBANK_ACCT_DATA acctData[NUM_ACCTS];

int handleClient(int clientSocket)
{

    int msgSize;

    if((msgSize = recv(clientSocket, &bankData, sizeof(bankData), 0)) < 0)
    {
        printf("Failed to receive on client socket; Error %d: %s\n", errno, strerror(errno));
        return -1;
    }
        /* Used for debugging purposes*/
        printf("- - - - - - - - - - - - - - - - - - - - - - -\n");
        printf("WE GOT TRANS NUMBAH : %d\n",bankData.trans);
        printf("WE GOT ACCT NUMBAH : %d\n",bankData.acctnum);
        printf("WE GOT VALUE NUMBAH : %d\n\n",bankData.value);

        /* Balance check transaction */
        if(bankData.trans == 2)
        {
            printf("BALANCE CHECK FAM!\n");
            bankData.acctnum = bankData.acctnum;
            bankData.value = acctData[bankData.acctnum].balance; 
            if(acctData[bankData.acctnum].balance < 0)
            {
                bankData.value = 0;
            }
            send(clientSocket, &bankData, sizeof(bankData), 0);  
        }
        
        /* Deposit transaction*/
        else if(bankData.trans == 0)
        {
            printf("DEPOSIT FAM?\n");
            bankData.acctnum = bankData.acctnum;
            acctData[bankData.acctnum].balance += bankData.value;
            send(clientSocket, &bankData, sizeof(bankData), 0);
        }

       /* Withdraw transaction */
        else if(bankData.trans == 1)
        {
            printf("WiTHDRAWING FAM?\n");
            bankData.acctnum = bankData.acctnum;
            if(acctData[bankData.acctnum].balance <= bankData.value){
            acctData[bankData.acctnum].balance = 0;
            send(clientSocket, &bankData, sizeof(bankData), 0); 
        } else 
            acctData[bankData.acctnum].balance -= bankData.value;
            send(clientSocket, &bankData, sizeof(bankData), 0);  
        }

    close(clientSocket);

    return 0;
}

int setupTCPServer(unsigned int portNum, int maxConn)
{
    int servSocket;
    struct sockaddr_in servAddr;

    /* Setup address */
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(0);    /* Accept connections on an IP address */
    servAddr.sin_port = htons(portNum);

    /* Create socket */
    if((servSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        printf("Failed to create socket; Error %d: %s\n", errno, strerror(errno));
        return -1;
    }

    /* Bind socket for server */
    if(bind(servSocket,(struct sockaddr *) &servAddr,sizeof(servAddr)) < 0)
    {
        printf("Failed to bind socket; Error %d: %s\n", errno, strerror(errno));
        return -1;
    }

    /* Setup socket for listening */
    if((listen(servSocket, maxConn)) < 0)
    {
        printf("Failed to listen on socket; Error %d: %s\n", errno, strerror(errno));
        return -1;
    }

    /* return the socket handle */
    return servSocket;

}

int main(int argc, char **argv)
{
    int mySocket, clientSocket;
    struct sockaddr_in clientAddr;
    unsigned int clientLen = sizeof(clientAddr);
    unsigned int portNum;

    printf("ACCESSED PRINT SERVER FAM\n");
   	
  	if(argc != 2)
    {
        printf("Usage: bankServer portNum\n");
        return -1;
    }

	/* Setup TCP port number */
	portNum = atoi(argv[1]);

    /* Setup the server TCP Socket */
    if((mySocket = setupTCPServer(portNum, 5)) < 0)
    {
        return -1;
    }

    /* Accept client connections */
    for(;;)
    {
        if((clientSocket = accept(mySocket, (struct sockaddr *) &clientAddr, &clientLen)) < 0)
        {
            printf("Failed to accept connection from client; Error %d: %s\n", errno, strerror(errno));
            return -1;
        }

        else if((clientSocket = accept(mySocket, (struct sockaddr *) &clientAddr, &clientLen)) > 0)
        {
            handleClient(clientSocket);
        }

    }

    return 0;

}
