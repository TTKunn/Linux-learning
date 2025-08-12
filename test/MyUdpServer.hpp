#pragma once

#include <iostream>
#include <memory>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <strings.h>
#include <arpa/inet.h>
#include "Log.hpp"

using namespace LogModule;

const int defaultfd = -1;

class MyUdpServer{
public:
    MyUdpServer(const std::string &ip,uint16_t port)
     :_sockfd(defaultfd),
      _ip(ip),
      _port(port)
    {

    }

    void Init(){
        //1.创建套接字
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(_sockfd < 0){
            LOG(LogLevel::FATAL) << "socket error" ;
            exit(1);
        }
        LOG(LogLevel::INFO) << "socket create success, sockfd: " << _sockfd ;
        
        //2.帮i的那个socket信息，ip和端口
        //2.1 填充sockaddr_in结构体
        struct sockaddr_in local;
        bzero(&local,sizeof(local)); //一个用于内存初始化的函数，主要功能是将指定内存区域的所有字节设置为 0。
        
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = 
    
    }
    void Start();

private:
    int _sockfd;
    uint16_t _port;  // 就是unsigned short int类型重定义了
    std::string _ip; // 这里使用字符串风格存储点分十进制的ip（比如1234.1.2.3）后需要转换成网络字节序
};