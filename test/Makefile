.PHONY: all clean

# 定义生成的可执行文件目标
all: myudpserver myudpclient

# 编译服务端
myudpserver: MyUdpServer.cc
	g++ -o $@ $^ -std=c++17

# 编译客户端
myudpclient: MyUdpClient.cc
	g++ -o $@ $^ -std=c++17

# 清理生成的可执行文件
clean:
	rm -f myudpserver myudpclient