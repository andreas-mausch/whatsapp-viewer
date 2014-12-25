#pragma once

#include <string>

struct sqlite3;
struct sqlite3_stmt;

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

	static std::string readString(sqlite3_stmt *res, int column);

};
