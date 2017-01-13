/* UT El Paso EE 4374 Lab 4
   Socket Programming Client/Server application
   Author: Jose Caballero
   04/26/2016

   The purpose of this lab was to create a banking client application that connects to a banking server and 
   the client handles operations such as checking balances, deposits, and withdrawal for an specific account 
   number. Using the send/recv function, a packet containing a bank object was sent and received from the client
   to the server and vice-versa. 
 
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "jgcaballero_banking.h"

/* Global variables that are shared */
sBANK_PROTOCOL bankData;

int setupTCPClient(char *servIPAddr, unsigned int portNum)
{
    int clientSocket;
    struct sockaddr_in servAddr;

    /* Setup address of server */
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(servIPAddr);
    servAddr.sin_port = htons(portNum);

    /* Create socket */
    if((clientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        printf("Failed to create socket; Error %d: %s\n", errno, strerror(errno));
        return -1;
    }

    /* Connect socket to server */
    if(connect(clientSocket,(struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
    {
        printf("Failed to connect socket to %s:%d; Error %d: %s\n", servIPAddr, portNum, errno, strerror(errno));
        return -1;
    }

    return clientSocket;
}

int main(int argc, char **argv)
{
    int mySocket;
    char serverIP[15];
    unsigned int portNum, acountNum, moneyValue;
    char command[5];
    char recvBuff[1024];

    /* If arguments are not the six specified program, return */
    if(argc != 6)
    {
        printf("Usage: bankClient servIPAddr servPortNum command acctNum value\n");
        return -1;
    }

	/* Setup the IP address */
	strcpy(serverIP, argv[1]);
	
	/* Setup TCP port number */
	portNum = atoi(argv[2]);

    /* Setup Command */
    strcpy(command, argv[3]);

    /* Setup Account Number */
    acountNum = atoi(argv[4]);

    /* Setup value */
    moneyValue = atoi(argv[5]);

    /* Setup the client socket */
    if((mySocket = setupTCPClient(serverIP, portNum)) < 0)
    {
        printf("SERVERIP: %d\n", serverIP );
        printf("PORTNUMBER: %d\n", portNum);
        printf("Could not establish connection to server!\n");
        return -1;
    }

    /* If connection was successful ...*/
    else if((mySocket = setupTCPClient(serverIP, portNum)) > 0)
    {
        /*B for balance inquiry command*/
        if(strcmp(command, "B") == 0){
       
        /* Creating bank information that will be sent to the server. */
        bankData.trans = 2;
        bankData.acctnum = acountNum;
        bankData.value = moneyValue;

        /* Send socket */
        send(mySocket, &bankData, sizeof(bankData), 0);
        printf("- - - -S E N T - - - -\n");
        printf("TRANSACTION : %d\n", bankData.trans);
        printf("ACCOUNT NUMBER: %d\n",bankData.acctnum);
        printf("VALUE: %d\n", bankData.value);

        /* Receive whatever the server sent back */
        recv(mySocket, &bankData, sizeof(bankData), 0);
        printf("- - R E C E I V E D - - \n\n");
        printf("CHECKING ACCOUNT NUMBER: %d\n",bankData.acctnum);
        printf("ACCOUNT BALANCE IS: %d\n", bankData.value);
        }

        /*D for deposit command*/
        else if(strcmp(command, "D") == 0){

        bankData.trans = 0;
        bankData.acctnum = acountNum;
        bankData.value = moneyValue;

        send(mySocket, &bankData, sizeof(bankData), 0);
        printf("- - - -S E N T - - - -\n");
        printf("TRANSACTION : %d\n", bankData.trans);
        printf("ACCOUNT NUMBER: %d\n",bankData.acctnum);
        printf("VALUE: %d\n", bankData.value);

        recv(mySocket, &bankData, sizeof(bankData), 0);
        printf("- - R E C E I V E D - - \n\n");
        printf("ADDING $$$ TO ACCOUNT NUMBER: %d\n",bankData.acctnum);
        printf("MONEY DEPOSITED IS: %d\n", bankData.value);
        }

        /*W for withdrawal command*/
        else if(strcmp(command, "W") == 0){

        bankData.trans = 1;
        bankData.acctnum = acountNum;
        bankData.value = moneyValue;

        send(mySocket, &bankData, sizeof(bankData), 0);
        printf("- - - -S E N T - - - -\n");
        printf("TRANSACTION : %d\n", bankData.trans);
        printf("ACCOUNT NUMBER: %d\n",bankData.acctnum);
        printf("VALUE: %d\n", bankData.value);

        recv(mySocket, &bankData, sizeof(bankData), 0);
        printf("- - R E C E I V E D - - \n\n");
        printf("RETRIEVING $$$ TO ACCOUNT NUMBER: %d\n",bankData.acctnum);
        printf("MONEY RETRIEVED : %d\n", bankData.value);
        }

    }

    /*send(mySocket, sendString, strlen(sendString), 0) */
    close(mySocket);

    return 0;
}
