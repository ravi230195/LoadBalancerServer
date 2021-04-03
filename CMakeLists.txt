cmake_minimum_required(VERSION 3.17)
project(LoadBalancer)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)
add_executable( LoadBalancer main.cpp
        Socket/Socket.h Socket/Socket.cpp
        ServerSocket/ServerSocket.cpp ServerSocket/ServerSocket.h
        LoadBalancerServer/Server.cpp LoadBalancerServer/Server.h
        Logging/logger.h
        ThreadPool/ThreadPool.cpp ThreadPool/ThreadPool.h
        ConnectionMapper/ConnectionMapper.cpp ConnectionMapper/ConnectionMapper.h
        NetworkWorker/NetworkWorker.cpp NetworkWorker/NetworkWorker.h
        ClientSocket/ClientSocket.cpp ClientSocket/ClientSocket.h
        ServerSelectionAlgorithm/ServerSelection.cpp ServerSelectionAlgorithm/ServerSelection.h
        ServerSelectionAlgorithm/RoundRobin/RoundRobin.cpp ServerSelectionAlgorithm/RoundRobin/RoundRobin.h
        ServerSelectionAlgorithm/ConsistentHashing/ConsistentHashing.cpp ServerSelectionAlgorithm/ConsistentHashing/ConsistentHashing.h
        ServerSelectionAlgorithm/AppServerInfo/AppServerInfo.cpp ServerSelectionAlgorithm/AppServerInfo/AppServerInfo.h
        ServerSelectionAlgorithm/IServerSelectionAlgorithm.h)
