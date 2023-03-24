#include "clients_obj.hpp"



// in evry default construct cretae empty string and when you want to fill add a variable to continue

clients_obj::clients_obj()
{
    total_bytes_received  = 0;
    i  = flag = flag_  = j = 0;
    tmp = 0;
    buffer = bodyofRequest = boundary = tempString = "";
    tmp = 0;
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
  
char * clients_obj::ft_substr(char const *s, unsigned int start, size_t len)
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
 
int clients_obj::pushToBuffer(int client_socket,  struct kevent  kev,int len, const int   kq)
{
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

int clients_obj::checkHeaderOfreq(int len)
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
                if(headerParss.checkHeaderOfreq_(headerOfRequest,tmp) == -2)
                    return -2;
                
                i = headerOfRequest.find("Transfer-Encoding: chunked");   // find way to check if boundry
                if(i != -1)
                { 
                    buffer.erase(buffer.begin(),buffer.begin() + pos + 2);
                    flag = 3;
                    return 1;
                }
                pos = headerOfRequest.find("Content-Length");  
                if(pos != -1)
                {
                    j = headerOfRequest.find("boundary");
                    if(j != -1)
                    {
                        flag = 4;
                        ContentLength = ft_atoi(headerOfRequest.substr(pos + 16,headerOfRequest.size()).c_str());
                        if(ContentLength == 0)
                            return -2;
                        i = headerOfRequest.size() + 3;// after herder
                        bytes_received -= i;
                        tmp = j + 9;
                        char *temp = (char*)buffer.data() + tmp;// because string() dont handle '\r'
                        tmp = 0;
                        while (temp[tmp] != '\r' && temp[tmp + 1] != '\n')
                            tmp++;
                        boundary.append("--").append(ft_substr(temp,0,tmp));// free boundry and temp?
                        
                         return 1;
                    }
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
    if(flag == 1 || flag == 2 || flag == 3 || flag == 4)
        return 1;
    else
        return -2;
}


int clients_obj::recv_from_evry_client(int client_socket,  struct kevent  kev,int len, const int   kq)
{
    int rtn;

    rtn = pushToBuffer(client_socket,  kev,len, kq);
     
    if(rtn == 0 || rtn == -1)
        return rtn;
    rtn = checkHeaderOfreq(len);
    if(rtn == -2)
        return -2;
    if(flag == 1) // if has content lenght
    {
        
        bodyParss.handle_post(len,headerOfRequest,buffer,ContentLength,i,flag_,client_socket);
    }
    
    else if(flag == 2)
    {
        // check header line and headers
         // without budy
    }
    if(flag == 3)// // handle chunked data when resend request
        bodyParss.handling_chunked_data(buffer,headerOfRequest,bodyofRequest,flag_);
    if(flag == 4)
        bodyParss.handling_form_data(buffer,headerOfRequest,boundary,bodyofRequest,total_bytes_received,ContentLength,i,bytes_received);
    
        
    return 1;
}



clients_obj::~clients_obj()
{
}

