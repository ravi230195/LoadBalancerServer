# Load Balancer

Load balancer efficiently distributes the network or application traffic across multiple servers in a server farm.

The idea is to implement a request-response paradigm. where client request traffic is routed to **public port** and the server farm responses are routed to **private port**.

# Instalation
### Build LoadBalancer
- git clone https://github.com/ravi230195/LoadBalancerServer.git

- cd LoadBalancer
- [cleans the executable]
  - cmake --build cmake-build-debug --target clean -- -j 6
  
- [build executable]
    - cmake --build cmake-build-debug --target all -- -j 6
    
- [run the executable]
  - ./cmake-build-debug/LoadBalancer
    
### Run AppSerevrs for testing
Open a new terminal go to repository and run below command
- [run AppServer]
  - ./TestFrameWork/runAppServer.sh

### Run Clients for testing
Open a new terminal go to repository and run below command
- [run AppServer]
    - ./TestFrameWork/runClients.sh
    
## Architecture

![alt text](<LB_Architecture.jpg>)
The sequence of numbers represents the call flow of the requests.
1. Client opens a TCP socket and sends a request to be processed to the **public port** of the load balancer.
2. On receiving the request **worker thread** is woken from the thread pool. The threads are maintained by ***threadPool*** Module and tasks are assigned by ***networkWorker*** Module. The following tasks are performed by worker thread.
    * Based on **client_ip + port** a unique transaction_id is generated.
    * The transcation_id is mapped to the open client socket for further processing. This is done by ***ConnectionMapper*** Module.
    * Depending on the configured server selection algorithm, an appropriate App server is picked. Which is Implemented in ***SeverSelectionAlgorithm*** module.
    * The worker thread opens a TCP connection towards the Appserver to send the request to be processed. The format used to communicate is as below.   
**```<transaction_id:request>```**
    * The Appserver acknowledges the message by sending **"PROCESSING"** and the TCP connection between AppServer and the Load Banalncer (private port) is closed.
    * The worker thread task is completed and is added back to the thread pool. 
    * On Successful processing of the request an AppServer opens a new TCP connection towards the Load balancer (private port) to send the response. The format used is   
  **```<transaction_id:response>```**
    * A new worker thread is picked from the thread pool to process the response from the AppServer, the transaction_id is used to identify the client socket towards the public port. This mapping is handled by the ***connectionMapper*** Module.
    * The Worker thread then sends the response to the client and then connection is closed. On completion of this task, the worker thread is again added back to the thread pool. 
              

## Modules
### Configuration
The configuation are statically read from the ConfigurationManager.xml file.
example configuraion
```
<Configuration>
    <LoadServerPublicPort>8081</LoadServerPublicPort>
    <LoadServerPrivatePort>8083</LoadServerPrivatePort>
    <NumberOfWorkerHreads>50</NumberOfWorkerHreads>
    <serverSelectionAlgorithm>RoundRobin</serverSelectionAlgorithm>
    <AppServer Name="Appserver_1">
        <AppServerConfiguration>
            <AppServerName>server_1</AppServerName>
            <AppServerIp>127.0.0.1</AppServerIp>
            <AppServerPort>9090</AppServerPort>
        </AppServerConfiguration>
    </AppServer>
    <AppServer Name="Appserver_2">
        <AppServerConfiguration>
            <AppServerName>server_2</AppServerName>
            <AppServerIp>127.0.0.1</AppServerIp>
            <AppServerPort>9091</AppServerPort>
        </AppServerConfiguration>
    </AppServer>
</Configuration>
```

### Socket
Contains API's that can be used to create sockets for both server and client ends. It also provides implementation to send/receive data on a socket. It can be viewed as a file descriptor after the successful creation. 
```
    // APIs used for establishing a server socket.
    // connection Server
    bool create();
    bool bind(const int port);
    bool listen();
    bool accept(Socket& newSocket);

    // API's used for establishing a client socket.
    // Client
    bool connect ( const std::string host, const int port );

    // data transfer used by both client and server
    bool receive(std::string& data);
    bool send(const std::string& data);
```
* #### ServerSocket
ServerSocket is derived from Socket Class and it provides APIs to run the server and accept the client connections.
```
class ServerSocket: public Socket

    // server listening Port
    int mServerPort 
    // to create, bind, listen for server.
    bool connect();

    /* 
     On successful acceptance of connection, a serverSocket is populated, 
     and used to send and receive data for that client.
    */
    bool accept(Socket& serverSocket);
```
* #### ClientSocket
ClientSocket is derived from Socket Class and it provides APIs for client to connect to the server and send/receive data.
```
class ClientSocket : public Socket

ClientSocket::ClientSocket ( std::string host, int port ) {
    if (!Socket::create()) {
        traceError("Client create failed \n");
    }

    if (!Socket::connect(host, port)) {
        traceError("Client connect failed \n");
    }
}
ClientSocket* clientSocket =  new ClientSocket(IP, port)
clientSocket->send(sendData);
```

### ConnectionMapper
ConnectionMapper is mainly responsible for providing transaction_id to socket mapping for the connecting clients. ConnectionMapper is a singleton class implementation. Many worker threads try to add and delete the transaction_id based on the task.  
The transaction_id is added by the worker thread when a new client request is being processed and transition_id is removed after the response from appServer is forwarded to the client.  
Since many worker threads try to add and remove the transaction_id mutual exclusion is implemented for the map.
```
 Class ConnectionMapper 

    // holds trancastion_id to client socket mapping
    std::unordered_map<std::string, Socket* > mMapper 
    // to Provide mutual exclusion while addition and deletion of transaction_id
    std::mutex mConnectionMapMutex;
    // used for singleton Implementation
    static std::mutex mCreationLock;
    static ConnectionMapper* getInstance()
    bool addConnection(Socket* connection)
    void removeConnection(const std::string uniqueId)
    Socket* getConnection(const std::string uniqueId)
```
### Logger
There are three different logging levels.
1. ERROR
2. INFO
3. DEBUG   

- function calls are:  
     - traceDebug()  
     - traceInfo()  
     - traceError()  
```
Example function call:
traceDebug("Data being sent [%s]", sendData.c_str());
traceError("Connection Already Found.... please debug");
```

- **Log format**  
  ***<log_level> | <thread_id> | <line_number>:<function_name> | <file_name> | <log output>***
```


Example log:
DEBUG | -527206592           |   29:IThreadPool               | ThreadPool.h         |Initializing ThreadPool.....
DEBUG | -527206592           |   34:IThreadPool               | ThreadPool.h         |ThreadPool up and running.....
DEBUG | -973445376           |   26:create                    | Socket.cpp           |Server connected on socket = [4] 
DEBUG | -973445376           |   37:bind                      | Socket.cpp           |Server Binding on socket = [4] 
DEBUG | -965052672           |   26:create                    | Socket.cpp           |Server connected on socket = [3] 
DEBUG | -965052672           |   37:bind                      | Socket.cpp           |Server Binding on socket = [3] 
DEBUG | -973445376           |   25:run                       | Server.cpp           |Initializing Server ......
DEBUG | -965052672           |   25:run                       | Server.cpp           |Initializing Server ......
DEBUG | -965052672           |   26:run                       | Server.cpp           |Server Up and Running....
DEBUG | -973445376           |   26:run                       | Server.cpp           |Server Up and Running....

```

### ServerSelectionAlgorithm
#### AppServerInfo
AppServerInfo represents a single appServer. It holds all the information of the appSerever. It contains the following information of the server.
- IP Address.
- Port.
- server Name.
- Number of requests served by that server.
- server hash [hash(IP + port)]
```
class AppServerInfo

    std::string mIp;
    int mPort;
    size_t mHash;
    std::string mServerName;
    int mRequestServed;

    AppServerInfo(std::string ip, int port, std::string name):mIp(ip), mPort(port), mServerName(name){
        std::string keyStr = mIp +  " " +  std::to_string(mPort);
        mHash = std::hash<std::string>{}(keyStr);
    }

    std::string getAppServerIP();
    std::string getAppServerName();
    int getAppServerPort();
    size_t getAppServerHash();
```
#### ServerSelection
Provides an interface for the load balancer to choose a server selection algorithm and based on the algorithm selected returns an appServer to which the request must be forwarded when an API is called.  
ServerSelection is a singleton class implementation.
```
Class ServerSelection
    // used for singleton Implementation
    static std::mutex mCreationLock;
    // API returns the appServer Info selected using the algorithm
    AppServerInfo* getAppServerInfo(std::pair<Socket *, std::string> request);
    // Holds the serverSelectionAlgorithm configured to use
    static IServerSelectionAlgorithm* mServerSelectionAlgorithm;
    // populates ServerSelection Algorithm example (consistent hashing, round robin)
    static void getServerSelection();
```


#### IServerSelectionAlgorithm
ServerSelectionAlgorithm is mainly responsible for the selection of appServer from server farms based on the algorithm selected. 

```
Class IServerSelectionAlgorithm
    // The API returns the appServer Info to which the request must be forwarded to based on algorithm selected.
    virtual AppServerInfo* getAppServerInfo(std::pair<Socket *, std::string> request) = 0;
```


- ##### RoundRobin
RoundRobin is derived from IServerSelectionAlgorithm. 
Round robin load balancing is a simple way to distribute client requests across a group of servers. A client request is forwarded to each server in turn.  
Since many worker threads call getAppServerInfo(), mutual exclusion on it is implemented.
```
class RoundRobin: public IServerSelectionAlgorithm 
    // Holds the appServer Data
    std::vector<AppServerInfo*> mAppServerData;
    // used for mutual exclusion.
    std::mutex mRoundRobinLock;
    // API returns the appSever Info to which the request must be forwaded.
    virtual AppServerInfo* getAppServerInfo(std::pair<Socket *, std::string> request);
    // used to populate the mAppServerData based on the info of the Appservers provided
    void readAppServerConfig();
};
```
- ##### ConsistentHashing
ConsistentHashing is derived from IServerSelectionAlgorith. Consistent Hashing is a distributed hashing scheme that operates independently of the number of servers or objects in a distributed hash table by assigning them a position on an abstract circle, or hash ring.
Since many worker threads call getAppServerInfo(), mutual exclusion on it is implemented.
```
class ConsistentHashing: IServerSelectionAlgorithm 
    // used for mutual exclusion.
    std::mutex mConsistentHashLock;
    // Holds the appServer Data
    std::vector<AppServerInfo*> mAppServerData;
    // API returns the appSever Info to which the request must be forwaded.
    virtual AppServerInfo* getAppServerInfo(std::pair<Socket *, std::string> request);
    // used to populate the mAppServerData based on the info of the Appservers provided
    void readAppServerConfig();
    // used to populate circular ring like structure to represent consistent hashing.
    std::map<size_t , AppServerInfo*> mRing;
    void populateConsistentHashingRing();
```

### ThreadPool
ThreadPool module is mainly responsible to provide worker threads to the load balancer to perform a task. A worker thread is generally is woken from the pool when a client sends a request to load balancer or when an appServer sends a repose to the request.  
Based on the number specified and machine concurrency a number of threads are spawned and set in a sleeping state. When a task is added to the queue a worked thread is picked from to pool to process the request. 
- Start a thread pool with the same number of threads as cores in the machine.
- Every time a request is received, we will submit the request to the thread pool
- Whenever the thread from the thread pool finishes it adds back to the threadpool.
```
Class IThreadPool
    // number of threads in spawned
    int mThreadNumber;
    // list of threads in the pool  
    std::vector<std::thread> mThread;
    // to provide mutual exclusion to the task queue
    std::mutex mWorkingQueueMutex;
    bool mShutDown;
    // used to wake up the thread when a task is added
    std::condition_variable_any mWorkQueueConditionVariable;
    std::queue<std::pair<Socket*, std::pair<std::string, bool>>> mWorkerQueue;

    IThreadPool(int threadNumber): mThreadNumber(threadNumber), mShutDown(false)
    void createThreads();
    void doWork();
    // task to be performed by the thread
    virtual void process(std::pair<Socket*, std::pair<std::string, bool>> request) = 0;
    // the task added to the queue by the load balancer which is then picked up by the worker thread in the thread pool
    void addToQueue(Socket* connection, std::string data, bool response = true);
```
- #### NetworkWorker
NetworkWorker is derived from the IThreadPool. The main responsibility of networkWorker is to implement the process(), which handles the task to be implemented by the thread.  
- process()  
 API is used to implement the task to be performed. Based on the port where the request arrived either processResponse() or processRequest() is called.  
If the request arrived on public port processRequest() is called and If the request arrived on private port processResponse() is called.


There are mainly two tasks provided by the networkWorker.
- processRequest()  
  The main function of this API is to handle the requests coming from the client,   
   -  it generates transaction_id, 
   - adds mtraction_id to socket mapping using ***connectionMapper*** Module, 
   - then opens a TCP connection towards the appserver selected by serverselection algorithm implemented by ***serverSeletionAlgorithm*** module.
   - And Once ACK is received from the appServer, the task is completed and thread is added back to threadPool.

- processResponse()   
The main function of this API is to handle the requests coming from the appServer.   
Once the request is processed by the appServer, appServer opens a new TCP connection towards the load balancer private port and sends the response along with transaction_id to which its responding.   
   -  it splits the request from appServer ***<transaction_id : response>***
   - using the ***connectionMapper*** module gets the client socket responsible for the respose. 
   - then it send the response to the client socket which is open.
   - Once the task is completed and thread is added back to threadPool.
```
class NetworkWorker: public IThreadPool
    // API is used to implement the task to be performed. 
    // Based on the port where the request arrived either processResponse or processRequest() is called.
    virtual void process(std::pair<Socket*, std::pair<std::string, bool>>  request);
    void addConnectionMap(Socket* socket);
    void processResponse(std::pair<Socket *, std::string> response);
    void processRequest(std::pair<Socket *, std::string> response);
```
## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.
