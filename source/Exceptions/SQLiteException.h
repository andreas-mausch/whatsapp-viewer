#pragma once

#include "Exception.h"

class SQLiteDatabase;

class SQLiteException : public Exception
{
private:
	static std::string buildCause(const std::string &reason, SQLiteDatabase &database);
public:
	SQLiteException(const std::string &reason, SQLiteDatabase &database);
};
