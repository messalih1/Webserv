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


using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::stack;

#define BUFFER_SIZE 1024

class clinets_obj
{
    friend class lunch_server;
    private:
        string buffer;
        int bytes_received;
        int total_bytes_received;
    public:
        clinets_obj();
        clinets_obj(string str);
        clinets_obj(int newSize);
        int reseve(string buff);
        ~clinets_obj();
         
};


// 20991



#endif

 