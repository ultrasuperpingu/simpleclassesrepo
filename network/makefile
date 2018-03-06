
CC=g++

all: tcpClientTest httpRequestTest

tcpClientTest: TcpClient.h TcpClient.cpp makefile
	$(CC) -o tcpClientTest TcpClient.cpp -Wall -Wextra -pedantic -DTEST_TCP_CLIENT

httpRequestTest: TcpClient.h TcpClient.cpp HttpRequest.h HttpRequest.cpp makefile
	$(CC) -o httpRequestTest TcpClient.cpp HttpRequest.cpp -std=c++11 -Wall -Wextra -pedantic -DTEST_HTTP_REQUEST

clean:
	rm tcpClientTest httpRequestTest