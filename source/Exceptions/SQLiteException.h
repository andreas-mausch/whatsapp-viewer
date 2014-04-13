#pragma once

#include "Exception.h"

class WhatsappDatabase;

class SQLiteException : public Exception
{
private:
	static std::string buildCause(const std::string &reason, WhatsappDatabase &database);
public:
	SQLiteException(const std::string &reason, WhatsappDatabase &database);
};
