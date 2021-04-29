# CS 342 : Computer Networks Lab (Group Number 78)

### 180101012 - ARYAN CHAUHAN  
### 180101037 - ANKET SANJAY KOTKAR  
  
  
## Implementing TFTP server  

### Packate Structure of the communicated packets are as follow:  
```
RRQ (read) or WRQ (write)   
| opcode (2B) | Filename |  0x00  | mode | 0x00     |  
| 1=RRQ,2=WRQ | (n bytes)|  1 byte|      | (1 byte) |  

DATA packet  
| opcode (2B) | Block number |  DATA         |  
| 3 = DATA    |  (2 bytes)   |  0-512 byte   |  
(All data packets contain 512 bytes except the last one (0-511 bytes))  
  
  
ACK packet   
| opcode  | Block number  |  
| 4 = ACK | (2 bytes)     |  
```

### Execution: 

In order to check the execution of the code:  
Put the server.c and client.c in seperate folders to check for the successful transfer of files.  

First start the server execution and then the client.  
In the folder with server.c, open the terminal and execute the following commands:  
```
$ gcc server.c -o server 
$ ./server <port_number>
```

In the folder with client.c, open the terminal and execute the following commands:  
```
$ gcc client.c -o client 
$ ./client <ip_addr> <port_number>  
```

In the place of _ip\_addr_, put your own _ip\_addr_ (of your machine). One should insert server IP in place of _ip\_addr_ if transferring files to some other PC, given that both server and client are connected to same network.  


For the implementation of the TFTP server, UDP socket programming was used.  

### In the implemented code, following functions are implemented:  

* GET <file_name> : Downloads the file with file_name from server onto the client.  
* PUT <file_name> : Uploads the file with file_name from client to the server.  
* Handle errors from server : If in some case, the requested file from GET does not exist on server, then server shows the error in the terminal that the requested file did not existed on the server and an empty file is created on the client.  
* EXIT : Exits the socket communication.  


### Testing of the program: The code was successfully tested to send the file of different formats like .png, .pdf, .txt etc.

### Assumptions:
* If the mentioned file is already present in the client, it will be overwritten.  
* Restarting may be done by the exit command or by simply killing the process (ctrl+C) and starting it again.  
* File names do not contain space.  
* Files that are to be transferred are not opened in some other application (like gedit). This is to prevent any type of wrong data exchange.  
* All the files are transferred to the folder containing the source files (client.c and server.c).  


### Additional assumptions:
*  The code is written in a way that it is supposed to use same port numbers on both server and client terminal. Doing this everything works fine. Otherwise timeout error occurs.
*  CLIENT and SERVER folders act like the server and client hosts. 
*  Well the files will be properly transferred if and only if they are in the CLIENT and SERVER folders. For files outside these folders it will not work as expected. It is like that those two folders are medium of exchange. And thus when using the names of the files use only the filename do not add additional path to it. Eg- In filename do not enter the absolute path of the file just its name would suffice.


