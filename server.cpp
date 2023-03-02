#include <iostream>
#include <iostream>
#include <algorithm>
#include <utility>
#include <memory>
#include <stack>
#include <vector>
 #include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::stack;


#define PORT  8080
#define MAX_EVENTS 10
#define BUFFER_SIZE 1024

// • Checking the value of errno is strictly forbidden after a read or a write operation.

//

void error(const char * msg)
{
    perror(msg);
    exit(0);
}

int main(int ac, char *argv[])
{
    int fd_server, new_socket, kq,optval,nevents;
    long read_from_new_socket;
    socklen_t len;
    struct sockaddr_in serv_struct;
    struct kevent ev_serv_set[MAX_EVENTS], ev_clt_set[MAX_EVENTS];
    char buffer[BUFFER_SIZE] = {0};

    // create socket
    fd_server = socket(AF_INET,SOCK_STREAM,0);
    if(fd_server < 0)
        error("error in opening socket");

    std::memset(&serv_struct, 0, sizeof(serv_struct));

    serv_struct.sin_family = AF_INET; 
    serv_struct.sin_addr.s_addr = INADDR_ANY;
    serv_struct.sin_port = htons(PORT);

    optval = 1;
    if(setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
        error("in setsockopt()");

    if (bind(fd_server,  reinterpret_cast<struct sockaddr *> (&serv_struct), sizeof(serv_struct)) < 0) 
        error("ERROR on binding");

        
    if (listen(fd_server, 10) < 0)
        perror("In listen()");
    
    len = sizeof(serv_struct);
    
    kq = kqueue();

    EV_SET(&ev_serv_set[0], fd_server, EVFILT_READ, EV_ADD, 0, 0, NULL);
    
    if ((kevent(kq, ev_serv_set, 1, NULL, 0, NULL)) < 0)// register the server socket for read events
        error("in kevent()");
    
    struct kevent kev;
    while(1)// allows the program to continuously monitor for events on the registered file descriptors.
    {
        nevents = kevent(kq, NULL, 0, ev_clt_set, MAX_EVENTS, NULL);// waiting for  server && client events
        if (nevents < 0) 
            error("in kevent()");
        if(nevents)
        {
            // cout << nevents << endl;
            for (int i = 0; i < nevents; i++) // loop for numbers of events occurs
            {
                if(ev_clt_set[i].ident == fd_server)// if new connection
                {
                    if ((new_socket = accept(fd_server, reinterpret_cast<struct sockaddr *> (&serv_struct), &len) ) < 0 )// will accept it
                        perror("In accept()");
                    
                    // Add the client socket to the kqueue
                    EV_SET(&kev, new_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);// prepar new connection , new clinet for incomming data
                    if (kevent(kq, &kev, 1, NULL, 0, NULL) == -1) // add new connection to kevents, 
                        error("in kevent()");
                    else
                        std::cout << "New client connected\n";
                }
                else 
                { 
                    // if incomming data will enter the first if because it is not new connection , will enter here and Handle data from client
                    int client_socket = ev_clt_set[i].ident;// have fd of the client
                
                    int nbytes = recv(client_socket, buffer, sizeof(buffer), 0);
                    if (nbytes == 0) 
                    {
                        // Client closed connection or error occurred
                        std::cout << "Client disconnected\n";
                        EV_SET(&kev, client_socket, EVFILT_READ, EV_DELETE, 0, 0, NULL);//In the example code you provided, EV_SET is used to modify an existing event, specifically to delete it from the kqueue.
                        kevent(kq, &kev, 1, NULL, 0, NULL); // to  stop monitoring the client socket for incoming data
                        close(client_socket);
                    }
                    else
                    {
                        std::cout << "Received data from client: " << buffer << "\n";
                        // send(client_socket, buffer, nbytes, 0);
                    }
                }
            }

        }
    }
}
        
        // char buffer[30000] = {0};
        // read_from_new_socket = read( new_socket , buffer, 30000);
        // cout << buffer << endl;
    
        // cout << "\n+++++++ Waiting for new connection ++++++++\n\n";
 
// 1 - serch for arguments of EV_SET
//  2 - kevents


    // when pass ev_set[0] to EV_SET , it assign server fd to ev_set[0] he kernel event to monitor for that file descriptor. 


// ident: A file descriptor or identifier of the event source. the server socket, so that the kqueue can monitor the server socket for new incoming connections.



// by EV_SET we assign to server socket read events for waiting an enent occur This allows the process (serve socket) to be notified when a new connection is made to the server socket.