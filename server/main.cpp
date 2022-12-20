#include <iostream>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int val = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));  
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    std::cout<<"bind\n";
    if (bind(sockfd,(struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cout<<"bind error\n";
        exit(EXIT_FAILURE);
    }
    std::cout<<"listen\n";
    if (listen(sockfd, 3) < 0) {
        std::cout<<"listen error\n";
        exit(EXIT_FAILURE);
    }
    std::cout<<"accept\n";
    int addrlen = sizeof(address);
    int connfd = accept(sockfd, (struct sockaddr*)&address, (socklen_t*)&addrlen); 
    if (connfd < 0) {
        std::cout<<"accept error\n";
    }
    int receiveBuffer[128];
    std::cout<<"waiting for data: \n";
    int read = recv(connfd, receiveBuffer, sizeof(receiveBuffer), 0);
    int operation = receiveBuffer[0];
    std::cout<<"operation: " << operation <<"\n";
    int numOfInts = receiveBuffer[1];
    
    std::vector<int> ints(receiveBuffer + 2, receiveBuffer + 2 + numOfInts);
    std::cout<<"ints\n";
    for (auto it : ints) {
        std::cout<<it<<" ";
    }
    std::cout<<"\n";
    
    std::sort(std::begin(ints), std::end(ints));
    if (operation == 0) {
        std::reverse(std::begin(ints), std::end(ints));
    }
    std::cout<<"ints\n";
    for (auto it : ints) {
        std::cout<<it<<" ";
    }

    int msg[128];
    std::copy(std::begin(ints), std::end(ints), msg);
    int nbytes_sent = send(connfd, msg, sizeof(msg), 0);
    close(connfd);
    shutdown(sockfd, SHUT_RDWR);
    return 0;
}