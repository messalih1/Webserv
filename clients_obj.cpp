#include "clients_obj.hpp"
 #include <fstream>


// in evry default construct cretae empty string and when you want to fill add a variable to continue

clients_obj::clients_obj()
{
    total_bytes_received = total_bytes_received = 0;
    i  = flag  = 0;
    // buffer.resize(BUFFER_SIZE);
    buffer = "";
    file = "file";
}


int clients_obj::recv_from_evry_client(int client_socket,  struct kevent  kev,int len, const int   kq)
{
    total_bytes_received = 0;
    index = std::to_string(client_socket);
    
    char data[len];
    bzero(data, len);
    bytes_received = recv(client_socket, &data, len, 0);

    if(bytes_received == 0)
        return 0;
    int j = 0; 
    while (j < bytes_received)
    {
        buffer.push_back(data[j]);
        j++;
    }
    int pos = 0;
    while (buffer[pos] && flag == 0)
    {
        if(buffer[pos] == '\r' && buffer[pos + 1] == '\n')
        {
            pos += 2;
            if(buffer[pos] == '\r' && buffer[pos + 1] == '\n' )
            {
                headerOfRequest = buffer.substr(0,pos);
                pos = headerOfRequest.find("mp4");
                if(pos != -1)
                {
                    MyFile.open(file.append(index).append(".mp4"));
                }
                pos = headerOfRequest.find("jpeg");
                if(pos != -1)
                {
                    MyFile.open("file.jpeg");
                }
                pos = headerOfRequest.find("pdf");
                if(pos != -1)
                {
                    MyFile.open("file.pdf");
                }
                pos = headerOfRequest.find("Content-Length");
                if(pos != -1)
                {
                    ContentLength = atoi(headerOfRequest.substr(pos + 16,headerOfRequest.size()).c_str());
                    flag = 1;
                    i = headerOfRequest.size();
                    break;
                }
                // return 2;
            }
            --pos;
        }
        pos++;
    }
     
    i += len;
    if(i >= ContentLength )
    {
        // cout << ContentLength << endl;
        // MyFile.open(file.append(index));
        MyFile << buffer.substr(headerOfRequest.size() + 2,ContentLength);
        // cout << buffer.substr(headerOfRequest.size() + 2,ContentLength) << endl;

        MyFile.close();
  
    }
    // cout << i << endl;
    return 1;
}

 

clients_obj::~clients_obj()
{
}