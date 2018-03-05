#pragma once
#include <string>

#ifdef _WIN32
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <inttypes.h>
#include <arpa/inet.h>
typedef int SOCKET;
#define INVALID_SOCKET -1
#endif

class TcpClient
{
public:
	TcpClient(std::string address, int port);
	virtual ~TcpClient();
	bool connect();
	
	bool send(const uint8_t* data, uint32_t size);
	int receive(uint8_t* data, uint32_t size);
	void close();
	bool isOpen() const;
protected:
	SOCKET sock;
	std::string address;
	int port;
	struct sockaddr_in server;
#ifdef _WIN32
	bool inited;
#endif
};

inline bool TcpClient::isOpen() const { return sock != INVALID_SOCKET; }

