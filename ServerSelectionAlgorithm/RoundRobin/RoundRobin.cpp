//
// Created by ravikiran on 12/26/20.
//

#include "RoundRobin.h"
#include "../../Logging/logger.h"
using namespace std;
void RoundRobin::readAppServerConfig() {
    /*
     * TODO: read From config file
    */
    mAppServerData.push_back(new AppServerInfo("127.0.0.1", 9090, "AppServer1"));
    mAppServerData.push_back(new AppServerInfo("127.0.0.1", 9091, "AppServer2"));
    mAppServerData.push_back(new AppServerInfo("127.0.0.1", 9092, "AppServer3"));

    for (auto appServerInfo : mAppServerData) {
        traceDebug("ServerName [%s] with ip [%s] and port[%d] , serverHash [%u]", appServerInfo->getAppServerName().c_str(),
                   appServerInfo->getAppServerIP().c_str(),
                   appServerInfo->getAppServerPort(),
                   appServerInfo->getAppServerHash());
    }
}

AppServerInfo * RoundRobin::getAppServerInfo(std::pair<Socket *, std::string> request) {
    lock_guard<std::mutex> lock(mRoundRobinLock);
    unsigned int random = rand();
    return mAppServerData[random % mAppServerData.size()];
}