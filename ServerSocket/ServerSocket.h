//
// Created by ravi on 12/2/20.
//

#ifndef LOADBALANCER_SERVERSOCKET_H
#define LOADBALANCER_SERVERSOCKET_H

#include "../Socket/Socket.h"
#include <string.h>

class ServerSocket: public Socket {
    int mServerPort;
public:
    ServerSocket(int port){
        mServerPort = port;
    }

    ServerSocket (){};
    virtual ~ServerSocket(){};

    bool connect();
    bool accept(Socket& serverSocket);


};


#endif //LOADBALANCER_SERVERSOCKET_H
