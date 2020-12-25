
#include "iostream"
#include "Socket.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>


using namespace std;

bool Socket::create() {
    mSocketFD = socket(AF_INET, SOCK_STREAM, 0);


    if(!isValidPort()) {
        traceError("Socket connection fail \n");
        return false;
    }

    int on = 1;
    if ( setsockopt ( mSocketFD, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 )
        return false;
    traceDebug("Server connected on socket = [%d] ", mSocketFD);
    return true;
}

bool Socket::bind(const int port) {
    if(!isValidPort())
        return false;

    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_addr.sin_port = htons ( port );
    traceDebug("Server Binding on socket = [%d] ", mSocketFD);
    int bindReturn = ::bind(mSocketFD, (struct sockaddr*)&m_addr, sizeof(m_addr));
    if(bindReturn < 0)
        return false;

    return true;
}

bool Socket::listen() {
    if(!isValidPort())
        return false;

    int listenReturn = ::listen(mSocketFD, MAXCONNECTIONS);
    if(listenReturn == -1)
        return false;
    return true;
}

bool Socket::accept(Socket &newSocket) {
    int addrlen = sizeof(m_addr);
    newSocket.mSocketFD = ::accept(mSocketFD, (struct sockaddr*)&m_addr, (socklen_t*)&addrlen);
    if(newSocket.mSocketFD <= 0)
        return false;
    return true;
}

bool Socket::send(const string& data){
    int status = ::send (mSocketFD, data.c_str(), data.size(), MSG_NOSIGNAL );
    if ( status == -1 )
        return false;
    return true;
}

bool Socket::receive(std::string &data){
    char dataBuffer[MAXRECV + 1];
    memset ( dataBuffer, 0, MAXRECV + 1 );
    int status = ::read (mSocketFD, dataBuffer, MAXRECV);
    if ( status == 0 )
        return false;
    data = dataBuffer;
    return true;
}

bool Socket::connect ( const std::string host, const int port )
{
    if ( ! isValidPort() ) return false;
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons ( port );

    int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );

    if ( errno == EAFNOSUPPORT )
        return false;

    status = ::connect ( mSocketFD, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );
    return status == 0;
}
