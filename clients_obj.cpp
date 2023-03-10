#include "clients_obj.hpp"
 #include <fstream>


// in evry default construct cretae empty string and when you want to fill add a variable to continue

clients_obj::clients_obj()
{
    total_bytes_received  = 0;
    i  = flag = flag_  = c = t= error = 0;
    buffer = "";
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
    temp = ft_substr(headerOfRequest.data(),j,i); // free?
     
    if( strcmp(temp,"HTTP/1.1") != 0 )
    {
        free(temp);
        return -1;
    }
    free(temp);
    
    return i + 2;
}
  
int clients_obj::checkKeyValue(char *token, int & i, int & j)
{
    char *temp;
    while (token[i] && token[i] != ' ')
        i++;
    
    temp = ft_substr(token,j,i);
     
    if( strcmp("Host:",temp) != 0 && strcmp("User-Agent:",temp) != 0 && strcmp("Accept:",temp) != 0 && strcmp("Accept-Language:",temp) != 0 && strcmp("Accept-Encoding:",temp) != 0 && strcmp("Authorization:",temp) != 0 && strcmp("Cache-Control:",temp) != 0 && 
    strcmp("Connection:",temp) != 0 && strcmp("Content-Length:",temp) != 0 && strcmp("Content-Type:",temp) != 0   && strcmp("sec-ch-ua:",temp) != 0 && strcmp("sec-ch-ua-mobile:",temp) != 0 && strcmp("sec-ch-ua-platform:",temp) != 0 &&
    strcmp("Upgrade-Insecure-Requests:",temp) != 0 && strcmp("Sec-Fetch-Site:",temp) != 0 && strcmp("Sec-Fetch-Mode:",temp) != 0 && strcmp("Sec-Fetch-User:",temp) != 0 && strcmp("Sec-Fetch-Dest:",temp) != 0) 
    {
        free(temp);
        return -1;
    }
    free(temp);
    
    temp = ft_substr(token,i + 1,strlen(token - j));
     
    if(strlen(temp) == 0 || temp[strlen(temp) - 1] != '\r')
    {
        free(temp);
        return -1;
    }
    free(temp);
    return 1;
}

int clients_obj::checkHeaders(int index)
{
    // when add char after \r\n will add in first of the second line
    string str;
    char *temp;
    int i;
    int j;

    str  = &headerOfRequest[index]; 
    i = j = 0;

    while (str[i])
    {
        j = i;
        while (str[i] && str[i] != ':')
            i++;
        i++;
        temp = ft_substr(str.data(),j,i);
        if( strcmp("Host:",temp) != 0 && strcmp("User-Agent:",temp) != 0 && strcmp("Accept:",temp) != 0 && strcmp("Accept-Language:",temp) != 0 && strcmp("Accept-Encoding:",temp) != 0 && strcmp("Authorization:",temp) != 0 && strcmp("Cache-Control:",temp) != 0 && 
        strcmp("Connection:",temp) != 0 && strcmp("Content-Length:",temp) != 0 && strcmp("Content-Type:",temp) != 0   && strcmp("sec-ch-ua:",temp) != 0 && strcmp("sec-ch-ua-mobile:",temp) != 0 && strcmp("sec-ch-ua-platform:",temp) != 0 &&
        strcmp("Upgrade-Insecure-Requests:",temp) != 0 && strcmp("Sec-Fetch-Site:",temp) != 0 && strcmp("Sec-Fetch-Mode:",temp) != 0 && strcmp("Sec-Fetch-User:",temp) != 0 && strcmp("Sec-Fetch-Dest:",temp) != 0) 
        {
            if(strcmp("\r\n",temp) == 0)
            {
                free(temp);
                return 1;    
            }
            else
            {
                free(temp);
                return -1;
            }
        }
        i++;
        j = i;
        while (str[i] && str[i] != '\n')
            i++;
        temp = ft_substr(str.data(),j,i );
        if(strlen(temp) < 2 ||  temp[strlen(temp) - 1] != '\r')// && temp[strlen(temp) + 1] != '\n'))
        {
            free(temp);
            return -1;
        }
        free(temp);
        i++;
    }
    return 1;
}


int clients_obj::checkHeaderOfreq_()
{
    int rtn = checkHeaderLine();
    if(rtn == -1)
        return -1;
    rtn = checkHeaders(rtn);
    if(rtn == -1)
        return -1;
    return 1;
}

// check if empty request

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
                headerOfRequest = buffer.substr(0,pos);
                 
                if(checkHeaderOfreq_() == -1)
                    return -2;
                else
                    return 1;
                
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
        return -2;
}


int clients_obj::recv_from_evry_client(int client_socket,  struct kevent  kev,int len, const int   kq)
{
    int rtn;

    rtn = pushToBuffer(client_socket,  kev,len, kq);
  
    if(rtn == 0 || rtn == -1)
        return rtn;
    rtn = checkHeaderOfreq();
        
 
     
    return rtn;
}



clients_obj::~clients_obj()
{
}