.PHONY: all
all: myudpserver myudpclient

myudpserver:Main.cc
	g++ -o $@ $^ -std=c++11
myudpclient:UdpClient.cc
	g++ -o $@ $^ -std=c++11

.PHONY: clean
clean:
	rm -f myudpserver my	udpclient