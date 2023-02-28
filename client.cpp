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
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>



using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::stack;


#define PORT  1233
 

void error(const char * msg)
{
    perror(msg);
    exit(0);
}

int main(int ac, char *argv[])
{
    int fdClient;
    long read_from_socket;

    struct sockaddr_in serv_addr;// empty struct and will fill them by infos of socket


    fdClient = socket(AF_INET,SOCK_STREAM,0);
    if(fdClient < 0)
        error("error in opening socket");

    std::memset(&serv_addr, 0, sizeof(serv_addr));

    // fill infos of the server

    serv_addr.sin_family = AF_INET; // ivp4
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");// ip of server

    if (connect(fdClient, reinterpret_cast<struct sockaddr *>(&serv_addr), sizeof(serv_addr)) < 0)
        error("error in connet()");
    
    send(fdClient , "hello" , strlen("hello") , 0 );
    cout << "Hello message sent\n" << endl;
    char buffer[1024] = {0};
    read_from_socket = read( fdClient , buffer, 1024);
    cout << buffer << endl;

}