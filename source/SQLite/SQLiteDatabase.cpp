#include "SQLiteDatabase.h"
#include "sqlite3.h"
#include "../Exceptions/SQLiteException.h"

SQLiteDatabase::SQLiteDatabase(const std::string &filename)
{
	if(sqlite3_open(filename.c_str(), &database) != SQLITE_OK)
	{
		throw SQLiteException("Could not open SQLite database", *this);
	}
}

SQLiteDatabase::~SQLiteDatabase()
{
	sqlite3_close(database);
}

sqlite3* SQLiteDatabase::getHandle()
{
	return database;
}

int SQLiteDatabase::getErrorCode()
{
	return sqlite3_errcode(database);
}

std::string SQLiteDatabase::getErrorMessage()
{
	return sqlite3_errmsg(database);
}
