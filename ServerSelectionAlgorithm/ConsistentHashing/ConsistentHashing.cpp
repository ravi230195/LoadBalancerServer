//
// Created by ravikiran on 12/27/20.
//

#include "ConsistentHashing.h"
#include "../AppServerInfo/AppServerInfo.h"
#include "../../Logging/logger.h"
#include "../../ConnectionMapper/ConnectionMapper.h"
#include "../../ConfigurationManagement/ConfigurationManager.h"

void ConsistentHashing::readAppServerConfig() {
    /*
     * TODO: read From config file

    mAppServerData.push_back(new AppServerInfo("127.0.0.1", 9090, "AppServer1"));
    mAppServerData.push_back(new AppServerInfo("127.0.0.1", 9091, "AppServer2"));
    mAppServerData.push_back(new AppServerInfo("127.0.0.1", 9092, "AppServer3"));
    */

    for(auto serverInfo : ConfigurationManager::getInstance()->getAppServerInfo())
        mAppServerData.push_back(new AppServerInfo(serverInfo->getIp(), serverInfo->getPort(), serverInfo->getServerName()));
    traceDebug("Reading Succesful [%d]", mAppServerData.size());
    for (auto appServerInfo : mAppServerData) {
        traceDebug("ServerName [%s] with ip [%s] and port[%d] , serverHash [%u]", appServerInfo->getAppServerName().c_str(),
                   appServerInfo->getAppServerIP().c_str(),
                   appServerInfo->getAppServerPort(),
                   appServerInfo->getAppServerHash());
    }
}

void
ConsistentHashing::populateConsistentHashingRing()
{
    readAppServerConfig();
    std::lock_guard<std::mutex> lock(mConsistentHashLock);
    for(auto appServerInfo : mAppServerData)
        mRing[appServerInfo->getAppServerHash()] = appServerInfo;
}

AppServerInfo*
ConsistentHashing::getAppServerInfo(std::pair<Socket *, std::string> request)
{
    size_t requestHash =  ConnectionMapper::hashFunction(request.first);
    std::string sendData = request.second + ":" + std::to_string(requestHash);
    traceDebug("clientHash [%u]", requestHash);
    std::lock_guard<std::mutex> lock(mConsistentHashLock);
    auto it = mRing.lower_bound(requestHash);
    if(it == mRing.end())
        return mRing.begin()->second;
    return it->second;
}