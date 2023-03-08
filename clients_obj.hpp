#ifndef CLIENTS_OBJ_HPP
#define CLIENTS_OBJ_HPP



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
#include <exception>
 #include <fstream>
#include<string>  


using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::stack;

#define BUFFER_SIZE 1024

class clients_obj
{
     private:
    public:
        string file;
        string buffer;
        string headerOfRequest;
        string bofyofRequest;
        int bytes_received;
        int total_bytes_received;
        long long i;
        std::ofstream MyFile;
        int flag;
        string index;
        long long ContentLength;
        clients_obj();

        int recv_from_evry_client(int client_socket,  struct kevent   kev, int len, int   kq);
        void check_buffer();
        int contentLeninReq();
        ~clients_obj();
         
};


// 20991



#endif

 