//
// Created by ravi on 10/1/20.
//

#ifndef LOGGER_H
#define LOGGER_H
#include <string.h>
#include <thread>
#include "unordered_map"

//TAGS
#define NEXTLINE     "\n"
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define ERROR_TAG   "ERROR"
#define INFO_TAG    "INFO"
#define DEBUG_TAG   "DEBUG"

#define ERROR     20
#define INFO      40
#define DEBUG     60

#ifndef LOG_LEVEL
#define LOG_LEVEL   DEBUG // Default
#endif

#define LOG_FMT             "%-2s | %-20d | %4d:%-25s | %-20s |"
#define LOG_ARGS(LOG_TAG)   LOG_TAG, std::this_thread::get_id(), __LINE__, __FUNCTION__, __FILENAME__

#define FILE "temp.txt"

#define PRINT(args, ...) fprintf(stderr, args, __VA_ARGS__)
#define PRINTCONSOLE(args, ...) fprintf(stderr, args, __VA_ARGS__)

#if LOG_LEVEL >= DEBUG
#define traceDebug(message, args...)     PRINT(LOG_FMT message NEXTLINE, LOG_ARGS(DEBUG_TAG), ## args)
#else
#define traceDebug(message, args...)
#endif

#if LOG_LEVEL >= INFO
#define traceInfo(message, args...)      PRINT(LOG_FMT message NEXTLINE, LOG_ARGS(INFO_TAG), ## args)
#else
#define traceInfo(message, args...)
#endif

#if LOG_LEVEL >= ERROR
#define traceError(message, args...)     PRINT(LOG_FMT message NEXTLINE, LOG_ARGS(ERROR_TAG), ## args); \
                                         PRINTCONSOLE(LOG_FMT message NEXTLINE, LOG_ARGS(ERROR_TAG), ## args)
#else
#define traceError(message, args...)
#endif



#endif //LOGGER_H
