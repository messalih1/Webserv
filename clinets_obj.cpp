#include "clinets_obj.hpp"




clinets_obj::clinets_obj()
{
     
 
    buffer.resize(BUFFER_SIZE);
}

clinets_obj::clinets_obj(string str):buffer(str)
{
    
}

clinets_obj::~clinets_obj()
{
}