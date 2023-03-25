#ifndef PARSSINGOFBODY_HPP
#define PARSSINGOFBODY_HPP

#include <iostream>

class parssingOfBody
{
    private:

    public:
        std::string             file;
        std::string             exetention;
        int                     fd;

        parssingOfBody(/* args */);
        void putDataTofile(std::string  data, std::string & bodyofRequest);
        void handling_form_data(std::string& buffer, std::string &headerOfRequest, std::string &boundary,std::string &bodyofRequest,  int &total_bytes_received,unsigned long &ContentLength, int  &, int &);
        void handle_post(int len, std::string &headerOfRequest,std::string &bodyofRequest, unsigned long & ContentLength, int &i,int & flag_,int &);
        void handling_chunked_data(std::string &buffer,std::string &headerOfRequest, std::string &bodyofRequest, int & flag_,  int & i, int &);
        ~parssingOfBody();
};


#endif