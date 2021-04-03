//
// Created by ravi on 12/2/20.
//

#include <memory>
#include "Server.h"
#include "../ServerSocket/ServerSocket.h"
#include "../ThreadPool/ThreadPool.h"

using namespace std;

const string welcome = "Connected Successfully\n";
const string receivedData = "Received Data Successfully\n";

Server::Server(int port):mPort(port) {
}

[[noreturn]] void Server::run(IThreadPool* threadPool) {
    mThreadPool = threadPool;
    unique_ptr<ServerSocket> serverSocket(new ServerSocket(mPort));
    serverSocket->connect();

    fd_set readfds;
    int serverFD = serverSocket->getSocket();
    traceDebug("Initializing Server ......");
    traceDebug("Server Up and Running....");

    while (true)
    {
        FD_ZERO(&readfds);
        FD_SET(serverFD, &readfds);
        int maxSocketFd = serverFD;
        for(auto connections : mConnections){
            FD_SET(connections->getSocket(), &readfds);
            maxSocketFd = max(maxSocketFd, connections->getSocket());
        }

        select(maxSocketFd + 1, &readfds,NULL, NULL, NULL);
        if(FD_ISSET(serverFD, &readfds)){
            Socket* newSocket = new ServerSocket();
            if(!serverSocket->accept(*newSocket))
                continue;
            mConnections.push_back(newSocket);
        }

        list<Socket*>::iterator it = mConnections.begin();
        traceDebug("Total Clients on Server [%d]", (int)mConnections.size());
        while(it != mConnections.end())
        {
            Socket* connections = *it;
            if(FD_ISSET(connections->getSocket(), &readfds))
            {
                string data;
                if(!connections->receive(data)) {
                    //Somebody disconnected , get his details and print
                    traceDebug("Host DISCONNECTED , ip %s , port %d ", connections->mClientIp.c_str(), connections->mServerConnectPort);

                    //Close the socket and mark as 0 in list for reuse
                    it = mConnections.erase(it);
                    continue;
                }
                else {
                    traceDebug("Host  ip [%s] , port [%d] , data [%s]", connections->mClientIp.c_str(), connections->mServerConnectPort, data.c_str());
                    mThreadPool->addToQueue(connections, data, mPort == 8081);
                }
            }
            ++it;
        }
    }
}