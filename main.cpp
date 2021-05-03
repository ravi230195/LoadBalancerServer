#include <iostream>
#include "unordered_map"
#include "Socket/Socket.h"
#include "LoadBalancerServer/Server.h"
#include "ThreadPool/ThreadPool.h"
#include "NetworkWorker/NetworkWorker.h"
#include "ConfigurationManagement/ConfigurationManager.h"

int main() {
    //utils::addThreadName(std::this_thread::get_id(), "main");
    Server* publicInterface = new Server(ConfigurationManager::getInstance()->getLoadServerPublicPort());
    Server* privateInterface = new Server(ConfigurationManager::getInstance()->getLoadServerPrivatePort());
    traceDebug("Server Public Port [%d]", ConfigurationManager::getInstance()->getLoadServerPublicPort());
    traceDebug("Server Private Port [%d]", ConfigurationManager::getInstance()->getLoadServerPrivatePort());
    IThreadPool* threadPool = new NetworkWorker(ConfigurationManager::getInstance()->getNumberOfWorkerThreads());

    std::thread thread1(&Server::run, publicInterface, threadPool);
    std::thread thread2(&Server::run, privateInterface, threadPool);


    thread1.join();
    thread2.join();
    delete publicInterface;
    delete privateInterface;
    delete threadPool;
    return 0;
}
