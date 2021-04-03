//
// Created by ravi on 12/7/20.
//
#include "ConnectionMapper.h"

using namespace std;

//static variable
ConnectionMapper* ConnectionMapper::mInstance = NULL;
mutex ConnectionMapper::mCreationLock;

ConnectionMapper* ConnectionMapper::getInstance() {
    if(mInstance == NULL) {
        lock_guard<mutex> lock(mCreationLock);
        if (mInstance == NULL)
            mInstance = new ConnectionMapper();
    }
    return mInstance;
}

bool ConnectionMapper::addConnection(Socket* connection, std::string hashStr)
{
    lock_guard<mutex> lock(mConnectionMapMutex);
    if (mMapper.find(hashStr) != mMapper.end()) {
        traceError("Connection Already Found.... please debug");
        return false;
    }

    mMapper[hashStr] = connection;
    return true;
}

bool ConnectionMapper::addConnection(Socket* connection) {
    string hashStr = to_string(hashFunction(connection));

    {
        lock_guard<mutex> lock(mConnectionMapMutex);
        if (mMapper.find(hashStr) != mMapper.end()) {
            traceError("Connection Already Found.... please debug");
            return false;
        }

        mMapper[hashStr] = connection;
        return true;
    }
}
Socket* ConnectionMapper::getConnection(const std::string uniqueId) {
    {
        lock_guard<mutex> lock(mConnectionMapMutex);
        if (mMapper.find(uniqueId) == mMapper.end()) {
            traceDebug("Client doesnt Doesnt Exits");
            return NULL;
        }
        return mMapper[uniqueId];
    }
}

void ConnectionMapper::removeConnection(const std::string uniqueId) {
    {
        lock_guard<mutex> lock(mConnectionMapMutex);
        mMapper.erase(uniqueId);
        delete mMapper[uniqueId];
    }
}
