//
// Created by ravi on 12/8/20.
//

#ifndef LOADBALANCER_NETWORKWORKER_H
#define LOADBALANCER_NETWORKWORKER_H

#include "../ThreadPool/ThreadPool.h"
#include "../ConnectionMapper/ConnectionMapper.h"

class NetworkWorker: public IThreadPool {
    virtual void process(std::pair<Socket*, std::pair<std::string, bool>>  request);
    void addConnectionMap(Socket* socket);
    void processResponse(std::pair<Socket *, std::string> response);
    void processRequest(std::pair<Socket *, std::string> response);
public:
    NetworkWorker(int threadNumber) : IThreadPool(threadNumber),
                                      mConnectionMapper(ConnectionMapper::getInstance()) {};

    virtual ~ NetworkWorker(){
        delete mConnectionMapper;
    }

protected:
    ConnectionMapper* mConnectionMapper;
};



#endif //LOADBALANCER_NETWORKWORKER_H
