
CC=g++

all: tcpClientTest httpRequestTest

tcpClientTest: TcpClient.h TcpClient.cpp
	$(CC) -o tcpClientTest TcpClient.cpp -DTEST_TCP_CLIENT

httpRequestTest: TcpClient.h TcpClient.cpp HttpRequest.h HttpRequest.cpp
	$(CC) -o httpRequestTest TcpClient.cpp HttpRequest.cpp -std=c++11 -DTEST_HTTP_REQUEST

clean:
	rm tcpClientTest httpRequestTest