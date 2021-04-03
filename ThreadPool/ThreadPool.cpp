 //
// Created by ravi on 12/2/20.
//

#include "ThreadPool.h"
#include "string"
#include <mutex>
#include "../ClientSocket/ClientSocket.h"

using namespace std;

void IThreadPool::createThreads() {
    for(int i = 0; i < mThreadNumber; ++i)
        mThread.push_back(std::thread(&IThreadPool::doWork, this));
}


void IThreadPool::doWork() {
    while (!mShutDown) {
        std::pair<Socket*, std::pair<std::string, bool>> request;
        {
            unique_lock<mutex> lock(mWorkingQueueMutex);
            mWorkQueueConditionVariable.wait(lock, [&] {
                return !mWorkerQueue.empty() || mShutDown;
            });
            request = mWorkerQueue.front();
            mWorkerQueue.pop();
        }
        process(request);
    }
}


void IThreadPool::addToQueue(Socket* connection, std::string data, bool response) {
    lock_guard<mutex> lock(mWorkingQueueMutex);
    traceDebug("Data queued [%s] response[%s]", data.c_str(), response ? "True" : "False");
    mWorkerQueue.push({connection, {data, response}});
    mWorkQueueConditionVariable.notify_one();
}
