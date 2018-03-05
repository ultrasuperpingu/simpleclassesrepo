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

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#else
#include <netdb.h>
#include <unistd.h>
#endif
#include "TcpClient.h"
#include <iostream>

using std::string;
using std::cin;
using std::cerr;
using std::cout;
using std::endl;

TcpClient::TcpClient(std::string address, int port) : address(address), port(port), sock(INVALID_SOCKET)
#ifdef _WIN32
, inited(false)
#endif
{
#ifdef _WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		cerr << "WSAStartup failed: " << err << endl;
		return;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		cerr << "Could not find Winsock.dll version 2.2" << endl;
		WSACleanup();
		return;
	}
	inited = true;
#endif
}
TcpClient::~TcpClient()
{
	close();
#ifdef _WIN32
	if(inited)
		WSACleanup();
#endif
}

bool TcpClient::connect()
{
	if (sock == INVALID_SOCKET)
	{
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock == INVALID_SOCKET)
		{
			return false;
		}
	}
	
	if (inet_addr(address.c_str()) == -1)
	{
		struct hostent *he;
		struct in_addr **addr_list;

		if ((he = gethostbyname(address.c_str())) == NULL)
		{
			cerr << "gethostbyname: Failed to resolve hostname" << endl;
			return false;
		}

		addr_list = (struct in_addr **) he->h_addr_list;

		for (int i = 0; addr_list[i] != NULL; i++)
		{
			server.sin_addr = *addr_list[i];
			break;
		}
	}
	else
	{
		server.sin_addr.s_addr = inet_addr(address.c_str());
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	if (::connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		cerr << "connect failed. Error" << endl;
		return false;
	}
	return true;
}

bool TcpClient::send(const uint8_t* data, uint32_t size)
{
	if (::send(sock, (const char*)data, size, 0) < 0)
	{
		return false;
	}
	return true;
}
int TcpClient::receive(uint8_t* data, uint32_t maxsize)
{
	return recv(sock, (char*)data, sizeof(uint8_t)*maxsize, 0);
}

void TcpClient::close()
{
	if (isOpen()) {
#ifdef _WIN32
		::closesocket(sock);
#else
		::close(sock);
#endif
	}
}

#ifdef TEST_TCP_CLIENT
int main(int argc, char *argv[])
{
	
	string host;
	while (true)
	{
		cout << "Enter hostname (empty line to quit): ";
		std::getline(cin, host);
		if (host.length() == 0)
			break;
		TcpClient c(host, 80);

		if (!c.connect())
		{
			cerr << "Failed to connect" << endl;
			return -1;
		}
		string data = "GET / HTTP/1.1\r\nHost:" + host + "\r\n\r\n";
		c.send((const uint8_t*)data.c_str(), data.length());

		std::string resp;
		uint8_t received[2048];
		int size = c.receive(received, 2048);
		if (size < 0)
		{
			cerr << "Reception failed" << endl;
			return -1;
		}
		if (size == 2048)
			size--;
		received[size] = 0;
		resp = (char*)received;
		cout << resp << endl;
	}
	return 0;
}
#endif
