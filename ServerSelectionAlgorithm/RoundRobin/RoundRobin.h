//
// Created by ravikiran on 12/26/20.
//

#ifndef LOADBALANCER_ROUNDROBIN_H
#define LOADBALANCER_ROUNDROBIN_H
#include "../IServerSelectionAlgorithm.h"
#include "vector"
#include "mutex"
class AppServerInfo;
class Socket;

class RoundRobin: public IServerSelectionAlgorithm {
    std::vector<AppServerInfo*> mAppServerData;
    std::mutex mRoundRobinLock;

public:
    RoundRobin(){
        readAppServerConfig();
    }

    virtual ~RoundRobin(){
        for (auto appServerInfo : mAppServerData) {
            delete appServerInfo;
        }
    }

    virtual AppServerInfo* getAppServerInfo(std::pair<Socket *, std::string> request);
    void readAppServerConfig();
};

#endif //LOADBALANCER_ROUNDROBIN_H
