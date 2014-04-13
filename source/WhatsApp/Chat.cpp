#include "Chat.h"
#include "Database.h"
#include "Message.h"
#include "../VectorUtils.h"

WhatsappChat::WhatsappChat(WhatsappDatabase &database, const std::string &key, const std::string &subject, long long creation)
	: database(database), key(key), subject(subject), creation(creation), messagesLoaded(false)
{
}

WhatsappChat::~WhatsappChat()
{
	clearVector(messages);
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

std::vector<WhatsappMessage *> WhatsappChat::getMessages()
{
	if (!messagesLoaded)
	{
		database.getMessages(key, messages);
		messagesLoaded = true;
	}

	return messages;
}
