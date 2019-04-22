#include "HavlongSServer.h"
#include <map>
#include <regex>

using namespace HavlongSS;

class CheckerServerImpl : public HavlongSServer {
private:
	std::map<std::string, std::string> keyDB;
	void handle(std::string& data) {
		printf("Login attempt with: %s\n", data.c_str());
		fflush(stdout);
		std::smatch matches;
		if (std::regex_match(data, matches,
			std::regex(R"(\s*\{\s*"key"\s*:\s*"(\w+)\"\s*,\s*"user"\s*:\s*"(\w+)\"\s*\}\s*)"))) {
			std::string key = matches[1];
			std::string user = matches[2];
			if (keyDB.count(key) && keyDB[key] == user) {
				data = ok();
			}
			else {
				data = deny();
			}
		}
		else if (std::regex_match(data, matches,
			std::regex(R"(\s*\{\s*"user"\s*:\s*"(\w+)\"\s*,\s*"key"\s*:\s*"(\w+)\"\s*\}\s*)"))) {
			std::string key = matches[2];
			std::string user = matches[1];
			if (keyDB.count(key) && keyDB[key] == user) {
				data = ok();
			}
			else {
				data = deny();
			}
		}
		else {
			data = bad();
		}
		fflush(stdout);
	}
	virtual std::map<std::string, std::string> fillDB() = 0;
	virtual std::string ok() = 0;
	virtual std::string deny() = 0;
	virtual std::string bad() = 0;
public:
	CheckerServerImpl(const std::string port) 
		: HavlongSServer(port){}
	void start() {
		this->keyDB = fillDB();
		HavlongSServer::start();
	}
};