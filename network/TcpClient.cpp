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

	/* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		/* Tell the user that we could not find a usable */
		/* Winsock DLL.                                  */
		cerr << "WSAStartup failed with error: "<< err << endl;
		return;
	}

	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		cerr << "Could not find a usable version of Winsock.dll" << endl;
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
/**
Connect to a host on a certain port number
*/
bool TcpClient::connect()
{
	//create socket if it is not already created
	if (sock == INVALID_SOCKET)
	{
		//Create socket
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock == INVALID_SOCKET)
		{
			return false;
		}
	}
	

	//setup address structure
	if (inet_addr(address.c_str()) == -1)
	{
		struct hostent *he;
		struct in_addr **addr_list;

		//resolve the hostname, its not an ip address
		if ((he = gethostbyname(address.c_str())) == NULL)
		{
			//gethostbyname failed
			cerr << "gethostbyname: Failed to resolve hostname" << endl;

			return false;
		}

		//Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
		addr_list = (struct in_addr **) he->h_addr_list;

		for (int i = 0; addr_list[i] != NULL; i++)
		{
			//strcpy(ip , inet_ntoa(*addr_list[i]) );
			server.sin_addr = *addr_list[i];

			cout << address << " resolved to " << inet_ntoa(*addr_list[i]) << endl;

			break;
		}
	}

	//plain ip address
	else
	{
		server.sin_addr.s_addr = inet_addr(address.c_str());
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	//Connect to remote server
	if (::connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		cerr << "connect failed. Error" << endl;
		return false;
	}

	cout << "Connected\n";
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

		cout << "----------------------------" << endl;
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
		cout << "----------------------------" << endl;
	}
	return 0;
}
#endif
