//
// Created by ravikiran on 12/27/20.
//

#include "AppServerInfo.h"


std::string AppServerInfo::getAppServerIP(){
    return mIp;
}

std::string AppServerInfo::getAppServerName(){
    return mServerName;
}

int AppServerInfo::getAppServerPort(){
    return mPort;
}

size_t AppServerInfo::getAppServerHash(){
    return mHash;
}