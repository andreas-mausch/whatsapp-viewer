#include "Message.h"

WhatsappMessage::WhatsappMessage(std::string chatId, bool fromMe, int status, std::string data, long long timestamp, long long receivedTimestamp, long long sendTimestamp, std::string mediaUrl, std::string mediaMimeType, int mediaSize) :
	chatId(chatId), fromMe(fromMe), status(status), data(data), timestamp(timestamp), receivedTimestamp(receivedTimestamp), sendTimestamp(sendTimestamp), mediaUrl(mediaUrl), mediaMimeType(mediaMimeType), mediaSize(mediaSize)
{
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
