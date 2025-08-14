#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// ./myudpclient server_ip server_port
int main(int argc, char* argv[]){
    if(argc != 3){
        std::cerr << "Usage:" << argv[0] << " + server_ip + server_port + " << std::endl;
        return 1;
    }
    
    //1.创建套接字
    int sockfd = socket (AF_INET,SOCK_DGRAM,0);
    if(sockfd < 0){
        std::cerr << "socket error" << std::endl;
        return 2;
    }
    //2.绑定。
    //客户端需要进行绑定，但是不需要我们手动显式地进行bind绑定
    //当首次发送消息的时候，os会自动给客户端进行绑定。
    //端口号采用随机端口号
    while(1){
        std::string input;
        std::cout << "Please Enter# ";
        std::getline(std::cin,input);
        sockaddr_in dest_addr;
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(atoi(argv[2]));
        dest_addr.sin_addr.s_addr = inet_addr(argv[1]);
        socklen_t addrlen = sizeof(dest_addr);

        int send_size = sendto(sockfd, input.c_str(),sizeof(input),0,(struct sockaddr*)&dest_addr,addrlen);
    
        char recv_buf[1024];
        sockaddr_in peer_addr;
        socklen_t peer_addr_len = sizeof(peer_addr);
        int recv_size = recvfrom(sockfd,recv_buf,sizeof(recv_buf)-1,0,(struct sockaddr*)&peer_addr,&peer_addr_len);
        if(recv_size > 0){
            recv_buf[recv_size] = '\0';
            std::cout << "server# " << recv_buf << std::endl;
        }
    }


    return 0;
}