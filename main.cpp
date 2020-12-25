#include <iostream>
#include "unordered_map"
#include "Socket/Socket.h"
#include "Server/Server.h"
#include "ThreadPool/ThreadPool.h"
#include "NetworkWorker/NetworkWorker.h"

//std::mutex threadMapperMutex;
//std::unordered_map<std::thread::id, std::string> threadNameMapper;
//void addThreadEntry(std::thread::id id, std::string name)
//{
//    std::lock_guard<std::mutex> lock(threadMapperMutex);
//    threadNameMapper[id] = name;
//}
//
//std::string getThreadId(std::thread::id id)
//{
//    std::lock_guard<std::mutex> lock(threadMapperMutex);
//    return threadNameMapper[id];
//}

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
