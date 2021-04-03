//
// Created by ravikiran on 12/27/20.
//

#ifndef LOADBALANCER_ISERVERSELECTIONALGORITHM_H
#define LOADBALANCER_ISERVERSELECTIONALGORITHM_H
#include "AppServerInfo/AppServerInfo.h"

class Socket;
class IServerSelectionAlgorithm{
public:
    virtual AppServerInfo* getAppServerInfo(std::pair<Socket *, std::string> request) = 0;
    virtual ~IServerSelectionAlgorithm(){}
};
#endif //LOADBALANCER_ISERVERSELECTIONALGORITHM_H
