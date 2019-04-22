#include "HavlongSServer.h"

const int bufferSize = 512;

void HavlongSS::HavlongSServer::initWSA(WSADATA& wsaData)
{
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		perror("WSA Startup failed: ");
		exit(1);
	}
}

SOCKET HavlongSS::HavlongSServer::initServerSocket()
{
	struct addrinfo* result = nullptr, info;
	ZeroMemory(&info, sizeof(info));
	info.ai_family = AF_INET;
	info.ai_socktype = SOCK_STREAM;
	info.ai_protocol = IPPROTO_TCP;
	info.ai_flags = AI_PASSIVE;

	int iResult = getaddrinfo(nullptr, port.c_str(), &info, &result);
	if (iResult != 0) {
		perror("Getting of standart addrinfo failed: ");
		WSACleanup();
		exit(2);
	}

	SOCKET toReturn = INVALID_SOCKET;
	toReturn = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (toReturn == INVALID_SOCKET) {
		perror("Server socket init failed: ");
		WSACleanup();
		exit(3);
	}

	iResult = bind(toReturn, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		perror("Binding failed: ");
		freeaddrinfo(result);
		closesocket(toReturn);
		WSACleanup();
		exit(4);
	}

	freeaddrinfo(result);
	puts("Server started...");
	fflush(stdout);
	return toReturn;
}

void HavlongSS::HavlongSServer::startListening(SOCKET& listener)
{
	if (listen(listener, SOMAXCONN) == SOCKET_ERROR) {
		perror("Listening failed: ");
		closesocket(listener);
		WSACleanup();
		exit(5);
	}
}

SOCKET HavlongSS::HavlongSServer::acceptNewClient(SOCKET& listener)
{
	SOCKET toReturn = INVALID_SOCKET;

	toReturn = accept(listener, nullptr, nullptr);
	if (toReturn == INVALID_SOCKET) {
		perror("Accepting failed: ");
		closesocket(listener);
		WSACleanup();
		exit(6);
	}
	puts("Connection opened...");
	fflush(stdout);
	return toReturn;
}

void HavlongSS::HavlongSServer::readData(SOCKET& client, std::string& data)
{
	int iResult;
	char buf[bufferSize + 1];
	do {
		ZeroMemory(buf, sizeof(buf));
		iResult = recv(client, buf, bufferSize, 0);
		if (iResult > 0) {
			data += buf;
		}
		else {
			perror("Receiving failed: ");
			closesocket(client);
			WSACleanup();
			exit(7);
		}
	} while (iResult == bufferSize);
}

void HavlongSS::HavlongSServer::writeData(SOCKET& client, std::string& data)
{
	int iResult = send(client, data.c_str(), data.size(), 0);
	if (iResult == SOCKET_ERROR) {
		perror("Sending failed: ");
		closesocket(client);
		WSACleanup();
		exit(8);
	}
}

void HavlongSS::HavlongSServer::close(SOCKET& client)
{
	puts("Connection closing...");
	int iResult = shutdown(client, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		perror("Shutdown failed: ");
		closesocket(client);
		WSACleanup();
		exit(8);
	}
	closesocket(client);
	fflush(stdout);
}

HavlongSS::HavlongSServer::HavlongSServer(const std::string& port) 
	: port(port){}

void HavlongSS::HavlongSServer::start()
{
	WSADATA wsaData;
	this->initWSA(wsaData);

	SOCKET listener = this->initServerSocket();

	this->startListening(listener);

	while (true) {
		SOCKET client = this->acceptNewClient(listener);
		std::string clientInfo;
		this->readData(client, clientInfo);
		this->handle(clientInfo);
		this->writeData(client, clientInfo);
		this->close(client);
	}
	closesocket(listener);
	WSACleanup();
}
