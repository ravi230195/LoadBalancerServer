//
// Created by ravikiran on 4/7/21.
//

#include "ConfigurationManager.h"
#include "../Logging/logger.h"
#include <cassert>

using namespace std;
using namespace tinyxml2;
namespace {
    XMLElement* getXMLElement(string element)
    {
        traceDebug("Loading Configuration...");
        const char* fileName = "/home/ravikiran/CLionProjects/LoadBalancerServer/ConfigurationManagement/Configuration.xml";
        XMLDocument doc;
        doc.LoadFile(fileName);
        traceDebug("[%s]", doc.ErrorName());
        traceDebug("Loading Configuration success...");

        XMLElement *pRoot, *pParm, *pProcess, *pApp, *pLineFormat;
        pRoot = doc.FirstChildElement( "Configuration" );
        if ( !pRoot ) {
            // Parse parameters
            traceDebug("EROORRR");
        }
        traceDebug("[%s]", pRoot == NULL ? "T" : "F");
        return pRoot;
    }
}

const std::string AppServerInfoMO::getIp() const {
    return mIp;
}

void AppServerInfoMO::setIp(const std::string &mIp) {
    AppServerInfoMO::mIp = mIp;
}

int AppServerInfoMO::getPort() const {
    return mPort;
}

void AppServerInfoMO::setPort(int mPort) {
    AppServerInfoMO::mPort = mPort;
}

const std::string AppServerInfoMO::getServerName() const {
    return mServerName;
}

void AppServerInfoMO::setServerName(const std::string &mServerName) {
    AppServerInfoMO::mServerName = mServerName;
}

void AppServerInfoMO::setServerAlive(bool serverUp)
{
    mAlive = serverUp;
}
bool AppServerInfoMO::getServerAlive()
{
    return mAlive;
}


void AppServerInfoMO::displayServersInfo()
{
    traceDebug("Server Name [%s]", AppServerInfoMO::getServerName().c_str());
    traceDebug("Server Ip [%s]", AppServerInfoMO::getIp().c_str());
    traceDebug("Server Port [%d]", AppServerInfoMO::getPort());
    traceDebug("Server Port [%s]", AppServerInfoMO::getServerAlive() ? "T" : "F");
}
ConfigurationManager* ConfigurationManager::mConfigurationManager = NULL;
mutex ConfigurationManager::mCreationLock;

ConfigurationManager* ConfigurationManager::getInstance() {
    if(mConfigurationManager == NULL)
    {
        lock_guard<mutex> lock(mCreationLock);
        if(mConfigurationManager== NULL)
            mConfigurationManager = new ConfigurationManager();
    }
    return mConfigurationManager;
}


int ConfigurationManager::getNumberOfWorkerThreads() {
    lock_guard<mutex> lock(mConfigurationManagerLock);
    if(mNumberOfWorkerThreads == -1)
        setNumberOfWorkerThreads();
    return mNumberOfWorkerThreads;
}

void ConfigurationManager::setNumberOfWorkerThreads() {
    traceDebug("Loading Configuration...");
    const char* fileName = "/home/ravikiran/CLionProjects/LoadBalancerServer/ConfigurationManagement/Configuration.xml";
    XMLDocument doc;
    doc.LoadFile(fileName);
    traceDebug("[%s]", doc.ErrorName());
    XMLElement *root;
    root = doc.FirstChildElement( "Configuration" );
    if ( !root ) {
        assert("Error Parsing");
        return;
    }

    mNumberOfWorkerThreads = stoi(root->FirstChildElement("NumberOfWorkerHreads")->GetText());
}

const std::string ConfigurationManager::getServerSelectionAlgorithm() {
    lock_guard<mutex> lock(mConfigurationManagerLock);
    if(mServerSelectionAlgorithm.empty()) {
        setServerSelectionAlgorithm();
    }
    return mServerSelectionAlgorithm;
}

void ConfigurationManager::setServerSelectionAlgorithm() {
    traceDebug("Loading Configuration...");
    const char* fileName = "/home/ravikiran/CLionProjects/LoadBalancerServer/ConfigurationManagement/Configuration.xml";
    XMLDocument doc;
    doc.LoadFile(fileName);
    traceDebug("[%s]", doc.ErrorName());
    XMLElement *root;
    root = doc.FirstChildElement( "Configuration" );
    if ( !root ) {
        assert("Error Parsing");
        return;
    }
    mServerSelectionAlgorithm = root->FirstChildElement("serverSelectionAlgorithm")->GetText();
}

int ConfigurationManager::getLoadServerPublicPort() {
    lock_guard<mutex> lock(mConfigurationManagerLock);
    if(mLoadServerPublicPort == -1)
        setLoadServerPublicPort();
    return mLoadServerPublicPort;
}

void ConfigurationManager::setLoadServerPublicPort() {
    traceDebug("Loading Configuration...");
    const char* fileName = "/home/ravikiran/CLionProjects/LoadBalancerServer/ConfigurationManagement/Configuration.xml";
    XMLDocument doc;
    doc.LoadFile(fileName);
    traceDebug("[%s]", doc.ErrorName());
    XMLElement *root;
    root = doc.FirstChildElement( "Configuration" );
    if ( !root ) {
        assert("Error Parsing");
        return;
    }
    mLoadServerPublicPort = stoi(root->FirstChildElement("LoadServerPublicPort")->GetText());
}

int ConfigurationManager::getLoadServerPrivatePort() {
    lock_guard<mutex> lock(mConfigurationManagerLock);
    if(mLoadServerPrivatePort == -1)
        setLoadServerPrivatePort();
    return mLoadServerPrivatePort;
}

void ConfigurationManager::setLoadServerPrivatePort() {
    traceDebug("Loading Configuration...");
    const char* fileName = "/home/ravikiran/CLionProjects/LoadBalancerServer/ConfigurationManagement/Configuration.xml";
    XMLDocument doc;
    doc.LoadFile(fileName);
    traceDebug("[%s]", doc.ErrorName());
    XMLElement *root;
    root = doc.FirstChildElement( "Configuration" );
    if ( !root ) {
        assert("Error Parsing");
        return;
    }
    mLoadServerPrivatePort = stoi(root->FirstChildElement("LoadServerPrivatePort")->GetText());
}

std::vector<AppServerInfoMO*> ConfigurationManager::getAppServerInfo()
{
    lock_guard<mutex> lock(mConfigurationManagerLock);
    if(mServerInfo.empty()) {
        traceDebug("Loading Configuration...");
        const char *fileName = "/home/ravikiran/CLionProjects/LoadBalancerServer/ConfigurationManagement/Configuration.xml";
        XMLDocument doc;
        doc.LoadFile(fileName);
        traceDebug("[%s]", doc.ErrorName());
        XMLElement *root;
        root = doc.FirstChildElement("Configuration");
        if (!root) {
            assert("Error Parsing");
            return {};
        }
        XMLElement* appServerConfig = root->FirstChildElement("AppServer");
        while (appServerConfig)
        {
            traceDebug("Found Server");
            AppServerInfoMO* appServerInfoMo = new AppServerInfoMO();
            XMLElement* appServer = appServerConfig->FirstChildElement("AppServerConfiguration");
            appServerInfoMo->setIp(appServer->FirstChildElement("AppServerIp")->GetText());
            appServerInfoMo->setPort(stoi(appServer->FirstChildElement("AppServerPort")->GetText()));
            appServerInfoMo->setServerName(appServer->FirstChildElement("AppServerName")->GetText());
            if(appServer->FirstChildElement("AppServerUp") == NULL || !strcmp(appServer->FirstChildElement("AppServerUp")->GetText(), "True"))
                appServerInfoMo->setServerAlive(true);
            else
                appServerInfoMo->setServerAlive(false);

            appServerInfoMo->displayServersInfo();
            if(appServerInfoMo->getServerAlive())
                mServerInfo.push_back(appServerInfoMo);
            appServerConfig = appServerConfig->NextSiblingElement("AppServer");
        }
    }

    vector<AppServerInfoMO*> aliveServers;
    for(auto serverInfo : mServerInfo)
    {
        if(serverInfo->getServerAlive())
            aliveServers.push_back(serverInfo);
    }
    return aliveServers;
}