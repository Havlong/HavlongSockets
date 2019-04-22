#include "HavlongSClient.h"

using namespace HavlongSC;

class CheckerClientImpl : public HavlongSClient {
private:
	void handleData(const std::string& data) {
		puts(data.c_str());
		fflush(stdout);
	}

public:
	CheckerClientImpl(const std::string& host, const std::string& port)
	: HavlongSClient(host, port) {}

	void execute(std::string key, std::string user) {
		std::string data = "{ \"key\": \"";
		data += key;
		data += "\", \"user\": \"";
		data += user;
		data += "\" }";
		HavlongSClient::execute(data);
	}
};