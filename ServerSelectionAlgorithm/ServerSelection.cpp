//
// Created by ravikiran on 12/26/20.
//

#include "ServerSelection.h"
#include "IServerSelectionAlgorithm.h"
#include "RoundRobin/RoundRobin.h"
#include "../Logging/logger.h"

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
    /*
    * TODO: Read from CM for selectionAlgo;
    */
    traceDebug("Selected Round Robin...");
    mServerSelectionAlgorithm = new RoundRobin();
}

AppServerInfo* ServerSelection::getAppServerInfo(std::pair<Socket *, std::string> request)
{
    return mServerSelectionAlgorithm->getAppServerInfo(request);
}