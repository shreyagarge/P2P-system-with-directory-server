#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/select.h>

int main(int argc, char** argv) 
{
	//HERE YOU STORE THE PORT NUMBER THAT WAS ENTERED AS AN ARGUMENT WHEN THE CLIENT SPAWNED
	char port_number[10];
	strcpy(port_number, argv[1]);
	//"port_number" IS STORING THE ENTERED PORT_NUMBER 

	struct addrinfo hints, *ai;
	int server_sock;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags	= AI_PASSIVE;
	
	ai = NULL;
	int res;
	int i, j, k;
	if((res = getaddrinfo(NULL, "9010", &hints, &ai)) != 0) 
	{
		printf("GETADDRESSINFO() FAILED\n");
		return 0;
	}

        if((server_sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol)) == -1) 
	{
		printf("CREATING SOCKET FAILED\n");
		return 0;
	}	
	
	if((res = connect(server_sock, ai->ai_addr, ai->ai_addrlen)) == -1) 
	{
		printf("CONNECTION FAILED\n");
		return 0;	
	}
        
         char ch,files[10000];
        char packet1[]="1 ";
        FILE *fp;
       
        system("ls > directory.txt");
        fp=fopen("directory.txt","r");
        
        int co=0;
        while((ch=fgetc(fp))!=EOF)
        { 
          if(ch == '\n')
          {
             ch=' ';
          } 
          files[co]=ch;
          co++;
        }
        files[co]='\0';
        fgets(files,10000,fp); 
         strcat(packet1,port_number);
         strcat(packet1," ");
         strcat(packet1,files);
         if (send(server_sock, packet1, strlen(packet1), 0) == -1){
			printf("Error in sending !\n");
			exit(0);
		}
  //packet1 sent. server recieves and stores it in its directory.
       
	pid_t pid;
	pid = fork();
     
        if(pid == 0)
	{

                while(1){
                int clientSocket;
                struct sockaddr_in serverAddr;
	        socklen_t addr_size;
                char filename[100],packet3[7],buffer1[10000];
                char packet2[]="2 ";
                printf("enter the name of the file required : ");
                gets(filename);
                 strcat(packet2,filename);
                puts("this is packet2 before it was sent : ");
                puts(packet2);
                 if (send(server_sock, packet2, strlen(packet2), 0) == -1){
			printf("Error in sending !\n");
			exit(0);
		}
                if (recv(server_sock, packet3, 7, 0) <= 0){
			printf("Error recieving content \n");
			exit(0);
                   }
                 puts("this is the packet3 it is recieving : ");
               
               puts(packet3);

                char s[]=" ";
                char *token;

                token = strtok(packet3,s);
                token = strtok(NULL,s);
                puts("this is the port number extracted from packet3 : ");
                puts(token);
                clientSocket = socket(PF_INET, SOCK_STREAM, 0);
		if (clientSocket == -1){
			printf("Error creating socket !\n");
			exit(0);
		}
                char dsf[4];
                strncpy(dsf,token,4);
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(atoi(token));
		serverAddr.sin_addr.s_addr = INADDR_ANY;
 		memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
		addr_size = sizeof serverAddr;

		if (connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size) == -1){
			printf("Error in connecting !\n");
			exit(0);
		}
                
               if (send(clientSocket, packet2, strlen(packet2), 0) == -1){
			printf("Error in sending !\n");
			exit(0);
		}
                if (recv(clientSocket, buffer1, sizeof(buffer1), 0) <= 0){
			printf("Error recieving content \n");
			exit(0);
                }
                else
               printf("file recieved.contents are :\n");
               puts(buffer1);
               
               close(clientSocket);
             }
		
	}
	else
	{
            while(1){
                int welcomeSocket,newSocket;
                struct sockaddr_in serverAddr;
                struct sockaddr_storage serverStorage;
	        socklen_t addr_size;
                char filename[100],buffer1[100];
                welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (welcomeSocket == -1){
		printf("Error creating socket !\n");
		exit(0);
	}  
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(port_number));
	serverAddr.sin_addr.s_addr = INADDR_ANY;   
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
	addr_size = sizeof (serverStorage);

	if (bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1){
		printf("Error in binding !\n");
		exit(0);
	}

	if(listen(welcomeSocket,5)==0)
		printf("Listening...\n");
	else{
		printf("Error in connection !\n");
		exit(0);
	}
	
	newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
	if (newSocket == -1){
		printf("Error accepting \n");
		exit(0);
	} 	
             if (recv(newSocket, filename, sizeof filename, 0) <= 0){
		printf("Error recieving \n");
		exit(0);
	}
        puts("packet2 is being recieved as ");
        puts(filename);
         int g;
         char s[]=" ";
                char *fn;

                fn = strtok(filename,s);
                fn = strtok(NULL,s);
        puts("filename extracted from packet2 is : ");
        puts(fn);
        FILE *fp1;
        fp1 = fopen(fn,"r");
        fgets(buffer1,100,fp1);
        
        if (send(newSocket, buffer1, sizeof(buffer1), 0) <= 0){
		printf("Error sending \n");
		exit(0);
	}
        close(newSocket);
        close(welcomeSocket);
    }
		
	}
}
            
