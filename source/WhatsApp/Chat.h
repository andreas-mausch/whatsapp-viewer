#pragma once

#include <string>
#include <vector>

class WhatsappDatabase;
class WhatsappMessage;

class WhatsappChat
{
private:
	WhatsappDatabase &database;

	std::string displayName;
	std::string key;
	std::string subject;
	long long creation;
	long long lastMessage;

	bool messagesLoaded;
	std::vector<WhatsappMessage *> messages;

public:
	WhatsappChat(WhatsappDatabase &database, const std::string &displayName, const std::string &key, const std::string &subject, long long creation, long long lastMessage);
	~WhatsappChat();

	const std::string& getDisplayName() const;
	const std::string& getKey() const;
	const std::string& getSubject() const;
	long long getCreation() const;
	long long getLastMessage() const;

	std::vector<WhatsappMessage *> getMessages(const volatile bool &running);
};
