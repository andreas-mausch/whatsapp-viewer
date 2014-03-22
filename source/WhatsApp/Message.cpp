#include "Message.h"

WhatsappMessage::WhatsappMessage(std::string chatId, bool fromMe, int status, std::string data, long long timestamp, long long receivedTimestamp, long long sendTimestamp, std::string mediaUrl, std::string mediaMimeType, int mediaWhatsappType, int mediaSize, const void *thumbImage, int thumbImageSize, const void *rawData, int rawDataSize) :
	chatId(chatId), fromMe(fromMe), status(status), data(data), timestamp(timestamp),
	receivedTimestamp(receivedTimestamp), sendTimestamp(sendTimestamp), mediaUrl(mediaUrl),
	mediaMimeType(mediaMimeType), mediaWhatsappType(mediaWhatsappType), mediaSize(mediaSize),
	thumbImage(NULL), thumbImageSize(thumbImageSize), rawData(NULL), rawDataSize(rawDataSize)
{
	if (thumbImage != NULL && thumbImageSize > 0)
	{
		this->thumbImage = new unsigned char[thumbImageSize];
		memcpy(this->thumbImage, thumbImage, thumbImageSize);
	}
	if (rawData != NULL && rawDataSize > 0)
	{
		this->rawData = new unsigned char[rawDataSize];
		memcpy(this->rawData, rawData, rawDataSize);
	}
}

WhatsappMessage::~WhatsappMessage()
{
	delete[] this->thumbImage;
	delete[] this->rawData;
}

const std::string& WhatsappMessage::getData() const
{
	return data;
}

bool WhatsappMessage::isFromMe() const
{
	return fromMe;
}

long long WhatsappMessage::getTimestamp() const
{
	return timestamp;
}

unsigned char *WhatsappMessage::getRawData()
{
	return rawData;
}

int WhatsappMessage::getRawDataSize()
{
	return rawDataSize;
}

int WhatsappMessage::getMediaWhatsappType() const
{
	return mediaWhatsappType;
}
