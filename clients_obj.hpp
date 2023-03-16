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
        string              file;
        string              buffer;
        string              headerOfRequest;
        string              bodyofRequest;
        string              tempString;
        string              index;
        std::ofstream       MyFile;

        int                 bytes_received;
        int                 total_bytes_received;
        int                 flag;
        int                 flag_;
        int                 j;
        unsigned long       i;
        unsigned long       ContentLength;
        int fd;

        clients_obj();
       
        int recv_from_evry_client(int client_socket,  struct kevent   kev, int len, int   kq);
        
        

        long long	ft_atoi(const char *str);
        
        int checkHeaderOfreq(int len);
        int checkHeaderOfreq_();
        int checkHeaderLine();
        int checkHeaders(int index);
        int pushToBuffer(int client_socket,  struct kevent  kev,int len, const int   kq);
        int checkKeyValue(char *token,const int & i);
        void chanckedRequest(int len);
        ~clients_obj();
         
};


// 20991

// if have an error in parssing should return a response
// should aassaign -1 to error and return 0 to desconnect
#endif

 
 // if post methode you need to check content leghent or transfer uncoding
 // if post check in first transfer uncoding, if not found check content len
 // if post and content len 0 => error

//  Transfer-Encoding in http 

