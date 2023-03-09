#include "clients_obj.hpp"
 #include <fstream>


// in evry default construct cretae empty string and when you want to fill add a variable to continue

clients_obj::clients_obj()
{
    total_bytes_received  = 0;
    i  = flag = flag_ = 0;
    buffer = "";
    file = "file";
}

long long	clients_obj::ft_atoi(const char *str)
{
	long long	res;
	long long	negative;

	negative = 1;
	res = 0;
	while (*str && (*str == ' ' || *str == '\n' || *str == '\t' ||
			*str == '\v' || *str == '\f' || *str == '\r'))
		++str;
	if (*str == '-')
		negative = -1;
	if (*str == '-' || *str == '+')
		++str;
	while (*str && *str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - 48);
		++str;
	}
	return (res * negative);
}

int clients_obj::pushToBuffer(int client_socket,  struct kevent  kev,int len, const int   kq)
{
    total_bytes_received = 0;
    index = std::to_string(client_socket);
    
    char data[len];
    bzero(data, len);
    bytes_received = recv(client_socket, &data, len, 0);
    if(bytes_received == -1)
        return -1;
    if(bytes_received == 0)
        return 0;
    int j = 0; 
    while (j < bytes_received)
    {
        buffer.push_back(data[j]);
        j++;
    }
    return 1;
}


int clients_obj::checkHeaderOfreq_()
{
    int j = 0;
    cout << "HRERE\n\n";
    char *token = strtok((char*)(headerOfRequest.data()),"\r\n");

    token = strtok(token," ");

    if(strcmp(token,"GET") != 0 && strcmp(token,"DELETE") != 0 && strcmp(token,"POST") != 0 )
        return -1;

    token = strtok(NULL," ");
    if(token[0] != '/')
        return -1;
   
    token = strtok(NULL," ");
    
    if(strcmp(token,"HTTP/1.1") != 0)
        return -1;

    return 1;

    // int j = 0;
    // char *token;
    // while (headerOfRequest[j])
    // {
    //     if(headerOfRequest[j] == )

    //     j++;
    // }
    

    return 1;
}

int clients_obj::checkHeaderOfreq()
{
    int pos = 0;
    if(buffer.size() <= 1) // if empty request
        return -1;
    while (buffer[pos] && flag == 0)// for entring one time
    {
        if(buffer[pos] == '\r' && buffer[pos + 1] == '\n')
        {
            pos += 2;
            if(buffer[pos] == '\r' && buffer[pos + 1] == '\n' )
            {
                headerOfRequest = buffer.substr(0,pos);
                // check header line and headers | send request from nc
                // send custom request to server
                if(checkHeaderOfreq_() == -1)
                    return -1;
                
                pos = headerOfRequest.find("Content-Length"); // return from the checkHeaderOfreq() the content lenght
                if(pos != -1)
                {
                    ContentLength = ft_atoi(headerOfRequest.substr(pos + 16,headerOfRequest.size()).c_str());
                    flag = 1;
                    i = headerOfRequest.size();
                    return  ContentLength;
                }
                else
                {
                    // without body
                    flag = 2;
                    return 0;
                }
            }
            --pos;
        }
        pos++;
    }
    
    if(flag == 1)
        return 1;
    else if(flag == 2)
        return 0;
    else
        return -1;
}

// check if empty request

int clients_obj::recv_from_evry_client(int client_socket,  struct kevent  kev,int len, const int   kq)
{
    int rtn;

    rtn = pushToBuffer(client_socket,  kev,len, kq);
    if(rtn == 0 || rtn == -1)
        return rtn;
    
    rtn = checkHeaderOfreq(); // first request has header and check this header
    if(rtn > 0 && flag == 1) // if has content lenght
    {
        if(flag_ == 0) // to enter here one time
        {
            rtn = headerOfRequest.find("mp4");
            if(rtn != -1)
                MyFile.open(file.append(index).append(".mp4"));
            rtn = headerOfRequest.find("jpeg");
            if(rtn != -1)
                MyFile.open("file.jpeg");
            rtn = headerOfRequest.find("pdf");
            if(rtn != -1)
                MyFile.open("file.pdf");
            flag_ = 1;
        }
        i += len;
        if(i >= ContentLength )// finish recivng
        {
            MyFile << buffer.substr(headerOfRequest.size() + 2,ContentLength);
            // cout << "recive from client: "  << client_socket << endl;
            MyFile.close();
        }
    }
    else if(rtn == 0)
    {
        // check header line and headers
         // withou budy
    }
    else
        return -2;
     
    
    return 1;
}



clients_obj::~clients_obj()
{
}