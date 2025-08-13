#include "MyUdpServer.hpp"
#include <memory>
#include <iostream>

// ./myudpserver ip port
int main(int argc, char* argv[]){
    if(argc != 3){
        std::cerr << "Usage:" << argv[0] << " + ip + port + " << std::endl;
    }
    std::string ip = argv[1];
    uint16_t port = std::stoi(argv[2]);
    Enable_Console_Log_Strategy();
    std::unique_ptr<MyUdpServer> usvr = std::make_unique<MyUdpServer>(ip,port);
    usvr->Init();
    usvr->Start();
    return 0;
}
