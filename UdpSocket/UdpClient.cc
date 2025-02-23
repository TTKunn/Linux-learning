#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <strings.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <cstdlib>

void Usage(std::string proc){
    std::cout <<"\n\rUsage: "<<proc<<" port[1024+]\n\r"<<std::endl;
}

// ./udpclient serverip serverport
int main(int argc, char* argv[]) {
    if(argc != 3)
    {
        Usage(argv[0]);
        exit(0);
    } 
    std::string serverip = argv[1];
    uint16_t serverport = atoi(argv[2]);


    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cout << "create socket failed" << std::endl;
        return 1;
    } 
    //客户端也要绑定ip，只不过不需要用户显式的绑定，一般由操作系统自动分配
    //因为客户端端口号有限制，假如固定绑定端口，可能会出现多个进程都要绑定同一个端口的情况，导致无法通信
    //并且client的port是多少并不重要，只要能保证主机上的唯一性即可

    char buffer[1024];
    std::string message;
    while(true){
        std::cout << "input message: ";
        getline(std::cin,message);

        struct sockaddr_in server;
        bzero(&server, sizeof(server));

        server.sin_family = AF_INET;
        server.sin_port = htons(serverport);
        server.sin_addr.s_addr = inet_addr(serverip.c_str());

        std::cout << "send message \" " << message << "\"  to "  << serverip << ":" << serverport << std::endl;

        sendto(sockfd, message.c_str(), message.size(), 0, (struct sockaddr*)&server, (socklen_t)sizeof(server));
        
        struct sockaddr_in temp;
        socklen_t len = sizeof(temp);

        ssize_t s = recvfrom(sockfd,buffer,1023,0,(struct sockaddr*)&temp, &len);
        if(s > 0){
            buffer[s] = 0;
            std::cout<<buffer<<std::endl;
        }
    }
 
    close(sockfd);
    return 0;
}