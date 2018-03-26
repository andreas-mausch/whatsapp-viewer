#pragma once

#include <string>
#include <vector>
#include "../Platforms/Win32/ThreadWindows.h"

class SQLiteDatabase;
class WhatsappDatabase;
class WhatsappMessage;

class QueryMessagesThread : public ThreadWindows
{
private:
	WhatsappDatabase &database;
	SQLiteDatabase &sqLiteDatabase;
	const std::string &chatId;
	std::vector<WhatsappMessage *> &messages;

public:
	QueryMessagesThread(WhatsappDatabase &database, SQLiteDatabase &sqLiteDatabase, const std::string &chatId, std::vector<WhatsappMessage *> &messages);
	virtual ~QueryMessagesThread();

	virtual void interrupt();
	virtual void run();

};
