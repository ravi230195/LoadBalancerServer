//
// Created by ravikiran on 12/26/20.
//

#ifndef LOADBALANCER_SERVERSELECTION_H
#define LOADBALANCER_SERVERSELECTION_H


#include <string>
#include "mutex"

class AppServerInfo;
class IServerSelectionAlgorithm;
class Socket;

class ServerSelection {
public:
    enum selectionAlgorithm{
        ROUNDROBIN
    };

    virtual ~ServerSelection(){
    }
    static ServerSelection* getInstance();
    AppServerInfo* getAppServerInfo(std::pair<Socket *, std::string> request);
private:
    static ServerSelection* mInstance;
    static std::mutex mCreationLock;
    static IServerSelectionAlgorithm* mServerSelectionAlgorithm;
    ServerSelection(){
        getServerSelection();
    };
    static void getServerSelection();
};


#endif //LOADBALANCER_SERVERSELECTION_H
