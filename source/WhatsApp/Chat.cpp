#include "Chat.h"
#include "Database.h"
#include "Message.h"
#include "../VectorUtils.h"

WhatsappChat::WhatsappChat(WhatsappDatabase &database, const std::string &displayName, const std::string &key, const std::string &subject, long long creation, long long lastMessage)
	: database(database), displayName(displayName), key(key), subject(subject), creation(creation), lastMessage(lastMessage), messagesLoaded(false)
{
}

WhatsappChat::~WhatsappChat()
{
	clearVector(messages);
}

const std::string& WhatsappChat::getDisplayName() const
{
	return displayName;
}

const std::string& WhatsappChat::getKey() const
{
	return key;
}

const std::string& WhatsappChat::getSubject() const
{
	return subject;
}

long long WhatsappChat::getCreation() const
{
	return creation;
}

long long WhatsappChat::getLastMessage() const
{
	return lastMessage;
}

std::vector<WhatsappMessage *> WhatsappChat::getMessages(const volatile bool &running)
{
	if (!messagesLoaded)
	{
		database.getMessages(key, messages, running);

		if (running)
		{
			messagesLoaded = true;
		}
	}

	return messages;
}
