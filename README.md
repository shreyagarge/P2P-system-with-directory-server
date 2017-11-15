# P2P-system-with-directory-server


Client side :

1.port number of the peer is entered as an argument when the client is spawned
2.it is stored in port_number.
3.server_sock is the socket that this client uses to communicate with the dedicated server.
4.use system function of c to list out all the files present in the pwd of the client into a file directory.txt from where you can construct a packet 1 by appending all the files into a single string and then sending it to the server
5.fork the client program here  because it will be working both as a client as well as a server at any given time
6.if its a child process, make it run as a client-client. i.e. it takes in the file request and makes a packet-2 out of it and then sends it to the server
7.in return it gets the packet-3 i mentioned in the server code.
8.from the packet-3, it selects the first (or any) port number of the peers and makes a tcp connection to it and becomes the client.
9.then it sends a packet2 again to this, and gets the file in return without involving the server
close the sockets.
10.all this has to be in a loop like while(1) this cycle has to run endlessly to keep recieving and sending requests without problems
11.if its the parent process, make it run as a client-server. i.e it now makes a socket welcomesocket and listens for incoming connections.
12.as soon as a connection comes in, it accepts that connection via newsocket
13.now all communication with the requesting client happens via this newsocket
14.so you again take in the filename from the client ,open up the file, and send the contents of the file.close the sockets.
15.same here as well, this has to be inside a loop while(1) , and has to run endlessly

Directory server side :

1.there are two file descriptor sets : master and read_f ds
2. master, holds all the socket descriptors that are currently connected, as well as the socket descriptor that is listening for new connections.
3.there is a structure which takes the incoming clients addresses and saves them temporarily and keeps passng them into for loop
4.and for all these addresses, listener socket is created and eaxh time setsockopt keeps setting their respective parameters to listener and it is bound there
5.after all the requests are heard, the connection is closed.
6.as soon as the client starts up, it sends a packet-1 to the server which it receives into buffer,the if checks if it is of packet-1 or packet-2 and acts accordingly
7.if its a packet-1, the server has to store the details of the new client as to what all files it has and the port number in a file maybe "server_directory.txt" in whatever format you want.
8.if the packet is of type-2, what it implies is that there is a client out there that needs a specific file. so, you have to go through your "server_directory.txt", and check which all port numbers have the requested file and as soon as you get one add it into  a string. keep adding all such port numbers and attach a number 3 at the beginning of this string. this string is now the packet-3 send this back to the client which asked for the aforementioned file
