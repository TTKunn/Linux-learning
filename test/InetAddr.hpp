#pragma once

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class InetAddr
// 提供主机序列和网络序列互相转换的方法
{
    public:
        InetAddr(struct sockaddr_in &addr)
        :_addr(addr)
        {
            _ip = inet_ntoa(addr.sin_addr);
            // 将网络字节序的 32 位 IP 地址（struct in_addr类型，存储在sockaddr_in
            // 的sin_addr成员中）转换为人类可读的点分十进制字符串（如"192.168.1.1"）。
            _port = ntohs(addr.sin_port);   
            // 将 16 位的网络字节序（大端）数据转换为主机字节序
        }
        uint16_t GetPort() const{
            return _port;
        }
        std::string GetIp() const{
            return _ip;
        }

    private:
        struct sockaddr_in _addr;
        std::string _ip;
        uint16_t _port;
};