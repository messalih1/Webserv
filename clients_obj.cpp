#include "clients_obj.hpp"
 

// in evry default construct cretae empty string and when you want to fill add a variable to continue

clients_obj::clients_obj()
{
    total_bytes_received = total_bytes_received = 0;
    i  = flag = 0;
    // buffer.resize(BUFFER_SIZE);
    buffer = "";
}


int clients_obj::recv_from_evry_client(int client_socket,  struct kevent  kev,int len, const int   kq)
{
    total_bytes_received = 0;
    
    
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
    if(i >= ContentLength)
    {
        cout << buffer.substr(headerOfRequest.size() + 2,ContentLength);
        // cout << i << endl;
        // cout << ContentLength << endl;
         
        exit(0);
    }
    // cout << i << endl;
    return 1;
}


// int clients_obj::findContentLenght()
// {
   

//     return 0;

// }

void clients_obj::check_buffer()
{ 
    // while (buffer[i])
    // {
    //     if(buffer[i] == '\r' && buffer[i + 1] == '\n')
    //     {
    //         i+=2;
    //         if(buffer[i] == '\r' && buffer[i + 1] == '\n' && buffer[i + 2] == '\0')
    //         {
                 
    //             i -= 2;
    //             headerOfRequest =  buffer.substr(0,i) ;
    //             cout << headerOfRequest << endl;
    //             // if(headerOfRequest.find("Content"))
    //             //     cout << "CONTENT LENGHT FINDING\n";
    //             break;
    //         }
    //         // else
    //         //     i--;

    //     }
    //     i++;
    // }
    
}

clients_obj::~clients_obj()
{
}