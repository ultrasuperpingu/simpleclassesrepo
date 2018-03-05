#include "HttpRequest.h"
#include "TcpClient.h"
#include <regex>
#include <iostream>
#include <sstream>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;


const char* methods[] = {
	"GET",
	"HEAD",
	"POST",
	"PUT",
	"DELETE",
	"CONNECT",
	"OPTIONS",
	"TRACE",
	"PATCH"
};

HttpRequest::HttpRequest(const string& address) : method(HTTP_GET)
{
	std::regex r("http://([a-zA-Z0-9\\.]+)(:[0-9]+)?(/.*)?");
	std::cmatch m;
	if (std::regex_match(address.c_str(), m, r))
	{
		serverAddress = m[1];
		localAddress = m[3];
		if (localAddress.length() == 0)
			localAddress = "/";
		string p = m[2];
		if (p.length() == 0)
			port = 80;
		else
			std::stringstream(p) >> port;
		headers["Host"] = serverAddress;
	}
	else
	{
		cerr << "invalid address" << endl;
	}
}


HttpRequest::~HttpRequest()
{
}

HttpResponse HttpRequest::send()
{
	TcpClient tcp(serverAddress, port);
	string data = string(methods[method]) + " " + localAddress+ " HTTP/1.1\r\n";
	for (auto i = headers.begin(); i != headers.end(); i++)
	{
		data += i->first + ":" + i->second + "\r\n";
	}
	data += "\r\n";
	if (!tcp.connect())
	{
		return HttpResponse();
	}
	tcp.send((const uint8_t*)data.c_str(), data.length());
	std::vector<uint8_t> buff(RBUFF_SIZE,0);
	int sizeReceived = 0;
	int totalSize = 0;
	while(true)
	{
		sizeReceived = tcp.receive(&buff[totalSize], RBUFF_SIZE);
		if (sizeReceived < 0)
			break;
		totalSize += sizeReceived;
		if (sizeReceived == RBUFF_SIZE)
			buff.resize(totalSize + RBUFF_SIZE);
		else
			break;
	}
	buff.resize(totalSize + 1);
	buff[totalSize] = 0;
	string response = string((char*)&buff[0]);
	return HttpResponse(response);
}

HttpResponse::HttpResponse(const string& response)
{
	body = response;
}

//#ifdef TEST_HTTP_REQUEST
#include <iostream>
int main()
{
	string address;
	while (true)
	{
		cout << "Enter an http address (empty line to quit): ";
		std::getline(cin, address);
		if (address.length() == 0)
			break;
		HttpRequest req(address);
		HttpResponse resp = req.send();
		cerr << resp.getBody() << endl;
	}
}
//#endif