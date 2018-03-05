#pragma once
/*
 *  TcpClient
 *  Copyright (c) 2018 Julien Soler
 *                All Rights Reserved
 *
 *  This program is free software. It comes without any warranty, to
 *  the extent permitted by applicable law. You can redistribute it
 *  and/or modify it under the terms of the Do What the Fuck You Want
 *  to Public License, Version 2, as published by Sam Hocevar. See
 *  http://www.wtfpl.net/ for more details.
 */

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

