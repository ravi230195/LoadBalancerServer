//
// Created by ravi on 12/7/20.
//

#ifndef LOADBALANCER_CONNECTIONMAPPER_H
#define LOADBALANCER_CONNECTIONMAPPER_H

#include "unordered_map"
#include "../Socket/Socket.h"
#include "mutex"


class ConnectionMapper {
    std::unordered_map<std::string, Socket* > mMapper;
    std::mutex mConnectionMapMutex;
    static ConnectionMapper* mInstance;
    static std::mutex mCreationLock;
    ConnectionMapper(){};
public:
    static ConnectionMapper* getInstance();
    bool addConnection(Socket* connection);
    bool addConnection(Socket* connection, std::string hashStr);
    void removeConnection(const std::string uniqueId);
    Socket* getConnection(const std::string uniqueId);
    static size_t hashFunction(Socket* connection)
    {
        int serverPort = connection->mServerConnectPort;
        std::string keyStr = connection->mClientIp +  "." +  std::to_string(serverPort);
        size_t hash = std::hash<std::string>{}(keyStr);
        return hash;
    }
};


#endif //LOADBALANCER_CONNECTIONMAPPER_H
