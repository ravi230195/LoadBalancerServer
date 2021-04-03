#include <iostream>
#include "unordered_map"
#include "Socket/Socket.h"
#include "LoadBalancerServer/Server.h"
#include "ThreadPool/ThreadPool.h"
#include "NetworkWorker/NetworkWorker.h"


int main() {

    Server* publicInterface = new Server(8081);
    Server* privateInterface = new Server(8083);

    IThreadPool* threadPool = new NetworkWorker(50);

    std::thread thread1(&Server::run, publicInterface, threadPool);
    std::thread thread2(&Server::run, privateInterface, threadPool);


    thread1.join();
    thread2.join();
    delete publicInterface;
    delete privateInterface;
    delete threadPool;
    return 0;
}
