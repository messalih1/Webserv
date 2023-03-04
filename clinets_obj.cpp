#include "clinets_obj.hpp"
#include "clinets_obj.hpp"


// in evry default construct cretae empty string and when you want to fill add a variable to continue

clinets_obj::clinets_obj()
{
    bytes_received = total_bytes_received = 0;
    // buffer.resize(BUFFER_SIZE);// first time when call the default constructor
}

clinets_obj::clinets_obj(string str)//:buffer(str)
{   
}
clinets_obj::clinets_obj(int newSize)//:buffer(str)
{   
}

int clinets_obj::reseve(string buff)
{
     

    // client_obj[client_socket].total_bytes_received += client_obj[client_socket].bytes_received;
    // client_obj[client_socket].buffer.resize(client_obj[client_socket].total_bytes_received);
    // client_obj[client_socket].buffer.append(buff);  
    // cout << "=> " << client_obj[client_socket].buffer << endl;
    return 1;
}

clinets_obj::~clinets_obj()
{
}