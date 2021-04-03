//
// Created by ravi on 12/2/20.
//

#include "ServerSocket.h"

using namespace std;
bool ServerSocket::connect() {
    if(!Socket::create())
        return false;

    if(!Socket::bind(mServerPort))
        return false;

    if(!Socket::listen())
        return false;

    return true;
}


bool ServerSocket::accept(Socket &serverSocket) {
    bool res = Socket::accept(serverSocket);

    if(res) {
        struct sockaddr_in address;
        int addrlen = sizeof(address);
        getpeername(serverSocket.getSocket(), (struct sockaddr *) &address, (socklen_t *) &addrlen);
        serverSocket.mServerConnectPort = ntohs(address.sin_port);
        serverSocket.mClientIp = inet_ntoa(address.sin_addr);
    }
    return res;
}
