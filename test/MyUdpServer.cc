#include "MyUdpServer.hpp"
#include "Dict.hpp"
#include <memory>
#include <iostream>


// ./myudpserver ip port
int main(int argc, char* argv[]){
    if(argc != 2){
        std::cerr << "Usage:" << argv[0] << " + port " << std::endl;
        return 1;
    }
    // std::string ip = argv[1];
    uint16_t port = std::stoi(argv[1]);
    Enable_Console_Log_Strategy();

    //字典对象提供翻译功能
    Dict dict;
    dict.Load();

    std::unique_ptr<MyUdpServer> usvr = std::make_unique<MyUdpServer>(port, [&dict](const std::string &message){
        return dict.Translate(message);
    });
    usvr->Init();
    usvr->Start();
    return 0;
}
