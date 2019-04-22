#include "HavlongSClient.h"

const int bufferSize = 512;

void HavlongSC::HavlongSClient::initWSA(WSADATA& wsaData)
{
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		perror("WSA Startup failed: ");
		exit(1);
	}
}

SOCKET HavlongSC::HavlongSClient::initClientSocket()
{
	struct addrinfo* result = nullptr, info;
	ZeroMemory(&info, sizeof(info));
	info.ai_family = AF_UNSPEC;
	info.ai_socktype = SOCK_STREAM;
	info.ai_protocol = IPPROTO_TCP;

	int iResult = getaddrinfo(host.c_str(), port.c_str(), &info, &result);
	if (iResult != 0) {
		perror("Getting of addrinfo failed: ");
		WSACleanup();
		exit(2);
	}

	SOCKET toReturn = INVALID_SOCKET;

	for (auto ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
		toReturn = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (toReturn == INVALID_SOCKET) {
			perror("Socket init failed: ");
			WSACleanup();
			exit(3);
		}

		iResult = connect(toReturn, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(toReturn);
			toReturn = INVALID_SOCKET;
			continue;
		}
		break;
	}

	if (toReturn == INVALID_SOCKET) {
		perror("Socket connect failed: ");
		WSACleanup();
		exit(4);
	}

	freeaddrinfo(result);
	puts("Connection opened...");
	fflush(stdout);
	return toReturn;
}

void HavlongSC::HavlongSClient::writeData(SOCKET& client, const std::string& data)
{
	int iResult = send(client, data.c_str(), data.size(), 0);
	if (iResult == SOCKET_ERROR) {
		perror("Sending failed: ");
		closesocket(client);
		WSACleanup();
		exit(5);
	}
}

void HavlongSC::HavlongSClient::readData(SOCKET& client, std::string& data)
{
	int iResult;
	char buf[bufferSize + 1];
	data.clear();
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
			exit(6);
		}
	} while (iResult == bufferSize);
}

void HavlongSC::HavlongSClient::close(SOCKET& client)
{
	puts("Connection closed...");
	closesocket(client);
	WSACleanup();
	fflush(stdout);
}

HavlongSC::HavlongSClient::HavlongSClient(const std::string& host, const std::string& port)
	: host(host), port(port) {}

void HavlongSC::HavlongSClient::execute(std::string data)
{
	WSADATA wsaData;
	this->initWSA(wsaData);

	SOCKET client = this->initClientSocket();

	this->writeData(client, data);
	this->readData(client, data);
	this->handleData(data);
	this->close(client);
}

