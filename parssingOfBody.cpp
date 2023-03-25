#include "parssingOfBody.hpp"
#include "clients_obj.hpp"
#include <cstdlib>

#include <string.h>



parssingOfBody::parssingOfBody(/* args */)
{
    file = "";
 
}

void parssingOfBody::putDataTofile(string  data, string & bodyofRequest)
{
    int pos = data.find("filename=\"");
 
    if(pos != -1)
    {
        int t = pos + 10;
        while (data[t] != '"')
            t++;
        file =  data.substr(pos + 10,t - (pos + 10));
        fd = open((char*)(file.data()),O_CREAT | O_RDWR , 0777);
        pos = data.find("Content-Type:");
        while (data[pos] != '\r' && data[pos + 1] != '\n')
            pos++;
        pos += 4;
        while (pos < data.size())
        {
            bodyofRequest.push_back(data[pos]);
            pos++;
        }
        write(fd,(void*)(bodyofRequest.data()),bodyofRequest.size());
        file.clear();
        bodyofRequest.clear();
        close(fd);
    }
}

void parssingOfBody::handling_form_data(string& buffer, string &headerOfRequest, string &boundary,string & bodyofRequest ,int &total_bytes_received,unsigned long &ContentLength,  int & i, int & bytes_received)
{
    // if(flag_ == 5)
    // {
    //     j = headerOfRequest.find("boundary");
    //     tmp = j + 9;
       
    //     char *temp = (char*)headerOfRequest.data() + tmp;// because string() dont handle '\r'
    //     tmp = 0;
    //     while (temp[tmp] != '\r' && temp[tmp + 1] != '\n')
    //         tmp++;
    //     boundary.append("--").append(ft_substr(temp,0,tmp));// free boundry and temp?
         
    //     total_bytes_received = ContentLength;
    //     i = 0;
    //     bodyofRequest.clear();
    // }
 
    // else
        total_bytes_received += bytes_received;
    if(total_bytes_received >= ContentLength)
    { 
        
        size_t start_idx = i;
        string separator = boundary;
        vector<string> substrings; // clear ?

        while (true) 
        {
            size_t end_idx = buffer.find(separator, start_idx);
            if (end_idx == string::npos) 
            {
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
                putDataTofile(*it,bodyofRequest);
            it++;
        }
    }
}



void parssingOfBody::handle_post(int len, std::string &headerOfRequest, std::string &buffer, unsigned long &ContentLength, int &i, int &flag_,int & client_socket)
{
    int rtn;
    
    i += len;
    if(i >= ContentLength )// finish recivng
    { 
        
        exetention = std::to_string(rand() % 100000);
        rtn = headerOfRequest.find("mp4");
        if(rtn != -1)
            fd = open((char*)(file.append(exetention).append(".mp4").data()),O_CREAT | O_RDWR , 0777);
        rtn = headerOfRequest.find("jpg");
        if(rtn != -1)
            fd = open((char*)(file.append(exetention).append(".jpg").data()),O_CREAT | O_RDWR , 0777);
        rtn = headerOfRequest.find("jpeg");
        if(rtn != -1)
            fd = open((char*)(file.append(exetention).append(".jpeg").data()),O_CREAT | O_RDWR , 0777);

        rtn = headerOfRequest.find("pdf");
        if(rtn != -1)
            fd = open((char*)(file.append(exetention).append(".pdf").data()),O_CREAT | O_RDWR , 0777);
        rtn = headerOfRequest.find("text/plain"); // ?
            
        if(rtn != -1)
            fd = open((char*)(file.append(exetention).data()),O_CREAT | O_RDWR , 0777);// should handle any text file
        
        write(fd,(void*)(buffer.substr(headerOfRequest.size() + 3,ContentLength).data()),buffer.substr(headerOfRequest.size() + 3,ContentLength).size());
        
        close(fd);
    }
}


void  parssingOfBody::handling_chunked_data(string &buffer,string &headerOfRequest, string &bodyofRequest, int & flag_,  int & i,  int & bytes)
{
     
    int dec,rtn;
    int j = 0;
    while (j < bytes)
    {
        if(isalnum(buffer[i]) || isalpha(buffer[i]))
        {
            int k = i;
            while ((isalnum(buffer[i]) || isalpha(buffer[i])))
            {
                i++;
                j++;
            }
             
            dec = std::stoul(buffer.substr(k,i), NULL, 16);
             
            i+=2;
            j+=2;
            if(dec == 0 && flag_ == 0)
            {
                exetention = std::to_string(rand() % 100000);
                rtn = headerOfRequest.find("mp4");
               
                if(rtn != -1)
                    fd = open((char*)(file.append(exetention).append(".mp4").data()),O_CREAT | O_RDWR , 0777);
                rtn = headerOfRequest.find("jpeg");
                if(rtn != -1)
                    fd = open((char*)(file.append(exetention).append(".jpeg").data()),O_CREAT | O_RDWR , 0777);
                 
                rtn = headerOfRequest.find("pdf");
                if(rtn != -1)
                    fd = open((char*)(file.append(exetention).append(".pdf").data()),O_CREAT | O_RDWR , 0777);
                rtn = headerOfRequest.find("text/plain"); // ?
                    
                if(rtn != -1)
                    fd = open((char*)(file.append(exetention).data()),O_CREAT | O_RDWR , 0777);// should handle any text file
                
                write(fd,(void*)(bodyofRequest.data()),bodyofRequest.size());
                
                close(fd);
                flag_ = 10;
            }
            while (dec--)
            {
                bodyofRequest.push_back(buffer[i]);
                i++;
                j++;
            }
            i+=2; 
            j+=2; 
        }
        j++;
    }
}

parssingOfBody::~parssingOfBody()
{
}