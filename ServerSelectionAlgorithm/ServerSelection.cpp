//
// Created by ravikiran on 12/26/20.
//

#include "ServerSelection.h"
#include "IServerSelectionAlgorithm.h"
#include "RoundRobin/RoundRobin.h"
#include "ConsistentHashing/ConsistentHashing.h"
#include "../Logging/logger.h"
#include "../ConfigurationManagement/ConfigurationManager.h"

using namespace std;

ServerSelection* ServerSelection::mInstance = NULL;
IServerSelectionAlgorithm* ServerSelection::mServerSelectionAlgorithm = NULL;
mutex ServerSelection::mCreationLock;

ServerSelection * ServerSelection::getInstance() {
    traceDebug("GetInstance()...");
    if(mInstance == NULL)
    {
        lock_guard<mutex> lock(ServerSelection::mCreationLock);
        if(mInstance == NULL)
        {
            mInstance = new ServerSelection();
        }
    }
}

void ServerSelection::getServerSelection() {

    traceDebug("Selected Round Robin...");
    if(ConfigurationManager::getInstance()->getServerSelectionAlgorithm() == "RoundRobin")
        mServerSelectionAlgorithm = new RoundRobin();
    else if(ConfigurationManager::getInstance()->getServerSelectionAlgorithm() == "ConsistentHashing")
        mServerSelectionAlgorithm = new ConsistentHashing();
}

AppServerInfo* ServerSelection::getAppServerInfo(std::pair<Socket *, std::string> request)
{
    return mServerSelectionAlgorithm->getAppServerInfo(request);
}