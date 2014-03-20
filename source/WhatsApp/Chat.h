#pragma once

#include <string>
#include <vector>

class WhatsappDatabase;
class WhatsappMessage;

class WhatsappChat
{
private:
	WhatsappDatabase &database;

	std::string key;
	std::string subject;
	long long creation;

	bool messagesLoaded;
	std::vector<WhatsappMessage *> messages;

public:
	WhatsappChat(WhatsappDatabase &database, const std::string &key, const std::string &subject, long long creation);
	~WhatsappChat();

	const std::string& getKey() const;
	const std::string& getSubject() const;
	long long getCreation() const;

	std::vector<WhatsappMessage *> getMessages();
};
