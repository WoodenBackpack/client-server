#include <iostream>
#include <vector>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
    std::string command = argv[1];
    std::vector<std::string> strings(argv + 2 , argv + argc);
    std::vector<int> ints;
    for (auto& it : strings) {
        ints.push_back(stoi(it));
    }
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int val = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,  &val, sizeof(val));  
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    int client_fd = connect(sockfd, (sockaddr*)&address, sizeof(address));
    if (client_fd < 0) {
        std::cout<<"connect error\n";
    }
    int msg[128];
    msg[0] = command == "sort" ? 1 : 0;
    msg[1] = ints.size();
    std::copy(std::begin(ints), std::end(ints), msg + 2);
    int nbytes_sent = send(sockfd, msg, sizeof(msg), 0);
    if (nbytes_sent > 0) {
        std::cout<<"sent " << nbytes_sent << " bytes of data \n";
    }

    int receiveBuffer[128];
    int read = recv(sockfd, receiveBuffer, sizeof(receiveBuffer), 0);
    if (read < 0) {
        std::cout<<"failed \n";
    }
    std::cout<<"received ints: \n";
    std::vector<int> receivedInts(receiveBuffer, receiveBuffer + ints.size());
    for (auto it: receivedInts) {
        std::cout<<it <<" ";
    }

    return 0;
}