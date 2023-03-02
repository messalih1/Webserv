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


using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::stack;


#define PORT  8080


// • Checking the value of errno is strictly forbidden after a read or a write operation.

void error(const char * msg)
{
    perror(msg);
    exit(0);
}

int main(int ac, char *argv[])
{
    int fd_server, new_socket;
    long read_from_new_socket;
    socklen_t len;

    struct sockaddr_in serv_struct;// empty struct and will fill them by infos of socket

    // create socket
    fd_server = socket(AF_INET,SOCK_STREAM,0);
    if(fd_server < 0)
        error("error in opening socket");
    
    // set all bytes of the structure to 0, we ensure that any padding bytes are also set to 0,
    // and that there are no uninitialized bytes in the structure.

    //     first arg is pointer
    std::memset(&serv_struct, 0, sizeof(serv_struct));

    serv_struct.sin_family = AF_INET; // ivp4
    serv_struct.sin_addr.s_addr = INADDR_ANY; // any ip can send to this server 
    serv_struct.sin_port = htons(PORT);

    // bind the socket to the server address
    /*
    assigning a specific IP address and port number to the socket created by 
    the server, so that the server can start listening for incoming connections from clients. 
    */

    if (bind(fd_server,  reinterpret_cast<struct sockaddr *> (&serv_struct), sizeof(serv_struct)) < 0) 
        error("ERROR on binding");

    if (listen(fd_server, 10) < 0)
        perror("In listen()");
    
    len = sizeof(serv_struct);
    
    while(1)
    {
        cout << "\n+++++++ Waiting for new connection ++++++++\n\n";
        if ((new_socket = accept(fd_server, reinterpret_cast<struct sockaddr *> (&serv_struct), &len) ) < 0 )
            perror("In accept()");
        
        char buffer[30000] = {0};
        read_from_new_socket = read( new_socket , buffer, 30000);
        cout << buffer << endl;
        
        close(new_socket);
         
        // break;
    }

    // use  setsockopt
}
 