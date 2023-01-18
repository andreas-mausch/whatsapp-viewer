#include "Chat.h"
#include "Database.h"
#include "Message.h"
#include "../VectorUtils.h"

WhatsappChat::WhatsappChat(WhatsappDatabase &database, int chatId ,const std::string &displayName, const std::string &key, const std::string &subject, long long creation, long long lastMessage, int messagesSent, int messagesReceived)
	: database(database), displayName(displayName), chatId(chatId), key(key), subject(subject), creation(creation), lastMessage(lastMessage), messagesSent(messagesSent), messagesReceived(messagesReceived), messagesLoaded(false)
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

int WhatsappChat::getMessagesSent() const
{
	return messagesSent;
}

int WhatsappChat::getMessagesReceived() const
{
	return messagesReceived;
}

std::vector<WhatsappMessage *> WhatsappChat::getMessages(const volatile bool &running)
{
	if (!messagesLoaded)
	{
		database.getMessages(chatId, messages, running);

		if (running)
		{
			messagesLoaded = true;
		}
	}

	return messages;
}
