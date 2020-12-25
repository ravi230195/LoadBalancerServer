//
// Created by ravi on 12/8/20.
//

#include "NetworkWorker.h"
#include "../ClientSocket/ClientSocket.h"
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
    if(!mConnectionMapper->addConnection(socket))
        assert(false);
}

void NetworkWorker::process(std::pair<Socket *, std::pair<std::string, bool>> request) {
    if(!request.second.second)
        processResponse({request.first, request.second.first});
    else
        processRequest({request.first, request.second.first});
}

void NetworkWorker::processRequest(std::pair<Socket *, std::string> request) {
    addConnectionMap(request.first);
    ClientSocket* clientSocket = new ClientSocket("127.0.0.1", 8089);
    traceDebug("Data [%s]", request.second.c_str());
    std::string sendData = request.second + ":" + ConnectionMapper::hashFunction(request.first);
    traceDebug("Data being sent [%s]", sendData.c_str());
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
    delete response.first;
    vector<string> res = split(response.second, ':');
    string uniqueId = res[0];
    mConnectionMapper->getConnection(uniqueId)->send(res[1]);
    mConnectionMapper->removeConnection(uniqueId);
}
