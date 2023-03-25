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
#include<fcntl.h>  

#include "parssingOfHeader.hpp"
#include "parssingOfBody.hpp"

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::stack;

#define BUFFER_SIZE 1024

class parssingOfHeader;
class parssingOfBody;
class clients_obj
{
     private:
    public:
        string              file;
        string              buffer;
        string              headerOfRequest;
        string              bodyofRequest;
        string              tempString;
        string              boundary;
        string              index;
        std::ofstream       MyFile;

        int                 bytes_received;
        int                 total_bytes_received;
        int                 flag;
        int                 flag_;
        int                 tmp;
        int                 fd;
        int                 j;
        int       i;
        unsigned long       ContentLength;

        clients_obj();
       
        int recv_from_evry_client(int client_socket,  struct kevent   kev, int len, int   kq);
        
        
        long long	ft_atoi(const char *str);
        char *ft_substr(char const *s, unsigned int start, size_t len);
        
        int checkHeaderOfreq(int & len);
       
        int pushToBuffer(int client_socket,  struct kevent  kev,int len, const int   kq);
 
        void putDataTofile(string data);
        
        void handling_form_data();
 

        parssingOfHeader headerParss;
        parssingOfBody     bodyParss;
        ~clients_obj();
         
};


#endif


