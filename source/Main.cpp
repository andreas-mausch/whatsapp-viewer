#include <windows.h>
#include <iostream>
#include <iomanip>

#include "Log.h"
#include "Main.h"
#include "SQLite/sqlite3.h"

#include "AES/AES.h"

Log log;

void buildKey(unsigned char *key, const char *accountName)
{
	const unsigned char baseKey[] = { 141, 75, 21, 92, 201, 255, 129, 229, 203, 246, 250, 120, 25, 54, 106, 62, 198, 33, 166, 86, 65, 108, 215, 147 };
	unsigned char accountNameMd5[] = { 0x47, 0xac, 0xfb, 0x8f, 0xa4, 0x42, 0x68, 0x87, 0x75, 0x6c, 0x74, 0xd2, 0xee, 0x90, 0x17, 0x78 };

	for (int i = 0; i < 24; i++)
	{
		key[i] = baseKey[i] ^ accountNameMd5[i & 0xF];
	}
}

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int commandShow)
{
	log << "WhatsApp Reader" << std::endl;

	unsigned char key[24];
	unsigned char initVector[] = { 0x1E,0x39,0xF3,0x69,0xE9,0xD,0xB3,0x3A,0xA7,0x3B,0x44,0x2B,0xBB,0xB6,0xB0,0xB9 };

	const char *accountName = "neonew.mobile@googlemail.com";
	buildKey(key, accountName);

	std::ifstream file("../data/msgstore.db.crypt5", std::ios::binary);
	file.seekg(0, std::ios::end);
	int filesize = file.tellg();
	file.seekg(0, std::ios::beg);

	char *databaseCryptedSigned = new char[filesize];
	file.read(databaseCryptedSigned, filesize);
	file.close();

	unsigned char *database = new unsigned char[filesize];
	memcpy(database, databaseCryptedSigned, filesize);
	delete[] databaseCryptedSigned;

	log << "Database: " << filesize << std::endl;
	// outputHexBuffer(databaseCrypted, filesize);

	decrypt_aes_cbc_192(database, database, filesize, key, initVector);
	// log.logHexBuffer(database, filesize);

	delete[] database;

	sqlite3* sqLiteDatabase;
	if(sqlite3_open("../data/msgstore.db", &sqLiteDatabase) != SQLITE_OK)
	{
		throw 2;
	}

	sqlite3_stmt    *res;
	const char      *tail;
	int             rec_count = 0;
	int error = sqlite3_prepare_v2(sqLiteDatabase,
        "SELECT * FROM chat_list",
        1000, &res, &tail);

	if (error != SQLITE_OK)
		{
		puts("We did not get any data!");
		return 0;
		}

	puts("==========================");

	while (sqlite3_step(res) == SQLITE_ROW)
	{
		const unsigned char *key = sqlite3_column_text(res, 1);
		const unsigned char *subject = sqlite3_column_text(res, 3);
		if (key != NULL)
		{
			log << "Key: " << key << "; ";
		}
		if (subject != NULL)
		{
			log << "Subject: " << subject;
		}
		log << std::endl;

		rec_count++;
	}

	log << "We received records: " << rec_count << std::endl << std::endl;

	error = sqlite3_prepare_v2(sqLiteDatabase,
        "SELECT * FROM messages where key_remote_jid = '12468280372@s.whatsapp.net' order by timestamp asc",
        1000, &res, &tail);

	if (error != SQLITE_OK)
		{
		puts("We did not get any data!");
		return 0;
		}

	puts("==========================");

	while (sqlite3_step(res) == SQLITE_ROW)
	{
		const unsigned char *data = sqlite3_column_text(res, 6);
		if (data != NULL)
		{
			log << "Data: " << data << std::endl;
		}
	}

	sqlite3_close(sqLiteDatabase);

	return 0;
}
