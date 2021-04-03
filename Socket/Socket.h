//
// Created by ravi on 12/1/20.
//

#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include "../Logging/logger.h"

const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;

class Socket {
    int mSocketFD;
    sockaddr_in m_addr;

public:
    Socket(){
        mSocketFD = -1;
        memset ( &m_addr, 0, sizeof ( m_addr ) );
    };
    virtual ~Socket()
    {
        traceDebug("Closing Socket [%d]", mSocketFD);
        if(!isValidPort())
            close(mSocketFD);
    }

    // connection Server
    bool create();
    bool bind(const int port);
    bool listen();
    bool accept(Socket& newSocket);

    // Client
    bool connect ( const std::string host, const int port );

    // data transfer
    bool receive(std::string& data);
    bool send(const std::string& data);

    inline bool isValidPort(){
        return mSocketFD != -1;
    }

    int getSocket()
    {
        return mSocketFD;
    }
    int mServerConnectPort;
    std::string mClientIp;
};


#endif //SOCKET_H
