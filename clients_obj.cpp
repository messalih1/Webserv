#include "clients_obj.hpp"
 #include <fstream>


// in evry default construct cretae empty string and when you want to fill add a variable to continue

clients_obj::clients_obj()
{
    total_bytes_received  = 0;
    i  = flag = flag_  = j = 0;
    buffer = bodyofRequest = "";
    file = "file";
}

char *ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*str;

	str = (char*)malloc(sizeof(*s) * (len - start + 1));
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (i >= start && j < len - start)
		{
			str[j] = s[i];
			j++;
		}
		i++;
	}
	str[j] = 0;
	return (str);
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

int clients_obj::checkHeaderLine()
{
    int i = 0;
    int j = 0;
    char *temp;
    
    j = i;
    while (headerOfRequest[i] && headerOfRequest[i] != ' ')
        i++;
    
    temp = ft_substr(headerOfRequest.data(),j,i);

    if( strcmp(temp,"GET") != 0 && strcmp(temp,"POST") != 0 && strcmp(temp,"DELETE") != 0)
    {
        free(temp);
        return -1;
    }
    free(temp);

    i++;
    j = i;
    while (headerOfRequest[i] && headerOfRequest[i] != ' ')
        i++;
    temp = ft_substr(headerOfRequest.data(),j,i);
    if(temp[0] != '/')
    {
        free(temp);
        return -1;
    }
    free(temp);

    i++;
    j = i;
    while (headerOfRequest[i] && headerOfRequest[i] != '\r' && headerOfRequest[i + 1] != '\n' )
        i++;
    temp = ft_substr(headerOfRequest.data(),j,i); 
     
    if( strcmp(temp,"HTTP/1.1") != 0 )
    {
        free(temp);
        return -1;
    }
    free(temp);
    i+=2;
    return i;
}
  
int clients_obj::checkKeyValue(char *header,const int & i)
{
    char *temp;
    
    temp = ft_substr(header,0,i);
 
    if( strcmp("Host:",temp) != 0 && strcmp("User-Agent:",temp) != 0 && strcmp("Accept:",temp) != 0 && strcmp("Accept-Language:",temp) != 0 && strcmp("Accept-Encoding:",temp) != 0 && strcmp("Authorization:",temp) != 0 && strcmp("Cache-Control:",temp) != 0 && 
    strcmp("Connection:",temp) != 0 && strcmp("Content-Length:",temp) != 0 && strcmp("Content-Type:",temp) != 0   && strcmp("sec-ch-ua:",temp) != 0 && strcmp("sec-ch-ua-mobile:",temp) != 0 && strcmp("sec-ch-ua-platform:",temp) != 0 &&
    strcmp("Upgrade-Insecure-Requests:",temp) != 0 && strcmp("Sec-Fetch-Site:",temp) != 0 && strcmp("Sec-Fetch-Mode:",temp) != 0 && strcmp("Sec-Fetch-User:",temp) != 0 && strcmp("Sec-Fetch-Dest:",temp) != 0 && strcmp("Postman-Token:",temp) != 0 && strcmp("Transfer-Encoding:",temp) != 0) 
    {
        free(temp); 
        return -2;
    }

    free(temp);
    
    temp = ft_substr(header,i + 1,strlen(header));
    
    if(temp[strlen(temp + 1)] != '\r' && temp[strlen(temp + 1)] != '\n')
        return -2;  
    return 1;
}

int clients_obj::checkHeaders(int index)
{
    // when add char after \r\n will add in first of the second line
    char *header;
    int i;

    // string str  = &headerOfRequest[index]; 
    i  = 0;
    header = strtok(&headerOfRequest[index],"\n");
    
    while (header != NULL)
    {
        i = 0;
        while (header[i] && header[i] != ' ')
            i++;
        if(checkKeyValue(header,i) == -2)
            return -2;
        header = strtok(NULL,"\n");
    }
    return 1;
}


int clients_obj::checkHeaderOfreq_()
{
    int rtn = checkHeaderLine();

    if(rtn == -1)
        return -1;
    
    rtn = checkHeaders(rtn);
    
    return rtn;
}

int clients_obj::checkHeaderOfreq()
{
    
    int pos = 0;
    
    while (buffer[pos] && flag == 0)// for entring one time
    {
        if(buffer[pos] == '\r' && buffer[pos + 1] == '\n')
        {
            pos += 2;
            if(buffer[pos] == '\r' && buffer[pos + 1] == '\n' )
            {
                headerOfRequest = buffer.substr(0,pos - 1);// not include \r\n
                
                if(checkHeaderOfreq_() == -2)
                    return -2;
                
                pos = headerOfRequest.find("Transfer-Encoding: chunked");  
                if(pos != -1)
                {
                    j = headerOfRequest.size() + 3;// because headerOfRequest just include \r, +1 to include also \n
                    flag = 3;
                    return 1;
                }
                pos = headerOfRequest.find("Content-Length");  
                if(pos != -1)
                {
                    ContentLength = ft_atoi(headerOfRequest.substr(pos + 16,headerOfRequest.size()).c_str());
                    if(ContentLength == 0)
                        return -2;
                    flag = 1;
                    i = headerOfRequest.size();
                    return  1;
                }
                else
                {
                    // without body
                    flag = 2;
                    return 1;
                }
            }
            --pos;
        }
        pos++;
    }
    
    // in entring second times
    if(flag == 1 || flag == 2 || flag == 3)
        return 1;
    else
        return -2;
}


int clients_obj::chanckedRequest(int index) // return from end of it 
{
    unsigned long decimalNum ;
    // start from end of header of request
    string hexString = "";
    string temp = &buffer[index];
    int i = 0;
    int j = 0;
    while (temp[i])
    {
        while (temp[i] && temp[i] != '\r')
        {
            hexString.push_back(temp[i]);
            i++;
        }
        decimalNum = std::stoul(hexString, NULL, 16);
        
        i+=2;
        while (j < decimalNum)
        {
            bodyofRequest.push_back(temp[i + j]);
            j++;
        }
 
        i+=2;
    }
    
    return index;
}

int clients_obj::recv_from_evry_client(int client_socket,  struct kevent  kev,int len, const int   kq)
{
    int rtn;

    rtn = pushToBuffer(client_socket,  kev,len, kq);
    
    if(rtn == 0 || rtn == -1)
        return rtn;
    
    rtn = checkHeaderOfreq();
    if(rtn == -2)
        return -2;
    if(flag == 1) // if has content lenght
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
    else if(flag == 2)
    {
        // check header line and headers
         // without budy
    }
    else if(flag == 3)// if is chenked
    {
        j += chanckedRequest(j);
    }
    
    return 1;
}



clients_obj::~clients_obj()
{
}

// trnsfere encoding