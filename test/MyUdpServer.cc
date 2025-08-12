#include "MyUdpServer.hpp"
#include <memory>
#include <iostream>

int main(){

    Enable_Console_Log_Strategy();
    std::unique_ptr<MyUdpServer> usvr = std::make_unique<MyUdpServer>();
    usvr->Init();
    usvr->Start();
    return 0;
}
