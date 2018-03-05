#pragma once
#include <string>
#include <map>
#define RBUFF_SIZE 4096
enum Method
{
	HTTP_GET,
	HTTP_HEAD,
	HTTP_POST,
	HTTP_PUT,
	HTTP_DELETE,
	HTTP_CONNECT,
	HTTP_OPTIONS,
	HTTP_TRACE,
	HTTP_PATCH
};

class HttpResponse;
class HttpRequest
{
public:
	HttpRequest(const std::string& address);
	virtual ~HttpRequest();

	HttpResponse send();

	const std::string& getHeader(const std::string& key) const;
	void setHeader(const std::string& key, const std::string& value);

protected:
	std::string serverAddress;
	int port; 
	std::string localAddress;
	Method method;
	std::map<std::string, std::string> headers;
};

class HttpResponse {
public:
	const std::string& getBody() const;
	int getStatusCode() const;
	const std::string& getStatusText() const;
	const std::string& getHeader(const std::string& key) const;
private:
	HttpResponse();
	HttpResponse(const std::string& response);
	friend HttpResponse HttpRequest::send();
	int statusCode;
	std::string statusText;
	std::string body;
	std::map<std::string, std::string> headers;
};

inline const std::string& HttpRequest::getHeader(const std::string& key) const { return headers.at(key); }
inline void HttpRequest::setHeader(const std::string& key, const std::string& value) { headers[key] = value; }

inline HttpResponse::HttpResponse() : statusCode(-1), statusText("Cannot send the request") {}
inline const std::string& HttpResponse::getBody() const { return body; }
inline int HttpResponse::getStatusCode() const { return statusCode; }
inline const std::string& HttpResponse::getStatusText() const { return statusText; }
inline const std::string& HttpResponse::getHeader(const std::string& key) const { return headers.at(key); }
