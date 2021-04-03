//
// Created by ravi on 12/8/20.
//

#ifndef LOADBALANCER_CLIENTSOCKET_H
#define LOADBALANCER_CLIENTSOCKET_H


#include "../Socket/Socket.h"
class ClientSocket : public Socket
{

public:
    ClientSocket ( std::string host, int port );
    virtual ~ClientSocket(){};
};



#endif //LOADBALANCER_CLIENTSOCKET_H
