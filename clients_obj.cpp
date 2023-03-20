#include "clients_obj.hpp"
#include <fstream>


// in evry default construct cretae empty string and when you want to fill add a variable to continue

clients_obj::clients_obj()
{
    total_bytes_received  = 0;
    i  = flag = flag_  = j = 0;
    buffer = bodyofRequest = boundary = tempString = "";
  
    
    // fd = open("1",O_RDWR | O_CREAT, 0777);
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
    strcmp("Upgrade-Insecure-Requests:",temp) != 0 && strcmp("Sec-Fetch-Site:",temp) != 0 && strcmp("Sec-Fetch-Mode:",temp) != 0 && strcmp("Sec-Fetch-User:",temp) != 0 && strcmp("Sec-Fetch-Dest:",temp) != 0 && strcmp("Postman-Token:",temp) != 0 && strcmp("Transfer-Encoding:",temp) != 0 && strcmp("Expect:",temp) != 0) 
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
    string str = "";
    i = index; 
      
    while (i < headerOfRequest.size())// sometimes in content has '\0' , dont loop
    {
        str.push_back(headerOfRequest[i]);
        i++;
    }
    
    i  = 0;
    header = strtok((char*)(str.data()),"\n");
    
     
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


void clients_obj::putDataTofile(string data)
{
    int pos = data.find("filename=\"");
    if(pos != -1)
    {
        int t = pos + 10;
        while (data[t] != '"')
            t++;
        file =  data.substr(pos + 10,t - (pos + 10));
        MyFile.open(file);
        pos = data.find("Content-Type:");
        while (data[pos] != '\r' && data[pos + 1] != '\n')
            pos++;
        pos += 4;
        while (pos < data.size())
        {
            bodyofRequest.push_back(data[pos]);
            pos++;
        }
        
        MyFile << bodyofRequest;
        file.clear();
        bodyofRequest.clear();
        MyFile.close();
    }
    

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
                
                if(checkHeaderOfreq_() == -2)
                    return -2;

                i = headerOfRequest.find("Transfer-Encoding: chunked");  
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
    if(flag == 4)
    {
        total_bytes_received += bytes_received;
        if(total_bytes_received >= ContentLength)
        { 
            size_t start_idx = i;
            string separator = boundary;
            vector<string> substrings; // clear ?

            while (true) {
   
                size_t end_idx = buffer.find(separator, start_idx);
                if (end_idx == string::npos) {
         
                    substrings.push_back(buffer.substr(start_idx));
                    break;
                }
  
                substrings.push_back(buffer.substr(start_idx, end_idx - start_idx));
 
                start_idx = end_idx + separator.length();
            }
            
            substrings.erase(substrings.end() - 1);// remove "--" after last boundry
            string s;
            vector<string>::iterator it = substrings.begin();
            while (it != substrings.end())
            { 
                if(!it->empty())
                    putDataTofile(*it);
                it++;
            }
        }       
    }
    return 1;
}



clients_obj::~clients_obj()
{
}

