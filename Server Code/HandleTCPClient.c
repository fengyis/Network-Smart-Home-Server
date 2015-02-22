/*
 *  HandleTCPClient.h  for DNS server
 *
 *  Created by Prof. TS for 4DN4 on Sunday, Jan. 19, 2014
 *
 */

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <string.h>
#include <stdbool.h>

#define RCVBUFSIZE 32   /* Size of receive buffer */
int i=1;
int y=0;

void DieWithError(char *errorMessage);  /* Error handling function */

void HandleTCPClient(int clntSocket)
{
    char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
    char command[RCVBUFSIZE];        /* Buffer for echo string */
    char writeValue[RCVBUFSIZE];
    int recvMsgSize;                    /* Size of received message */    int recvMsgSize1;                    /* Size of 1st received message */
    char dest[300];
    /* add the table fir the DNS server */
    struct {
	char deviceName[128];
	char readValue[32];
        char writeValue[32];
	} table[64];

   int  j,k, strlength;
   bool match;
   char *ADD,*REMOVE,*READ,*WRITE,*QUIT,*VALUE;
   ADD="add";
   REMOVE="remove";
   READ="read";
   WRITE="write";
   QUIT="quit";
   
   y++;
   
//   initiate smart home database
   if(y==1){
        strcpy(table[0].deviceName, "Thermostat-Main");
        strcpy(table[0].readValue, "19");
        strcpy(table[0].writeValue, "23");

        strcpy(table[1].deviceName, "Thermostat-Living-Room");
        strcpy(table[1].readValue, "18");
        strcpy(table[1].writeValue, "22");
   }



   memset(&echoBuffer, 0, sizeof(echoBuffer));   /* Zero out structure */
   memset(&command, 0, sizeof(command)); 
   memset(&VALUE, 0, sizeof(VALUE)); 
   memset(&writeValue, 0, sizeof(writeValue)); 
   memset(&dest, 0, sizeof(dest)); 


   
    /* Receive device name from client */
   if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");
   else 
        printf("server receives this device name: '%s'\n", echoBuffer);

   strlength = strlen(echoBuffer);
   
   /* send ACK */
   if (send(clntSocket, echoBuffer, strlength, 0) != strlength)
        DieWithError("send() failed");
   else
        printf("server sent string '%s' to client\n", echoBuffer);

   /* Receive command from client */
   if ((recvMsgSize = recv(clntSocket, command, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");
   
   /* send ACK */
   if (send(clntSocket, command, strlength, 0) != strlength)
        DieWithError("send() failed");

   printf("server sent ACK of '%s' to client\n", command);
   

   
   /*  search the database, looking for the same string */
   
   printf("server receives this command: '%s'\n", command);

	
    for (j=0;j<=63;j++) {
            if (strncmp(echoBuffer,table[j].deviceName, strlength) == 0) {
              printf("we have a match in row %g !\n", j);
//            a boolean variable to mark if the match is found
              match=true;
              break;
       }
    }
    if (j == 64) {
        printf("we have no match ! \n");
        match=false;
        sleep(1);
    }
    
    
   
//   give different response to different command, store the response message in VALUE
     if(!match && strncmp(command,ADD,strlen(ADD))==0){
         i++;
         printf("i= %d \n", i);
         if(i<64){
         strcpy(table[i].deviceName, echoBuffer);
         strcpy(table[i].readValue, "0");
         strcpy(table[i].writeValue, "0");
         VALUE="ADDED";
         }else{
         VALUE="Database full";
         }
     }else if(match && strncmp(command,ADD,strlen(ADD))==0) {
         VALUE="The device name already exist, please re-enter";
     }else if(strncmp(command,READ,strlen(READ))==0){
         if(match){
           strcpy(dest, "read value= " );
           strcat(dest, table[j].readValue);
           strcat(dest," write value= ");
           strcat(dest,table[j].writeValue); 
           printf(table[j].writeValue);
           VALUE=dest;
         }else{
           VALUE="device name not found, please re-enter";
         }
     }else if(strncmp(command,REMOVE,strlen(REMOVE))==0){
         if(match){
           strcpy(table[j].deviceName, "removed");
           strcpy(table[j].readValue, "0");
           strcpy(table[j].writeValue, "0");
           VALUE="Device found, data removed";
         }else{
           VALUE="Device not found, please re-enter";
         }
     }else if(strncmp(command,WRITE,strlen(WRITE))==0){
         sleep(1);
//       if write command is entered, receive the write value from client
         if ((recvMsgSize = recv(clntSocket, writeValue, RCVBUFSIZE, 0)) < 0)
                DieWithError("recv() failed");
         
         if(match){
             printf(table[j].writeValue);
             strcpy(table[j].writeValue, writeValue);
             VALUE="Device found, writting message stored";
             printf(table[j].writeValue);
             printf("the inedex j is %d",j);
           
         }else{
             VALUE="Device not found, please re-enter";
         }
     }else if(strncmp(command,QUIT,strlen(QUIT))==0){
           VALUE="Socket connection will be closed";  
     }else{
           VALUE="Command not found,please re-enter";     
     }
    
    


  
   
   sleep(1);
   strlength=strlen(VALUE);
   
// send client the message 
   if (send(clntSocket, VALUE, strlength, 0) != strlength)
        DieWithError("send() failed");

   printf("server sent VALUE '%s' to client\n", VALUE);
   
   printf("the value is %s", VALUE);
   


   close(clntSocket);    /* Close client socket */

}

