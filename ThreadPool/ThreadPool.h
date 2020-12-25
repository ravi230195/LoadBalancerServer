//
// Created by ravi on 12/2/20.
//

#ifndef LOADBALANCER_THREADPOOL_H
#define LOADBALANCER_THREADPOOL_H

#include "mutex"
#include "condition_variable"
#include "thread"
#include "vector"
#include "queue"
#include "../Socket/Socket.h"

class IThreadPool {
    int mThreadNumber;
    std::vector<std::thread> mThread;
    std::mutex mWorkingQueueMutex;
    bool mShutDown;
    std::condition_variable_any mWorkQueueConditionVariable;
    std::queue<std::pair<Socket*, std::pair<std::string, bool>>> mWorkerQueue;

    void createThreads();
    void doWork();
    virtual void process(std::pair<Socket*, std::pair<std::string, bool>> request) = 0;

public:
    IThreadPool(int threadNumber): mThreadNumber(threadNumber), mShutDown(false){
        traceDebug("Initializing ThreadPool.....");
        auto numberOfThreads = std::thread::hardware_concurrency();
        if(numberOfThreads == 0)
            mThreadNumber = 1;
        createThreads();
        traceDebug("ThreadPool up and running.....");
    };

    virtual ~IThreadPool()
    {
        mShutDown = true;
        mWorkQueueConditionVariable.notify_all();
        for (auto& worker : mThread) {
            if(worker.joinable())
                worker.join();
        }
    }

    void addToQueue(Socket* connection, std::string data, bool response = true);
};


#endif //LOADBALANCER_THREADPOOL_H
