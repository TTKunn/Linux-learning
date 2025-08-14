#include "MyUdpServer.hpp"
#include <memory>
#include <iostream>


std::string func(const std::string &message)
{
    std::string hello = "hello, ";
    hello += message;
    return hello + "\n";
}

// ./myudpserver ip port
int main(int argc, char* argv[]){
    if(argc != 2){
        std::cerr << "Usage:" << argv[0] << " + port " << std::endl;
        return 1;
    }
    // std::string ip = argv[1];
    uint16_t port = std::stoi(argv[1]);
    Enable_Console_Log_Strategy();
    std::unique_ptr<MyUdpServer> usvr = std::make_unique<MyUdpServer>(port, func);
    usvr->Init();
    usvr->Start();
    return 0;
}
