#include "CheckerServerImpl.cpp"
#include <iostream>

class MyCheckerServer : public CheckerServerImpl {
private:
	std::map<std::string, std::string> fillDB() {
		std::map<std::string, std::string> toReturn;
		toReturn.insert({ "123kkk54l3", "Vasya" });
		return toReturn;
	}
	std::string ok() {
		return "Valid license";
	}
	std::string bad() {
		return "Invalid request";
	}
	std::string deny() {
		return "Invalid license";
	}
public:
	MyCheckerServer(std::string port)
		: CheckerServerImpl(port) {}
};

int main() {
	std::string port;
	std::cout << "Enter port of server" << std::endl;
	std::getline(std::cin, port);

	MyCheckerServer server(port);
	server.start();

	return 0;
}