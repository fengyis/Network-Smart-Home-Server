/* ========= TCPEchoClient-DNS =========== */
/* created Jan.19, 2014 for 4DN4           */

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#define RCVBUFSIZE 128   /* Size of receive buffer */

void DieWithError(char *errorMessage);  /* Error handling function */

int main(int argc, char *argv[])
{   char ch1[20],ch2[20],ch3[20],ch4[20],ch5[20],name[80];    
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    unsigned short echoServPort;     /* Echo server port */
    char *servIP = ch1;                    /* Server IP address (dotted quad) */
    char *echoString = name;                /* String to send to echo server */
    char *servPort=ch3;
    char *command=ch4;
    char *writeValue=ch5;
    char echoBuffer[RCVBUFSIZE]; 
    char echoBuffer2[RCVBUFSIZE]; 
    char value[RCVBUFSIZE];     /* Buffer for echo string */
    unsigned int echoStringLen1;      /* Length of string to echo */
    unsigned int echoStringLen2; 
    int bytesRcvd, totalBytesRcvd;   /* Bytes read in single recv() 
                                        and total bytes read */
    char *WRITE;
    char *QUIT;
    WRITE="write";
    QUIT="quit";

//    scan user input
    printf("Please enter ip\n");
    scanf("%s",servIP);
    printf("Please enter port\n");
    scanf("%s",servPort);
    
    
//  convert string to integer
    echoServPort = atoi(servPort);


  
//  run forever, re-create socket connecting to sever for new command
    for (;;){
        
    memset(&echoBuffer, 0, sizeof(echoBuffer));   /* Zero out structure */
    memset(&value, 0, sizeof(value)); 
    
      /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

            
    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family      = AF_INET;             /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    echoServAddr.sin_port        = htons(echoServPort); /* Server port */

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("connect() failed");
    
    printf("Client connected to server on port %d\n", echoServPort);
    
        
    printf("Please enter command\n");
    scanf("%s",command);
    printf("Please enter device name\n");
    scanf("%s",echoString);
    if(strncmp(command,WRITE,strlen(WRITE))==0){
        printf("Please enter write value\n");
        scanf("%s",writeValue);
    }
        
    
    echoStringLen1 = strlen(echoString);          /* Determine input length */
    echoStringLen2 = strlen(command);  
    
    /* Send the devicename to the server */
    if (send(sock, echoString, echoStringLen1, 0) != echoStringLen1)
        DieWithError("send() sent a different number of bytes than expected");
    
    printf("Client sent message to server: ");
    printf(echoString); 
    printf("\n");
    
    /* receive the ACK  */
    bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0);
    
    if (bytesRcvd  <= 0)
        DieWithError("recv() failed or connection closed prematurely");
    else    
        printf("Client Receives ACK from Server of '%s'\n", echoBuffer);     


    printf("\n");  
    
    /* Send the command to the server */
    if (send(sock, command, echoStringLen2, 0) != echoStringLen2)
        DieWithError("send() sent a different number of bytes than expected");
    else
        printf("Client sent command to server: ");
        printf(command); 
        printf("\n");

        
    //receive the ACK
    bytesRcvd = recv(sock, echoBuffer2, RCVBUFSIZE - 1, 0);
    if (bytesRcvd  <= 0)
            DieWithError("recv() failed or connection closed prematurely");
    else    
        printf("Receives ACK from server of command ");
        printf(echoBuffer2);
        printf("\n");


    printf("\n");    
    
    //check if write command is inputed
    if(strncmp(command,WRITE,strlen(WRITE))==0){
        if (send(sock, writeValue, strlen(writeValue), 0) != strlen(writeValue))
            DieWithError("send() sent a different number of bytes than expected");
        else
            printf("Client sent writevalue to server: ");
            printf(writeValue); 
            printf("\n");
    }
    
    
    //receive server message responds to the command    
    bytesRcvd = recv(sock, value, RCVBUFSIZE - 1, 0);
    if (bytesRcvd  <= 0)
        DieWithError("recv() failed or connection closed prematurely");
    else    
        printf("Client Receives value '%s'\n", value);     


    printf("\n");    
    
    //close socket    
    sleep(1);
    close(sock);
    if(strncmp(command,QUIT,strlen(QUIT))==0){
          exit(0);
    }
    }
    
    

}



