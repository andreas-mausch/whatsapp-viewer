#pragma once

#include <string>

enum MediaWhatsappType
{
	MEDIA_WHATSAPP_TEXT,
	MEDIA_WHATSAPP_IMAGE,
	MEDIA_WHATSAPP_AUDIO
};

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
	int mediaWhatsappType;
	int mediaSize;
	unsigned char *thumbImage;
	int thumbImageSize;
	unsigned char *rawData;
	int rawDataSize;

public:
	WhatsappMessage(std::string chatId, bool fromMe, int status, std::string data, long long timestamp, long long receivedTimestamp, long long sendTimestamp, std::string mediaUrl, std::string mediaMimeType, int mediaWhatsappType, int mediaSize, const void *thumbImage, int thumbImageSize, const void *rawData, int rawDataSize);
	~WhatsappMessage();

	const std::string &getData() const;
	bool isFromMe() const;
	long long getTimestamp() const;
	unsigned char *getRawData();
	int getRawDataSize();
	int getMediaWhatsappType() const;
};
