#ifndef LUNCH_SERVER_HPP
#define LUNCH_SERVER_HPP

#include <iostream>
#include <iostream>
#include <algorithm>
#include <utility>
#include <memory>
#include <stack>
#include <vector>
#include <map>
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
#include <type_traits>


#include "clinets_obj.hpp"


using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::stack;


#define PORT  8080
#define MAX_EVENTS 200

class lunch_server
{
    friend class clinets_obj;

    private:
        int fd_server;
        int fd_new_client;
        int  kq;
        socklen_t len;
        int optval;
        int nevents;
        long read_from_new_socket;
        struct sockaddr_in serv_struct;
        struct kevent server_evets[MAX_EVENTS];
        struct kevent clients_events[MAX_EVENTS];
        struct kevent kev;
        int total_by ;
        int bytes_red;
        // clinets_obj client_obj;
        std::map<int,clinets_obj> client_obj;
        

    public: 
        lunch_server();
        void create_server_socket();
    

        ~lunch_server();
        class throwException : public std::exception
        {
            const char * what () const throw ();
        };
};




// create throw



/*

 std::map<int,clinets_obj> m;

    m.insert(std::make_pair<int,clinets_obj>(10,clinets_obj("hello")));
    m.insert(std::make_pair<int,clinets_obj>(20,clinets_obj("test")));
 
    cout << m[10].buffer << endl;
    cout << m[20].buffer << endl;
*/

#endif