#include "lunch_server.hpp"
#include "clients_obj.hpp"
#include <fstream>

#include <fcntl.h>


lunch_server::lunch_server()
{
    char buff[BUFFER_SIZE] = {0};
    
    fd_server = socket(AF_INET,SOCK_STREAM,0);
     
    if(fd_server < 0)
        throw throwException("error in creating the socket");

    std::memset(&serv_struct, 0, sizeof(serv_struct));

    serv_struct.sin_family = AF_INET; 
    serv_struct.sin_addr.s_addr = INADDR_ANY;
    serv_struct.sin_port = htons(PORT);

    optval = 1;
    if(setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
        throw throwException("error in setsockopt()");
    
    fcntl(fd_server, F_SETFL, O_NONBLOCK);
    /*
    kqueue() in blocking mode can handle multiple clients, but it may not be the most efficient way to do so.

When kqueue() is used with blocking file descriptors, any I/O operation performed on a file descriptor will block the calling process until the operation completes. This means that if the program is waiting for I/O on one file descriptor, it cannot perform any other work while waiting, including handling I/O events on other file descriptors.

In a multi-client scenario, this can result in poor performance and scalability, as the program may need to wait for I/O on multiple file descriptors simultaneously. If multiple file descriptors are used with blocking mode, the program may need to create multiple threads or processes to handle each file descriptor independently.

To handle multiple clients efficiently, it is often better to use non-blocking file descriptors with kqueue(), as this allows the program to perform other work while waiting for I/O events on multiple file descriptors. This can enable a single thread or process to handle many connections simultaneously, improving performance and reducing overhead.

In summary, while kqueue() in blocking mode can handle multiple clients, it may not be the most efficient approach in scenarios where multiple file descriptors need to be monitored simultaneously. Using non-blocking file descriptors can provide better performance and scalability in these scenarios.
    */
    if (bind(fd_server,  reinterpret_cast<struct sockaddr *> (&serv_struct), sizeof(serv_struct)) < 0) 
        throw throwException("error in bind()");

        
    if (listen(fd_server, 10) < 0)
        throw throwException("error in listen()");
    
    len = sizeof(serv_struct);
    
    kq = kqueue();

    EV_SET(&server_evets[0], fd_server, EVFILT_READ, EV_ADD, 0, 0, NULL);
    
    if ((kevent(kq, server_evets, 1, NULL, 0, NULL)) < 0)// register the server socket for read events
        throw throwException("error in kevent()");
    
    
    while(1)// allows the program to continuously monitor for events on the registered file descriptors.
    {
        nevents = kevent(kq, NULL, 0, clients_events, MAX_EVENTS, NULL);// waiting for  server && client events
         
        for (int i = 0; i < nevents; i++) // loop for numbers of events occurs
        {
            if(clients_events[i].ident == fd_server)// if new connection
            {
                if ((fd_new_client = accept(fd_server, reinterpret_cast<struct sockaddr *> (&serv_struct), &len) ) < 0 )// will accept it
                    throw throwException("error in accept()");
                // Add the client socket to the kqueue
                EV_SET(&kev, fd_new_client, EVFILT_READ, EV_ADD, 0, 0, NULL);// prepar new connection , new clinet for incomming data
                if (kevent(kq, &kev, 1, NULL, 0, NULL) == -1) // add new connection to kevents, 
                        throw throwException("error in kevent()");
                else
                {
                    // cout << "New client connected\n";
                    
                }
            }
            else if( clients_events[i].filter == EVFILT_READ)
            {
                int client_socket = clients_events[i].ident;
                int rtnFromfnc = map_objs[client_socket].recv_from_evry_client(client_socket,kev,clients_events[i].data,kq);
                if(rtnFromfnc == -1)
                    throw throwException("error in rev()");
                else if(rtnFromfnc == 0)// ||rtnFromfnc == -2 )
                {
                    // cout << "Client disconnected\n";
                    EV_SET(&kev, client_socket, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                    kevent(kq, &kev, 1, NULL, 0, NULL); 

                    close(client_socket);
                }
                
                else if(rtnFromfnc == -2)
                {
                    cout << "error in the request\n";
                    // EV_SET(&kev, client_socket, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                    // kevent(kq, &kev, 1, NULL, 0, NULL); 
                    // Check the request method
                    // if (request.method() != boost::beast::http::verb::get) {
                    // // Respond with "Method not allowed" error
                    // }

                    // // Check the request URI
                    // if (request.target().empty() || request.target()[0] != '/') {
                    // // Respond with "Bad request" error
                    // }

                    // // Check the HTTP version
                    // if (request.version() != 10 && request.version() != 11) {
                    // // Respond with "HTTP version not supported" error
                    // }

                    // // Check the headers
                    // for (auto const& header : request) {
                    // // Check each header line
                    // if (header.name_string().empty() || header.value().empty()) {
                    //     // Respond with "Bad request" error
                    // }
                    // }
                    // string res =  "HTTP/1.1 404 OK\r\nContent-Type: text/plain\r\nContent-Length: 6\r\n\r\error in the request";
                    // send(client_socket, res.c_str(), res.size(), 0);
                }
            }
        }
    }
}

// const char * lunch_server::throwException::what () const throw ()
// {
// 	return "error ";
// }
 

lunch_server::~lunch_server()
{
}
