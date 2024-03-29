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
#define PORT 8081
#define SA struct sockaddr
using namespace std;

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
    char buff[100];
    strcpy(buff, data.c_str());
    int n;
    printf("data %s\n", buff);
    write(sockfd, buff, sizeof(buff));
    bzero(buff, sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    printf("From Server : %s \n", buff);
    
    string recv =  buff;
    vector<string> res = split(recv, ' ');
    if(res.back() == data)
      printf("TestCase PASS\n");
    else
      printf("TestCase FAIL\n");
}

int main(int argc, char* argv[])
{
    string data = argv[1];
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
    } else
    printf("connected to the server..\n");

    // function for chat
    func(sockfd, data);

    // close the socket
    close(sockfd);


}

