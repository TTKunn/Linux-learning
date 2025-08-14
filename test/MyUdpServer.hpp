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
#include <functional>

using namespace LogModule;
using func_t = std::function<std::string(const std::string&)>;

const int defaultfd = -1;

class MyUdpServer{
public:
    MyUdpServer(uint16_t port, func_t func)
     :_sockfd(defaultfd),
    //   _ip(ip),
      _port(port),
      _func(func)
    {}

    void Init(){
        //1.创建套接字
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(_sockfd < 0){
            LOG(LogLevel::FATAL) << "socket error" ;
            exit(1);
        }
        LOG(LogLevel::INFO) << "socket create success, sockfd: " << _sockfd ;
        
        //绑定socket信息，ip和端口
        //填充sockaddr_in结构体
        struct sockaddr_in local;
        bzero(&local,sizeof(local)); //一个用于内存初始化的函数，主要功能是将指定内存区域的所有字节设置为 0。
        local.sin_family = AF_INET;

        //将端口从本地格式转换成网络序列
        local.sin_port = htons(_port);
        //将IP从string类型转化为4个字节存储，再转化为网络序列：
        //转成in_addr_t类型 inet_addr (const char *cp)
        // local.sin_addr.s_addr = inet_addr(_ip.c_str());
        // 不用上面的绑定具体ip，使用INADDR_ANY，表示绑定所有网卡
        local.sin_addr.s_addr = INADDR_ANY;
        
    
        //绑定套接字
        int n = bind(_sockfd, (struct sockaddr*)&local, sizeof(local));
        if( n < 0 ){
            LOG(LogLevel::FATAL) << "bind error";
            exit(2);
        }
        //绑定成功，输出日志
        LOG(LogLevel::INFO) << "bind success, sockfd:" << _sockfd;

        
    }
    void Start(){
        //因为udp不用管链接，一直管收发就好了
        _isrunning = true;
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        while(_isrunning){
            char buffer[1024];
            //1.不断收(读)消息
            ssize_t rec_msg = recvfrom(_sockfd, buffer, sizeof(buffer)-1, 0, (struct sockaddr* )&peer, &len);
            //sizeof(buffer)-1 是为了留一个位置将来添加 \n
            if(rec_msg > 0){
                //说明收到了消息
                int peer_port = ntohs(peer.sin_port);
                std::string peer_ip = inet_ntoa(peer.sin_addr);

                buffer[rec_msg] = 0;

                std::string result = _func(buffer);
                LOG(LogLevel::DEBUG) << "buffer:" << buffer << " from " << peer_ip << ":" << peer_port;

                // 2.发消息
                // std::string snd_msg = "receive message from server：";
                // snd_msg += buffer;
                // ssize_t snd_sz = sendto(_sockfd, snd_msg.c_str(), snd_msg.size(), 0, (struct sockaddr *)&peer, len);
                ssize_t snd_sz = sendto(_sockfd, result.c_str(), result.size(), 0, (struct sockaddr *)&peer, len);

            }
        }
    }

private:
    int _sockfd;
    uint16_t _port;  // 就是unsigned short int类型重定义了
    // std::string _ip; // 这里使用字符串风格存储点分十进制的ip（比如1234.1.2.3）后需要转换成网络字节序
    bool _isrunning;
    func_t _func;   //  服务器回调函数，用来对数据进行处理
};