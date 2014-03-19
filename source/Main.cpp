#include <windows.h>
#include <iostream>
#include <iomanip>

#include "Log.h"
#include "Main.h"

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
	log.logHexBuffer(database, filesize);

	delete[] database;

	return 0;
}
