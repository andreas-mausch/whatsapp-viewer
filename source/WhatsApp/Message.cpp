#include "Message.h"
#include <algorithm>

WhatsappMessage::WhatsappMessage(std::string messageId, std::string chatId, bool fromMe, int status, std::string data, long long timestamp, long long receivedTimestamp, long long sendTimestamp, std::string mediaUrl, std::string mediaMimeType, int mediaWhatsappType, int mediaSize, std::string mediaName, std::string mediaCaption, int mediaDuration, double latitude, double longitude, const void *thumbImage, int thumbImageSize, const std::string &remoteResource, const void *rawData, int rawDataSize, const void *thumbnailData, int thumbnailDataSize, WhatsappMessage *quotedMessage, bool link) :
	messageId(messageId), chatId(chatId), fromMe(fromMe), status(status), data(data), timestamp(timestamp),
	receivedTimestamp(receivedTimestamp), sendTimestamp(sendTimestamp), mediaUrl(mediaUrl),
	mediaMimeType(mediaMimeType), mediaWhatsappType(mediaWhatsappType), mediaSize(mediaSize),
	mediaName(mediaName), mediaCaption(mediaCaption), mediaDuration(mediaDuration),
	latitude(latitude), longitude(longitude),
	thumbImage(NULL), thumbImageSize(thumbImageSize),
	remoteResource(remoteResource),
	rawData(NULL), rawDataSize(rawDataSize),
	thumbnailData(NULL), thumbnailDataSize(thumbnailDataSize),
	quotedMessage(quotedMessage), link(link)
{
	if (thumbImage != NULL && thumbImageSize > 0)
	{
		this->thumbImage = new unsigned char[thumbImageSize];
		memcpy(this->thumbImage, thumbImage, thumbImageSize);

		buildFilename();
	}
	if (rawData != NULL && rawDataSize > 0)
	{
		this->rawData = new unsigned char[rawDataSize];
		memcpy(this->rawData, rawData, rawDataSize);
	}
	if (thumbnailData != NULL && thumbnailDataSize > 0)
	{
		this->thumbnailData = new unsigned char[thumbnailDataSize];
		memcpy(this->thumbnailData, thumbnailData, thumbnailDataSize);
	}
}

WhatsappMessage::~WhatsappMessage()
{
	delete[] this->thumbImage;
	delete[] this->rawData;
	delete[] this->thumbnailData;
}

const std::string& WhatsappMessage::getMessageId() const
{
	return messageId;
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

int WhatsappMessage::getMediaWhatsappType() const
{
	return mediaWhatsappType;
}

double WhatsappMessage::getLatitude() const
{
	return latitude;
}

double WhatsappMessage::getLongitude() const
{
	return longitude;
}

const std::string &WhatsappMessage::getRemoteResource() const
{
	return remoteResource;
}

const std::string &WhatsappMessage::getMediaName() const
{
	return mediaName;
}

const std::string &WhatsappMessage::getMediaCaption() const
{
	return mediaCaption;
}

int WhatsappMessage::getMediaDuration() const
{
	return mediaDuration;
}

bool WhatsappMessage::isLink() const
{
	return link;
}

bool WhatsappMessage::hasThumbnail()
{
	return getThumbnail() != NULL;
}

unsigned char *WhatsappMessage::getThumbnail()
{
	if (rawDataSize > 0 && rawData != NULL) {
		return rawData;
	}
	return thumbnailData;
}

int WhatsappMessage::getThumbnailSize()
{
	if (rawDataSize > 0 && rawData != NULL) {
		return rawDataSize;
	}
	return thumbnailDataSize;
}

bool WhatsappMessage::hasLinkThumbnail()
{
	return getLinkThumbnail() != NULL;
}

unsigned char *WhatsappMessage::getLinkThumbnail()
{
	if (!link || thumbImage == NULL)
	{
		return NULL;
	}

	unsigned char *image = thumbImage + 27;

	if (image[0] == 0xFF && image[1] == 0xD8)
	{
		return image;
	}

	return NULL;
}

int WhatsappMessage::getLinkThumbnailSize()
{
	unsigned char *thumbnail = getLinkThumbnail();

	if (thumbnail == NULL)
	{
		return 0;
	}

	return (thumbnail - thumbImage) + thumbImageSize;
}

WhatsappMessage *WhatsappMessage::getQuotedMessage()
{
	return quotedMessage;
}

const std::string &WhatsappMessage::getFilename() const
{
	return filename;
}

void WhatsappMessage::buildFilename()
{
	unsigned char pathStartPattern[] = { 0x4C, 0x00, 0x04, 'p', 'a', 't', 'h' };
	unsigned char *pathStart = std::search(thumbImage, thumbImage + thumbImageSize, pathStartPattern, pathStartPattern + sizeof(pathStartPattern));

	if (pathStart >= thumbImage + thumbImageSize)
	{
		return;
	}

	unsigned char endPattern[] = { 0x78, 0x70, 0x74 };
	unsigned char *end = std::search(pathStart, thumbImage + thumbImageSize, endPattern, endPattern + sizeof(endPattern));

	if (end >= thumbImage + thumbImageSize)
	{
		return;
	}

	unsigned short length = ((*(end + sizeof(endPattern))) << 8) | *(end + sizeof(endPattern) + 1);

	if (length == 0)
	{
		return;
	}

	filename = std::string(reinterpret_cast<char *>(end + sizeof(endPattern) + sizeof(unsigned short)), length);
}
