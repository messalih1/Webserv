#ifndef PARSSINGOFHEADER_HPP
#define PARSSINGOFHEADER_HPP

#include "../clients_obj.hpp"

 
class parssingOfHeader
{
    private:

    public:

        parssingOfHeader();

        long long	ft_atoi(const char *str);


        int checkHeaderOfreq(int len);
        int checkHeaderOfreq_();
        int checkHeaderLine();
        int checkHeaders(int index);
        int checkKeyValue(char *token,const int & i);
        
        ~parssingOfHeader();
};















#endif