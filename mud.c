#include <stdio.h> //stuff
#include <stdlib.h> //stuff
#include <string.h> //for the strings
#include <unistd.h> //close()
#include "flatfile.h" //for database stuff

#include <sys/types.h> //sockets
#include <sys/socket.h>
#include <sys/select.h>

#include <netinet/in.h>//socket stuff
#include <netdb.h>
#include <arpa/inet.h>    //close()

#include <errno.h>//for errors 
extern int errno;

//define important stuff
#define MAX_CLIENTS 3
#define PORT 1337

//define colors 
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


int main(){
    
    puts("started the server!");
    //declare variables and arrays and stuff
    struct sockaddr_in address;
    char buffer[256];
    char * message = "You have connected to the server!";
    int opt = 1;
    int sd, max_sd, i, j, activity, new_socket, readval, sendval, contains_linebr;
    fd_set readfds;
    //set up client sockets
    int client_socket[MAX_CLIENTS];
    //set all clients to 0
    for (i = 0; i < MAX_CLIENTS; i++) {  
        client_socket[i] = 0;  
     }  
    //initialize socket
    int server_socket = socket(AF_INET , SOCK_STREAM , 0);
    if(server_socket == -1){
        puts("failed");
    }
    printf("Server_socket running on socket descriptor #%d\n", server_socket);
    
    

    //this is just a good habit, it will work without this (lets multiple clients connect to server_socket)
    if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ){  
        exit(1);
    }  
   
    //set up adress info
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;

    //bind connection
    int connection = bind(server_socket, (struct sockaddr *) &address, sizeof(address));
    if(connection == -1){
        puts("error binding socket");
        exit(1);
    }
    printf("connection: %d\n", connection);

    //begin listening with up to 5 pending connections
    int start_listen = listen(server_socket, 5);
    if (start_listen == -1){
        puts("error listening on server_socket");
        exit(1);
    } 
    printf("start_listen: %d\n", start_listen);

    //get length of adress for later use
    int addrlen = sizeof(address);
   
    //infinite loop
    while(1){
        //set up file descriptors
        FD_ZERO(&readfds);
        FD_SET(server_socket, &readfds);
        memset(buffer, '\0', sizeof(buffer));
        
        //see which clients are connected
        for (i = 0; i < MAX_CLIENTS; i++){
            printf("client #%d is on socket descriptor #%d\n", i, client_socket[i]);
            //if socket is taken, add it to array
            if (client_socket[i] > 0){
                FD_SET(client_socket[i], &readfds);
                
                
            }
        }
        
            
        
        activity = select(FD_SETSIZE + 1, &readfds, NULL, NULL, NULL);
        if(activity < 0 ){
            puts("Error with select()");
            exit(1);
        }

        //see if connection is new connection to server;
        if(FD_ISSET(server_socket, &readfds)){

            if ((new_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)  {  
                puts("Error adding new client socket");
                exit(1);
            }
            
            printf(ANSI_COLOR_GREEN "new user connected on socket descriptor #%d\n" ANSI_COLOR_RESET, new_socket);

            for(i = 0; i < MAX_CLIENTS; i++){
                if(client_socket[i] == 0){
                client_socket[i] = new_socket;
                send(new_socket, "Welcome to the server!\n", sizeof("Welcome to the server!\n"), 0);
                break;
                } else if(i == MAX_CLIENTS-1){
                    send(new_socket, "Sorry, server full\n", sizeof("Sorry, server full\n"), 0);
                    close(new_socket);
                }
            }
        }
        //check status of other connections
        for(i = 0; i < MAX_CLIENTS; i++){
            sd = client_socket[i];
            if(FD_ISSET(sd, &readfds)){
                readval = read(sd, buffer, sizeof(buffer));
                //if readval = 0, client diconhected. remove them from array 
                if(readval == 0){
                    //printf("Client #%d has disconnected.\n", i);
                    client_socket[i] = 0;
                    close(sd);
                
               
            } 
            //user entered data, read it and process it
            else{

                //check for line break
                contains_linebr = 0;
                for(j = 0; j < sizeof(buffer); j++){
                    if (buffer[j] == '\n'){
                        buffer[j+1] = '\0';
                        contains_linebr = 1;
                    }
                    
                }
                if (contains_linebr){
                    //============================this section is run for each user who entered something=======================
                printf("A message came in on socket descriptor #%d\n", sd);
                
                
                sendval = send(client_socket[i], buffer, sizeof(buffer), 0);
                if (sendval < 0){
                    puts("Error writing to client");
                    exit(1);
                }


                //=====================================================================================================
            }
                
            }


            }
        }//end of regular clients

        fflush(stdout);
    }//end of while()
    
    
return(1);
}//end of main()



