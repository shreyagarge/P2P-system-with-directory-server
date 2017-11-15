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

#define PORT "9010" // THIS IS THE SERVER'S PORT ADDRESS

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) 
	{
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int main(void)
{
	fd_set master;
	fd_set read_fds;
	int fd_max;

	int listener;
	int newfd;
	struct sockaddr_storage client;
	char clientIP[INET6_ADDRSTRLEN];
	socklen_t addrlen;

	char buffer[1024];
	int nbytes;

	int yes = 1;
	int i, j, res;

	struct addrinfo hints, *ai, *p;

	FD_ZERO(&read_fds);
	FD_ZERO(&master);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if(res = getaddrinfo(NULL, PORT, &hints, &ai) != 0)
	{
		printf("getaddrinfo ERROR\n");
		return 0;
	}

	

	for(p = ai; p != NULL; p = p->ai_next) 
	{
    	listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (listener < 0) 
		{ 
			continue;
		}
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) 
		{
			close(listener);
			continue;
		}
		break;
	}

	

	freeaddrinfo(ai);

	if(listen(listener, 10) == -1)
	{
		printf("listening error\n");
		return 0;
	}

	FD_SET(listener, &master);

	fd_max = listener;

	int hash_table[1000000];

	for(;;) 
    {
        read_fds = master; // copy it
        if (select(fd_max+1, &read_fds, NULL, NULL, NULL) == -1) 
        {
            printf("SELECT error\n");
            return 0;
        }

        for(i = 0; i <= fd_max; i++) 
        {
            if (FD_ISSET(i, &read_fds)) 
            {
            	int hash_socket;
                if (i == listener) 
                {
                    addrlen = sizeof client;
					newfd = accept(listener,(struct sockaddr *)&client,&addrlen);

					if (newfd == -1) 
					{
                        printf("ACCEPT error\n");
                        return 0;
                    } 
                    else 
                    {
                        FD_SET(newfd, &master);
                        if (newfd > fd_max) 
                        { 
                            fd_max = newfd;
                        }
                        hash_socket = newfd;
                        printf("server: new connection from %s on socket %d\n", inet_ntop(client.ss_family, get_in_addr((struct sockaddr*)&client), clientIP, INET6_ADDRSTRLEN), newfd);
                    }
                }
                else 
                {

                    // HERE THE buffer RECEIVES STUFF FROM THE CLIENT THAT'S CONNECTED TO IT ..
                    // SO CONSIDERING PACKET-1 TO BE OF THIS FORMAT "packet_number|port_number|file1|file2|file3|file4..|xx|"
                    // PACKET-2 TO BE OF THIS FORM "packet_number|port_number_of_the_requesting_client|filename"
                    // PACKET-3 TO BE OF THE FORM "packet_number|peer_port_number-1|peer_port_number-2|xx|"

                    if ((nbytes = recv(i, buffer, sizeof buffer, 0)) <= 0) 
                    {
                        if (nbytes == 0) {
                            printf("server: socket %d hung up\n", i);
                        } 
                        else {
                            printf("RECEIVE error\n");
                            return 0;
                        }
                        close(i);
                        FD_CLR(i, &master);
                    } 
                    else 
                    {
                        if(buffer[0] == '1')
                        {
                         // puts(buffer);
 /* SO HERE AS SOON AS THE CLIENT STARTS UP, IT SENDS A PACKET-1 TO THE SERVER WHICH IT RECEIVES INTO BUFFER,THE IF CHECKS IF IT IS OF PACKET-1 OR PACKET-2 AND ACTS ACCORDINGLY.SINCE HERE, THIS IS A PACKET-1, THE SERVER HAS TO STORE THE DETAILS OF THE NEW CLIENT AS TO WHAT ALL FILES IT HAS AND THE PORT NUMBER IN A FILE MAYBE "server_directory.txt" IN WHATEVER FORMAT YOU WANT. */
                          FILE *fp2;
                          fp2 = fopen("server_directory.txt","a");
                          char peer[1000];
                         // strcat(buffer,"\n");
                         // strcpy(peer,buffer);
                          
                          fputs(buffer,fp2);
                          fputs("\n",fp2);
                          fclose(fp2);
                                     
              
                        }
                        else if(buffer[0] == '2')
                        {
                        	/* IF THE PACKET IS OF TYPE-2, WHAT IT IMPLIES IS THAT THERE IS A CLIENT OUT THERE THAT NEEDS A SPECIFIC FILE. SO, YOU HAVE TO GO THROUGH YOUR "server_directory.txt", AND CHECK WHICH ALL PORT NUMBERS HAVE THE REQUESTED FILE AND AS SOON AS YOU GET ONE ADD IT INTO A STRING. KEEP ADDING ALL SUCH PORT NUMBERS AND ATTACH A NUMBER 3 AT THE BEGINNING OF THIS STRING. THIS STRING IS NOW THE PACKET-3 SEND THIS BACK TO THE CLIENT WHICH ASKED FOR THE AFOREMENTIONED FILE */
                         char s[]=" ";
                         char *name;
                         char *port;
                         char buff[10];
                         puts("packet2 recieved as : ");
                         strncpy(buff,buffer,7);
                         puts(buff);
                         name = strtok(buff,s);
                         name = strtok(NULL,s);
                    /*     FILE *fp;
                         fp = fopen("server_directory", "r");
                         char line[200];
                          while ( fgets ( line, 200, fp ) != NULL ) {
                          if(strstr(line,name)){
                              
                                  port = strtok(line,s);
                                  port = strtok(NULL,s);            
                           }
                          }

         FILE *fp; 
       //char buffer[]="2 a.txt bjsdfh";
       char s[]=" ";
       char name[5];
       char port[4];
       char ch;
       int w;
       puts(buffer);
       for(w=3;w<8;w++)
       {
         name[w-3]=buffer[w];
       }
        //name[6]='\0';
       //name = strtok(buffer,s);
      // name = strtok(NULL,s);*/
       FILE *fp;
       char ch;
       puts("filename extracted from packet2 as : ");
       puts(name);
       fp = fopen("server_directory.txt", "r");
       char line[70];
       int tr=0,nt;
      while((ch=fgetc(fp))!=EOF){
       
    if(ch == '\n'){
            line[tr]='\0';
            
             if(strstr(line,name)){
                  puts("this is the line on which filename was found : ");
                  puts(line);
                  port = strtok(line,s);
                  port = strtok(NULL,s); 
            // for(nt=0;nt<5;nt++)
            // {
             //  port[nt]=line[nt+2];
            // }
              // port[5]='\0';
              }
            tr=0;
            }

           
            line[tr]=ch;
            tr++;
         }
          puts("portnumber extracted as : ");
          puts(port);
          fclose ( fp );
                         char packet3[]="3 ";
                         
                         strcat(packet3,port);
                         puts("packet3 has been created as : ");
                         puts(packet3);
                        // printf("%ld",sizeof packet3);
                        // printf("%ld",strlen(packet3));
                         send(i, packet3,strlen(packet3), 0);
                         
                        }
                    }
                }
            }
        }
    }
    return 0;
}


