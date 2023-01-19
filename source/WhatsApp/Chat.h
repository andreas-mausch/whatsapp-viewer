#pragma once

#include <string>
#include <vector>

class WhatsappDatabase;
class WhatsappMessage;

class WhatsappChat
{
private:
	WhatsappDatabase &database;

	int chatId;
	std::string displayName;
	std::string key;
	std::string subject;
	long long creation;
	long long lastMessage;

	int messagesSent;
	int messagesReceived;

	bool messagesLoaded;
	std::vector<WhatsappMessage *> messages;

public:
	WhatsappChat(WhatsappDatabase &database, int chatId, const std::string &displayName, const std::string &key, const std::string &subject, long long creation, long long lastMessage, int messagesSent, int messagesReceived);
	~WhatsappChat();

	const std::string& getDisplayName() const;
	const std::string& getKey() const;
	const std::string& getSubject() const;
	long long getCreation() const;
	long long getLastMessage() const;
	int getMessagesSent() const;
	int getMessagesReceived() const;

	std::vector<WhatsappMessage *> getMessages(const volatile bool &running);
};
