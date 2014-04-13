#pragma once

#include <string>
#include <vector>

struct sqlite3;
class WhatsappChat;
class WhatsappMessage;

class WhatsappDatabase
{
private:
	sqlite3* sqLiteDatabase;

public:
	WhatsappDatabase(const std::string &filename);
	~WhatsappDatabase();

	void getChats(std::vector<WhatsappChat *> &chats);
	void getMessages(const std::string &chatId, std::vector<WhatsappMessage *> &messages);

	int getErrorCode();
	std::string getErrorMessage();

};
