#include "parssingOfBody.hpp"
#include "clients_obj.hpp"
#include <cstdlib>

#include <string.h>



parssingOfBody::parssingOfBody(/* args */)
{
    file = "";
 
}

void parssingOfBody::handle_post(int len, std::string &headerOfRequest, std::string &buffer, unsigned long &ContentLength, unsigned long &i, int &flag_)
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


void  parssingOfBody::handling_chunked_data(string &buffer,string &headerOfRequest, string &bodyofRequest, int & flag_)
{
    int dec,rtn;
    int i = 0;
 
    while (i < buffer.size())
    {
        if(isalnum(buffer[i]) || isalpha(buffer[i]))
        {
            int k = i;
            while (isalnum(buffer[i]) || isalpha(buffer[i]))
                i++;
            
            dec = std::stoul(buffer.substr(k,i), NULL, 16);
            i+=2;
            if(dec == 0 && flag_ == 0)
            {
                // dec = headerOfRequest.find("boundary");
                // if(dec != -1)
                // {
                //     i = 0;
                //     buffer = bodyofRequest;
                //     flag_ = 5;
                //     handling_form_data();
                //     return ;
                // }
                exetention = std::to_string(rand() % 100000);
                rtn = headerOfRequest.find("mp4");
                
                if(rtn != -1)
                    fd = open((char*)(file.append(exetention).append(".mp4").data()),O_CREAT | O_RDWR , 0777);
                rtn = headerOfRequest.find("jpg");
                if(rtn != -1)
                    fd = open((char*)(file.append(exetention).append(".jpg").data()),O_CREAT | O_RDWR , 0777);

                rtn = headerOfRequest.find("pdf");
                if(rtn != -1)
                    fd = open((char*)(file.append(exetention).append(".pdf").data()),O_CREAT | O_RDWR , 0777);
                rtn = headerOfRequest.find("text/plain"); // ?
                    
                if(rtn != -1)
                    fd = open((char*)(file.append(exetention).data()),O_CREAT | O_RDWR , 0777);// should handle any text file
                
                
                write(fd,(void*)(bodyofRequest.data()),bodyofRequest.size());
                flag_ = 10;
            }
            while (dec--)
            {
                bodyofRequest.push_back(buffer[i]);
                i++;
            }
            i += 2; 
        }
    }
    // buffer.erase(buffer.begin(),buffer.end());
    buffer.clear();
}

parssingOfBody::~parssingOfBody()
{
}