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


#define PORT  8881
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


    if(setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
        error("in setsockopt()");

    if (bind(fd_server,  reinterpret_cast<struct sockaddr *> (&serv_struct), sizeof(serv_struct)) < 0) 
        error("ERROR on binding");

        
    if (listen(fd_server, 10) < 0)
        perror("In listen()");
    

    len = sizeof(serv_struct);
    
    kq = kqueue();

    EV_SET(&ev_serv_set[0], fd_server, EVFILT_READ, EV_ADD, 0, 0, NULL);
    
    if ((kevent(kq, ev_serv_set, 1, NULL, 0, NULL)) < 0)
        error("in kevent()");
    
    while(1)
    {
        nevents = kevent(kq, NULL, 0, ev_clt_set, MAX_EVENTS, NULL);
        if (nevents < 0) {
            std::cerr << "Error in kevent()" << std::endl;
            continue;
        }
       
        for (int i = 0; i < nevents; i++) 
        {
            if(ev_clt_set[i].ident == fd_server)// if event occur in server
            {
                if ((new_socket = accept(fd_server, reinterpret_cast<struct sockaddr *> (&serv_struct), &len) ) < 0 )
                    perror("In accept()");
            }
            // Add the client socket to the kqueue
            struct kevent kev;
            EV_SET(&kev, new_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
            if (kevent(kq, &kev, 1, NULL, 0, NULL) == -1) {
                std::cerr << "Failed to add client socket to kqueue\n";
                close(new_socket);
                continue;
            }
            std::cout << "New client connected\n";
        }

        

        
        // char buffer[30000] = {0};
        // read_from_new_socket = read( new_socket , buffer, 30000);
        // cout << buffer << endl;
        
        close(new_socket);
         
        // break;
    }

    // use  setsockopt
}
    
        // cout << "\n+++++++ Waiting for new connection ++++++++\n\n";
 
// 1 - serch for arguments of EV_SET
//  2 - kevents


    // when pass ev_set[0] to EV_SET , it assign server fd to ev_set[0] he kernel event to monitor for that file descriptor. 


// ident: A file descriptor or identifier of the event source. the server socket, so that the kqueue can monitor the server socket for new incoming connections.



// by EV_SET we assign to server socket read events for waiting an enent occur This allows the process (serve socket) to be notified when a new connection is made to the server socket.