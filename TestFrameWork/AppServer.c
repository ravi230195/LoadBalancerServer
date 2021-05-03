#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>
#include <bits/stdc++.h>
#define MAX 80
#define PORT 8083
#define SA struct sockaddr
using namespace std;
static int totalRequestServer;


vector<string> split(string s, char delimiter)
{
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while(getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

void func(int sockfd, string data) {
    char buff[50];
    strcpy(buff, data.c_str());
    int n;
    printf("requestServed %d\n", totalRequestServer);
    write(sockfd, buff, sizeof(buff));
}

void clientCall(string data)
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    } else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }

    // function for chat
    func(sockfd, data);

    // close the socket
    close(sockfd);
}



int main(int argc , char *argv[]) {
    int socket_desc, new_socket, c;
    struct sockaddr_in server, client;
    char *message;

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
    }
    string ssport = argv[1];
    int sport = stoi(ssport);
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(sport);

    //Bind
    if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
        puts("bind failed");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc, 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while ((new_socket = accept(socket_desc, (struct sockaddr *) &client, (socklen_t *) &c))) {
        totalRequestServer++;
        //message = NULL;
        char readM[100];
        bzero(readM, sizeof(readM));
        read(new_socket, readM, sizeof(readM));
        printf("Connection accepted... data to process [%s] \n", readM);

        message = NULL;
        //Reply to the client
        message = "PROCESSING\n";
        write(new_socket, message, strlen(message));

        sleep(1);

        string data = readM;
        vector <string> res = split(data, ':');
	
	    string ans = res[1] + ":Hello From AppServer " + res[0];
        clientCall(ans);
    }
}


