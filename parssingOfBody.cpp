#include "parssingOfBody.hpp"
#include "clients_obj.hpp"



parssingOfBody::parssingOfBody(/* args */)
{
    file = "";
    index = 0;// use rundom
}

void parssingOfBody::handle_post(int len, std::string &headerOfRequest, std::string &buffer, unsigned long &ContentLength, unsigned long &i, int &flag_)
{
    int rtn;
     
    i += len;
    if(i >= ContentLength )// finish recivng
    { 
        index++;
        exetention = std::to_string(index);
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

// void parssingOfBody::handle_post(int len, string &headerOfRequest,string &bodyofRequest , int & ContentLength, int & i,int & flag_)
// {
// }


parssingOfBody::~parssingOfBody()
{
}