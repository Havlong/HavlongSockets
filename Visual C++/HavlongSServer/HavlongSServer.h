#pragma once
#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdio>
#include <cstdlib>
#include <string>

namespace HavlongSS {
	class HavlongSServer
	{
	private:
		std::string port;
		void initWSA(WSADATA& wsaData);
		SOCKET initServerSocket();
		void startListening(SOCKET& listener);
		SOCKET acceptNewClient(SOCKET& listener);
		void readData(SOCKET& client, std::string& data);
		void writeData(SOCKET& client, std::string& data);
		void close(SOCKET& client);
		virtual void handle(std::string& data) = 0;
	public:
		HavlongSServer(const std::string& port);
		~HavlongSServer() = default;
		void start();
	};
}
