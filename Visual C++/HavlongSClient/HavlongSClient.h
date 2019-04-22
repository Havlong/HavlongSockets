#pragma once
#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdio>
#include <cstdlib>
#include <string>

namespace HavlongSC {
	class HavlongSClient
	{
	private:
		std::string host;
		std::string port;
		void initWSA(WSADATA& wsaData);
		SOCKET initClientSocket();
		void writeData(SOCKET& client, const std::string& data);
		void readData(SOCKET& client, std::string& data);
		void close(SOCKET& client);
		virtual void handleData(const std::string& data) = 0;
	public:
		HavlongSClient(const std::string& host, const std::string& port);
		~HavlongSClient() = default;
		void execute(std::string data);
	};
}
