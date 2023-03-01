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

#include <iostream>
#include <fstream>

#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/event.h>

#define MAX_EVENTS 32

int main(int argc, char* argv[]) {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int kq, nev, nread, nwrite;
    struct kevent ev_set[MAX_EVENTS], ev_list[MAX_EVENTS];

    // Create socket and bind to a port
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Failed to create server socket" << std::endl;
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);
    
    if (bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to bind to port" << std::endl;
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_socket, SOMAXCONN) < 0) {
        std::cerr << "Failed to listen for incoming connections" << std::endl;
        return 1;
    }

    // Create kqueue instance
    kq = kqueue();
    
    if (kq < 0) {
        std::cerr << "Failed to create kqueue" << std::endl;
        return 1;
    }

    
    EV_SET(&ev_set[0], server_socket, EVFILT_READ, EV_ADD, 0, 0, NULL); // for monitiring the server
    if (kevent(kq, ev_set, 1, NULL, 0, NULL) < 0) {
        std::cerr << "Failed to add server socket to kqueue" << std::endl;
        return 1;
    }
    while (true) {
        // Wait for events
        nev = kevent(kq, NULL, 0, ev_list, MAX_EVENTS, NULL);
        if (nev < 0) {
            std::cerr << "Error in kevent()" << std::endl;
            continue;
        }

        for (int i = 0; i < nev; i++) 
        {
            if (ev_list[i].ident == server_socket) {
                // New connection detected
                client_socket = accept(server_socket, (struct sockaddr*) &client_addr, &client_addr_len);
                if (client_socket < 0) {
                    std::cerr << "Failed to accept connection from client\n";
                    continue;
                }

                // Add the client socket to the kqueue
                struct kevent kev;
                EV_SET(&kev, client_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
                if (kevent(kq, &kev, 1, NULL, 0, NULL) == -1) {
                    std::cerr << "Failed to add client socket to kqueue\n";
                    close(client_socket);
                    continue;
                }

                std::cout << "New client connected\n";
            } 
            else 
            {
                // Handle data from client
                int client_socket = ev_list[i].ident;
                char buffer[1024];
                int nbytes = recv(client_socket, buffer, sizeof(buffer), 0);
                if (nbytes <= 0) {
                    // Client closed connection or error occurred
                    std::cout << "Client disconnected\n";
                    EV_SET(&kev, client_socket, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                    kevent(kq, &kev, 1, NULL, 0, NULL);
                    close(client_socket);
                    continue;
                }

                // Process the data received from the client
                std::cout << "Received data from client: " << buffer << "\n";

                // Echo the data back to the client
                send(client_socket, buffer, nbytes, 0);
            }
}
    }
