#include "UdpServer.hpp"
#include <memory>

// ./udpserver port

void Usage(std::string proc){
    std::cout <<"\n\rUsage: "<<proc<<" port[1024+]\n\r"<<std::endl;
}
//一个端口只能绑定一个进程，一个进程可以绑定多个端口。 


std::string Handle(const std::string& str){
    std::string reserve = "Server get a message: ";
    reserve += str;
    return reserve;
}


int main(int argc, char* argv[])
//使用这种方式可以动态调整端口
{
    if(argc != 2)
    //因为可执行程序名和 port 都在 argv[0] 和 argv[1] 中，所以 argc 必须为 2
    {
        Usage(argv[0]);
        exit(0);
    } 

    uint16_t port = std::stoi(argv[1]);
    //argv[1] 就是用户输入的端口号，需要转换为数字类型

    std::unique_ptr<UdpServer> server (new UdpServer(port));
    //云服务器禁止直接绑定公网IP，如果是虚拟机，上面的代码就能跑起来
    //如果一个机器绑定了多个ip，例如有.164 和 .165，如果这里只绑定了.164的，那么发往.165的请求就不会被监听到
    //所以，一般再云服务器上，一般绑定的ip地址是0，那么发给这台主机的数据，不管是.164还是.165，都能收到
    //ip绑定为0，就是任意地址绑定

    //对于绑定的端口，系统内定的端口号，一般都要有固定的应用层协议使用，一般不建议使用
    //建议一般绑定1024以上的端口，因为0-1023端口是系统保留端口，一般应用层协议都不会使用这些端口
    //如果要使用这些端口，需要root权限，或者使用sudo命令

    server->Init();
    server->Run(Handle);

    return 0;
}
 
