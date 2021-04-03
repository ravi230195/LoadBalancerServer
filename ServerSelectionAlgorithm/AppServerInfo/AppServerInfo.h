//
// Created by ravikiran on 12/27/20.
//

#ifndef LOADBALANCER_APPSERVERINFO_H
#define LOADBALANCER_APPSERVERINFO_H

#include "string"

class AppServerInfo {
private:
    std::string mIp;
    int mPort;
    size_t mHash;
    std::string mServerName;
    int mRequestServed;

public:
    AppServerInfo(std::string ip, int port, std::string name):mIp(ip), mPort(port), mServerName(name){
        std::string keyStr = mIp +  " " +  std::to_string(mPort);
        mHash = std::hash<std::string>{}(keyStr);
    }

    std::string getAppServerIP();
    std::string getAppServerName();
    int getAppServerPort();
    size_t getAppServerHash();
};


#endif //LOADBALANCER_APPSERVERINFO_H
