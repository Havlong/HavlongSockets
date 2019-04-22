#include "CheckerClientImpl.cpp"
#include <iostream>

int main() {
	std::string host, port;
	std::cout << "Enter host name of server" << std::endl;
	std::getline(std::cin, host);
	std::cout << "Enter port of server" << std::endl;
	std::getline(std::cin, port);

	CheckerClientImpl checkerClient(host, port);

	std::string key, user;
	std::cout << "Enter your username" << std::endl;
	std::getline(std::cin, user);
	std::cout << "Enter license key" << std::endl;
	std::getline(std::cin, key);

	checkerClient.execute(key, user);

	return 0;
}