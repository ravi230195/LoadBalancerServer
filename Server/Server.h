//
// Created by ravi on 12/2/20.
//

#ifndef LOADBALANCER_SERVER_H
#define LOADBALANCER_SERVER_H

#include "list"
class IThreadPool;
class Socket;
class Server {
    int mPort;
    std::list<Socket*> mConnections;
    IThreadPool* mThreadPool;
public:
    Server(int port);
    ~Server(){
    }

    [[noreturn]] void run(IThreadPool* threadPool);
};


#endif //LOADBALANCER_SERVER_H
