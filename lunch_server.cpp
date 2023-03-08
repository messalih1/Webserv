#include "lunch_server.hpp"
#include "clients_obj.hpp"
#include <fstream>

#include <fcntl.h>


lunch_server::lunch_server()
{
//    string buffer(1000);
    // buffer.resize(1000);
    char buff[BUFFER_SIZE] = {0};
    
    fd_server = socket(AF_INET,SOCK_STREAM,0);
     
    if(fd_server < 0)
        throw throwException();

    std::memset(&serv_struct, 0, sizeof(serv_struct));

    serv_struct.sin_family = AF_INET; 
    serv_struct.sin_addr.s_addr = INADDR_ANY;
    serv_struct.sin_port = htons(PORT);

    optval = 1;
    if(setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
        throw throwException();
    
    // fcntl(fd_server, F_SETFL, O_NONBLOCK);

    if (bind(fd_server,  reinterpret_cast<struct sockaddr *> (&serv_struct), sizeof(serv_struct)) < 0) 
        throw throwException();

        
    if (listen(fd_server, 10) < 0)
        throw throwException();
    
    len = sizeof(serv_struct);
    
    kq = kqueue();

    EV_SET(&server_evets[0], fd_server, EVFILT_READ, EV_ADD, 0, 0, NULL);
    
    if ((kevent(kq, server_evets, 1, NULL, 0, NULL)) < 0)// register the server socket for read events
        throw throwException();
    
    
    while(1)// allows the program to continuously monitor for events on the registered file descriptors.
    {
        nevents = kevent(kq, NULL, 0, clients_events, MAX_EVENTS, NULL);// waiting for  server && client events
         
        for (int i = 0; i < nevents; i++) // loop for numbers of events occurs
        {
            if(clients_events[i].ident == fd_server)// if new connection
            {
                if ((fd_new_client = accept(fd_server, reinterpret_cast<struct sockaddr *> (&serv_struct), &len) ) < 0 )// will accept it
                    throw throwException();
                
                // Add the client socket to the kqueue
                EV_SET(&kev, fd_new_client, EVFILT_READ, EV_ADD, 0, 0, NULL);// prepar new connection , new clinet for incomming data
                if (kevent(kq, &kev, 1, NULL, 0, NULL) == -1) // add new connection to kevents, 
                        throw throwException();
                else
                {
                    
                }
                    // cout << "New client connected\n";
            }
            else if( clients_events[i].filter == EVFILT_READ)
            {
                int client_socket = clients_events[i].ident;
                int rtnFromfnc = map_objs[client_socket].recv_from_evry_client(client_socket,kev,clients_events[i].data,kq);
            
            
                if(rtnFromfnc == -1)
                    throw throwException();
                else if(rtnFromfnc == 0)
                {
                    cout << "Client disconnected\n";
                    EV_SET(&kev, client_socket, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                    kevent(kq, &kev, 1, NULL, 0, NULL); 
                    close(client_socket);
                }
                // if(map_objs[client_socket].i >= map_objs[client_socket].ContentLength)
                // {
                //     cout << map_objs[client_socket].buffer.substr(map_objs[client_socket].headerOfRequest.size() + 2,map_objs[client_socket].ContentLength + map_objs[client_socket].headerOfRequest.size());
                // }
                
                 // else if(rtnFromfnc == 2)
                // {
 
                //     // handle request without body
                // }
                // else if(rtnFromfnc == 1)
                // {
                    // if(map_objs[client_socket].total_bytes_received >= map_objs[client_socket].ContentLength)
                // }

                     
                  
                
                //  parsing request => and check it the size | and check content lenght
            }
        }
    }
}

const char * lunch_server::throwException::what () const throw ()
{
	return "error ";
}
 

lunch_server::~lunch_server()
{
}
