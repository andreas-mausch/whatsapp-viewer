#pragma once

#include <string>
#include <vector>
#include "../Platforms/Win32/ThreadWindows.h"

struct sqlite3;
class WhatsappDatabase;
class WhatsappMessage;

class QueryMessagesThread : public ThreadWindows
{
private:
	WhatsappDatabase &database;
	sqlite3 *sqLiteDatabase;
	const std::string &chatId;
	std::vector<WhatsappMessage *> &messages;

public:
	QueryMessagesThread(WhatsappDatabase &database, sqlite3 *sqLiteDatabase, const std::string &chatId, std::vector<WhatsappMessage *> &messages);
	virtual ~QueryMessagesThread();

	virtual void interrupt();
	virtual void run();

};
