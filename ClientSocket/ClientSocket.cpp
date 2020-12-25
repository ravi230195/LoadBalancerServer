//
// Created by ravi on 12/8/20.
//

#include "ClientSocket.h"
#include "../Logging/logger.h"

ClientSocket::ClientSocket ( std::string host, int port ) {
    if (!Socket::create()) {
        traceError("Client create failed \n");
    }

    if (!Socket::connect(host, port)) {
        traceError("Client connect failed \n");
    }
}

