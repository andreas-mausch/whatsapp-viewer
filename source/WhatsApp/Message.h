#pragma once

#include <string>

enum MediaWhatsappType
{
	MEDIA_WHATSAPP_TEXT,
	MEDIA_WHATSAPP_IMAGE,
	MEDIA_WHATSAPP_AUDIO,
	MEDIA_WHATSAPP_VIDEO,
	MEDIA_WHATSAPP_CONTACT,
	MEDIA_WHATSAPP_LOCATION,
	MEDIA_WHATSAPP_UNKNOWN1,
	MEDIA_WHATSAPP_UNKNOWN2,
	MEDIA_WHATSAPP_CALL,
	MEDIA_WHATSAPP_FILE,
	MEDIA_WHATSAPP_UNKNOWN3,
	MEDIA_WHATSAPP_UNKNOWN4,
	MEDIA_WHATSAPP_UNKNOWN5,
	MEDIA_WHATSAPP_GIF,
	MEDIA_WHATSAPP_UNKNOWN6,
	MEDIA_WHATSAPP_UNKNOWN7,
	MEDIA_WHATSAPP_LIVE_LOCATION
};

class WhatsappMessage
{
private:
	std::string messageId;
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
	std::string mediaName;
	std::string mediaCaption;
	int mediaDuration;
	double latitude;
	double longitude;
	unsigned char *thumbImage;
	int thumbImageSize;
	std::string remoteResource;
	unsigned char *rawData;
	int rawDataSize;
	unsigned char *thumbnailData;
	int thumbnailDataSize;
	WhatsappMessage *quotedMessage;
	bool link;

	std::string filename;
	void buildFilename();

public:
	WhatsappMessage(std::string messageId, std::string chatId, bool fromMe, int status, std::string data, long long timestamp,
					long long receivedTimestamp, long long sendTimestamp,
					std::string mediaUrl, std::string mediaMimeType,
					int mediaWhatsappType, int mediaSize,
					std::string mediaName, std::string mediaCaption, int mediaDuration,
					double latitude, double longitude,
					const void *thumbImage,int thumbImageSize,
					const std::string &remoteResource, const void *rawData, int rawDataSize,
					const void *thumbnailData, int thumbnailDataSize,
					WhatsappMessage *quotedMessage, bool link);
	~WhatsappMessage();

	const std::string &getMessageId() const;
	const std::string &getData() const;
	bool isFromMe() const;
	long long getTimestamp() const;
	double getLatitude() const;
	double getLongitude() const;
	const std::string &getRemoteResource() const;
	int getMediaWhatsappType() const;
	const std::string &getMediaName() const;
	const std::string &getMediaCaption() const;
	int getMediaDuration() const;
	bool isLink() const;

	bool hasThumbnail();
	unsigned char *getThumbnail();
	int getThumbnailSize();

	bool hasLinkThumbnail();
	unsigned char *getLinkThumbnail();
	int getLinkThumbnailSize();

	const std::string &getFilename() const;

	WhatsappMessage *getQuotedMessage();
};
