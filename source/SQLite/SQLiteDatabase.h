#pragma once

#include <string>

struct sqlite3;

class SQLiteDatabase
{
private:
	sqlite3* database;

public:
	SQLiteDatabase(const std::string &filename);
	~SQLiteDatabase();

	int getErrorCode();
	std::string getErrorMessage();

	sqlite3* getHandle();

};
