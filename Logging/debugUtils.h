//
// Created by ravi on 10/1/20.
//

#ifndef TWDEVELOPMENTTEST_DEBUGUTILS_H
#define TWDEVELOPMENTTEST_DEBUGUTILS_H

#include <vector>
#include <list>
#include <iostream>
#include <string.h>
#include "logger.h"

namespace utils
{

    template <typename Iterator>
    void print(Iterator first, Iterator second)
    {
        for(; first != second; ++first)
            traceDebug("  [%s]",(*first).c_str());
    }
}
#endif //TWDEVELOPMENTTEST_DEBUGUTILS_H
