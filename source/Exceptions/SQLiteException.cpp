#include<sstream>

#include "SQLiteException.h"
#include "../Libraries/SQLite/SQLiteDatabase.h"

SQLiteException::SQLiteException(const std::string &reason, SQLiteDatabase &database)
	: Exception(buildCause(reason, database))
{
}

std::string SQLiteException::buildCause(const std::string &reason, SQLiteDatabase &database)
{
	int errorCode = database.getErrorCode();
	std::string errorMessage = database.getErrorMessage();

	std::ostringstream cause;
	cause << reason << "; SQLite error: " << errorCode << "; " << errorMessage;

	return cause.str();
}
