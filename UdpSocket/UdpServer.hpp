#pragma once

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include "Log.hpp"
#include <cstring>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <functional>

using func_t = std::function<std::string(const std::string&)>;
//func_t是一个函数类型，参数为const std::string&，返回值为std::string。

extern Log mylog;

enum{
    SOCKET_ERO = 1,
    BIND_ERO = 2
};

const int size = 1024;

// uint16_t default_port = 8080;

std::string default_ip = "0.0.0.0";
 
class UdpServer
{
public:
    UdpServer(const uint16_t &port , const std::string &ip = default_ip)
    :_sockfd(0),_port(port),_ip(ip),_is_running(false)
    {

    }

    void Init(){
        
        //1.创建udp套接字
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
        if(_sockfd < 0){
            mylog(Fatal, "创建套接字失败,sockfd: %d",SOCKET_ERO);
            exit(SOCKET_ERO);   //套接字创建失败，退出程序
        }
        mylog(Info, "创建套接字成功,sockfd: %d",_sockfd);


        //2.设置套接字地址结构，绑定本地地址bind
        struct sockaddr_in local;
        bzero(&local,sizeof(local));

        local.sin_family = AF_INET;
        //sin_family是struct sockaddr的成员变量，用来指定协议族，这里是IPv4。
        //AF_INET是IPv4的地址族，INADDR_ANY是特殊的IP地址，表示接收所有IP地址。

        local.sin_port = htons(_port);
        //sin_port是struct sockaddr_in的成员变量，用来指定端口号。
        //htons函数将主机字节序的端口号转换为网络字节序。
        //必须保证端口号是网络字节序列，因为端口号是要在网络上传输，发送给对方的。
        //如果机器本来就是大端字节序，则不需要转换，如果是小端字节序，则需要转换。

        //local.sin_addr.s_addr = _ip;
        //但是_ip是字符串类型，需要转换为网络字节序的IP地址,string->uint32_t,uint32_t要是网络序列的
        local.sin_addr.s_addr = inet_addr(_ip.c_str());
        //local.sin_addr.s_addr = INADDR_ANY;
        //这里可以指定IP地址，如果不指定，则绑定任意地址，云服务器一般绑定任意地址。
        //inet_addr函数将字符串形式的IP地址转换为网络字节序的IP地址。
        //要转换成c_str()形式的字符串，因为inet_addr函数只接受const char*类型。

        //目前做的工作是将一些数据设置到套接字地址结构中，但是还没有绑定到本地地址，现在本质知识定义了一个参数并给了一些数据
        //接下来要调用bind函数将套接字绑定到本地地址，这样才能接收到数据。

        int n = bind(_sockfd,(struct sockaddr*)&local,sizeof(local));
        //因为local类型是struct sockaddr_in，而bind函数的传参是struct sockaddr*，所以需要强制类型转换。
        //sizeof(local)是地址结构的大小，这里是sizeof(struct sockaddr_in)。
        //bind函数返回0表示绑定成功，返回-1表示绑定失败。

        if(n < 0){
            mylog(Fatal, "绑定套接字失败,sockfd: %d, error string: %s",BIND_ERO,strerror(errno));
            exit(BIND_ERO);   //绑定失败，退出程序
        }
        mylog(Info, "绑定套接字成功,sockfd: %d, port: %d, ip: %s",_sockfd,_port,_ip.c_str());

    }

    void Run(func_t func){
        char inbuffer[size];
        _is_running = true;
        while(_is_running){
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            ssize_t n =recvfrom (_sockfd,inbuffer,sizeof(inbuffer) - 1 , 0 , (struct sockaddr*)&client,&len);
            if(n < 0){
                mylog(Warning, "接收数据失败,sockfd: %d, error string: %s",_sockfd,strerror(errno));
                continue;
            }
            inbuffer[n] = 0;
            //inbuffer[n] = '\0'; //字符串结尾加上'\0'，以便输出时不用再加上endl
            
            std::string info = inbuffer;
            std::string echo_string = func(info);

            // //简单充当一次处理，也可以另外写一个函数用于处理，这里只做简单打印
            // std::string info = inbuffer;
            // std::string echo_string = "server echo:  " + info + "\n";
            mylog(Info, "接收到数据,sockfd: %d, data: %s",_sockfd,info.c_str());
            
            sendto(_sockfd,echo_string.c_str(),echo_string.size(),0,(struct sockaddr*)&client,len);
        }
    }

    ~UdpServer(){
        if(_sockfd > 0){
            close(_sockfd);
        }
    }

private:
    int _sockfd;        //网络文件描述符
    uint16_t _port;     //端口号
    std::string _ip;    //ip地址，任意地址绑定
    bool _is_running;   //服务器运行状态
};
