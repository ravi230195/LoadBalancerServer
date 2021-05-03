//
// Created by ravi on 10/1/20.
//

#include <vector>
#include <list>
#include <iostream>
#include <string.h>
#include "thread"
#include <mutex>
#include "unordered_map"

using namespace std;
//static unordered_map<thread::id, string> mMap;
//static mutex mLock;
namespace utils
{

    template <typename Iterator>
    void print(Iterator first, Iterator second)
    {
        for(; first != second; ++first)
            traceDebug("  [%s]",(*first).c_str());
    }
//    string getThreadName(thread::id t)
//    {
//        lock_guard<mutex> lock(mLock);
//        return mMap[t];
//    }
//
//    void addThreadName(thread::id t, string name)
//    {
//        lock_guard<mutex> lock(mLock);
//        mMap[t] = name;
//    }
}

