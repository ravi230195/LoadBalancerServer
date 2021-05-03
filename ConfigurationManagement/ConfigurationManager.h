//
// Created by ravikiran on 4/7/21.
//

#ifndef LOADBALANCER_CONFIGURATIONMANAGER_H
#define LOADBALANCER_CONFIGURATIONMANAGER_H
#include "../tinyxml2/tinyxml2.h"
#include "mutex"
#include "string.h"
#include "vector"

class AppServerInfoMO{

private:
    std::string mIp;
    int mPort;
    std::string mServerName;
    bool mAlive;
public:
    const std::string getIp() const;
    void setIp(const std::string &mIp);
    int getPort() const;
    void setPort(int mPort);
    const std::string getServerName() const;
    void setServerName(const std::string &mServerName);
    void setServerAlive(bool serverUp);
    bool getServerAlive();
    void displayServersInfo();

};

class ConfigurationManager {
    static ConfigurationManager* mConfigurationManager;
    static std::mutex mCreationLock;
    std::mutex mConfigurationManagerLock;

    int mNumberOfWorkerThreads;
    std::string mServerSelectionAlgorithm;
    int mLoadServerPublicPort;
    int mLoadServerPrivatePort;
    std::vector<AppServerInfoMO*> mServerInfo;
public:
    static ConfigurationManager* getInstance();

    int getNumberOfWorkerThreads();
    void setNumberOfWorkerThreads();

    const std::string getServerSelectionAlgorithm();
    void setServerSelectionAlgorithm();

    int getLoadServerPublicPort();
    void setLoadServerPublicPort();

    int getLoadServerPrivatePort();
    void setLoadServerPrivatePort();

    std::vector<AppServerInfoMO*> getAppServerInfo();

private:
    ConfigurationManager():mLoadServerPrivatePort(-1),
                            mLoadServerPublicPort(-1),
                            mNumberOfWorkerThreads(-1){

    }

};


#endif //LOADBALANCER_CONFIGURATIONMANAGER_H
