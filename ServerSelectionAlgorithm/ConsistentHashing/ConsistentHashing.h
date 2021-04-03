//
// Created by ravikiran on 12/27/20.
//

#ifndef LOADBALANCER_CONSISTENTHASHING_H
#define LOADBALANCER_CONSISTENTHASHING_H

#include "vector"
#include "map"
#include "mutex"
#include "../IServerSelectionAlgorithm.h"
class AppServerInfo;
class Socket;

class ConsistentHashing: IServerSelectionAlgorithm {
public:
    ConsistentHashing(){
        populateConsistentHashingRing();
    }

    std::mutex mConsistentHashLock;
    std::vector<AppServerInfo*> mAppServerData;

    virtual AppServerInfo* getAppServerInfo(std::pair<Socket *, std::string> request);
    void readAppServerConfig();

private:
    std::map<size_t , AppServerInfo*> mRing;
    void populateConsistentHashingRing();
};


#endif //LOADBALANCER_CONSISTENTHASHING_H
