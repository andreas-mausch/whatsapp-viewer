#pragma once

#include <string>

class WhatsappMessage
{
private:
	std::string chatId;
	bool fromMe;
	int status;
	std::string data;
	long long timestamp;
	long long receivedTimestamp;
	long long sendTimestamp;
	std::string mediaUrl;
	std::string mediaMimeType;
	int mediaSize;

public:
	WhatsappMessage(std::string chatId, bool fromMe, int status, std::string data, long long timestamp, long long receivedTimestamp, long long sendTimestamp, std::string mediaUrl, std::string mediaMimeType, int mediaSize);

	const std::string &getData() const;
	bool isFromMe() const;
};
