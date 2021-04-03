//
// Created by ravi on 12/8/20.
//

#include "NetworkWorker.h"
#include "../ClientSocket/ClientSocket.h"
#include "../ServerSelectionAlgorithm/ServerSelection.h"
#include "../ServerSelectionAlgorithm/AppServerInfo/AppServerInfo.h"
#include <cassert>
#include <sstream>

using namespace std;
namespace {

    vector<string> split(string s, char delimiter)
    {
        vector<string> tokens;
        string token;
        istringstream tokenStream(s);
        while(getline(tokenStream, token, delimiter))
        {
            tokens.push_back(token);
        }
        return tokens;
    }
}

void NetworkWorker::addConnectionMap(Socket* socket) {
    traceDebug("Adding Connection..");
    if(!mConnectionMapper->addConnection(socket))
        assert(false);
    traceDebug("Added Connection..");
}

void NetworkWorker::process(std::pair<Socket *, std::pair<std::string, bool>> request) {
    traceDebug("Processing request [%s] response[%s]", request.second.first.c_str(), request.second.second ? "True" : "False");
    if(!request.second.second)
        processResponse({request.first, request.second.first});
    else
        processRequest({request.first, request.second.first});
}

void NetworkWorker::processRequest(std::pair<Socket *, std::string> request) {
    traceDebug("Processing Request...");
    addConnectionMap(request.first);
    /*
     * TODO: Get info from server selection algorithm
    */


    size_t clientHash =  ConnectionMapper::hashFunction(request.first);
    std::string sendData = request.second + ":" + to_string(clientHash);
    AppServerInfo* appServerInfo = ServerSelection::getInstance()->getAppServerInfo(request);
    traceDebug("Data [%s]", request.second.c_str());
    traceDebug("Data being sent [%s]", sendData.c_str());
    traceDebug("Request is being routed to [%s] with ip [%s] and port[%d]", appServerInfo->getAppServerName().c_str(), appServerInfo->getAppServerIP().c_str(), appServerInfo->getAppServerPort());
    ClientSocket* clientSocket = new ClientSocket(appServerInfo->getAppServerIP(), appServerInfo->getAppServerPort());
    clientSocket->send(sendData);
    std::string data;
    clientSocket->receive(data);
    if(data == "PROCESSING") {
        traceDebug("Closing Socket Worker Thread....");
        delete clientSocket;
    }
}

void NetworkWorker::processResponse(std::pair<Socket *, std::string> response) {

    response.first->send("RECEIVED");
    traceDebug("Data From APP Server : [%s] ", response.second.c_str());
    delete response.first;
    vector<string> res = split(response.second, ':');
    string uniqueId = res[0];
    traceDebug("unique Id [%s]", uniqueId.c_str());
    if(mConnectionMapper->getConnection(uniqueId))
    mConnectionMapper->getConnection(uniqueId)->send(res[1]);
    //traceDebug("resend the data : [%d] ", mConnectionMapper->getConnection(uniqueId)->getSocket());
    mConnectionMapper->removeConnection(uniqueId);
}
