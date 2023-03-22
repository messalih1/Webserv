#ifndef PARSSINGOFBODY_HPP
#define PARSSINGOFBODY_HPP

#include <iostream>

class parssingOfBody
{
    private:

    public:
        std::string             file;
        std::string             exetention;
        int                     index;
        int                     fd;

        parssingOfBody(/* args */);

        void handle_post(int len, std::string &headerOfRequest,std::string &bodyofRequest, unsigned long & ContentLength, unsigned long & i,int & flag_);
        
        ~parssingOfBody();
};


#endif