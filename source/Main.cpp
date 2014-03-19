#include <windows.h>
#include <iostream>
#include <iomanip>

#include "Log.h"
#include "Main.h"

#include "AES/AES.h"
#include "AES/PolarSSL.h"

Log log;
int blocksize = 16;

void outputHexBuffer(const char *buffer, int length)
{
	int blocks = (length + blocksize - 1) / blocksize;

	for (size_t block = 0; block < blocks; block++)
	{
		for (int byte = 0; byte < blocksize; byte++)
		{
			log << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << ((int)buffer[block * blocksize + byte] & 0xFF) << " ";
		}

		log << std::endl;
	}
}

void outputHexBuffer(const unsigned char *buffer, int length)
{
	int blocks = (length + blocksize - 1) / blocksize;

	for (size_t block = 0; block < blocks; block++)
	{
		for (int byte = 0; byte < blocksize; byte++)
		{
			log << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << ((int)buffer[block * blocksize + byte] & 0xFF) << " ";
		}

		log << std::endl;
	}
}

void encrypt_aes_cbc_192(unsigned char *buffer, int length, const unsigned char *key, unsigned char *iv)
{
	aes_context ctx;

	log << "Key: " << std::endl;
	outputHexBuffer(key, 32);
	log << std::endl;

	log << "Length: " << length << std::endl;
	log << "Init Vector: " << std::endl;
	outputHexBuffer(iv, 16);
	log << std::endl;

	aes_setkey_enc( &ctx, key, 192);
	if (aes_crypt_cbc( &ctx, AES_ENCRYPT, length, iv, buffer, buffer ) != 0)
	{
		log << "error encrypting" << std::endl;
	}
}

void decrypt_aes_cbc_192(unsigned char *buffer, int length, const unsigned char *key, unsigned char *iv)
{
	aes_context ctx;

	log << "Key: " << std::endl;
	outputHexBuffer(key, 32);
	log << std::endl;

	log << "Length: " << length << std::endl;
	log << "Init Vector: " << std::endl;
	outputHexBuffer(iv, 16);
	log << std::endl;

	aes_setkey_dec( &ctx, key, 192);
	if (aes_crypt_cbc( &ctx, AES_DECRYPT, length, iv, buffer, buffer ) != 0)
	{
		log << "error encrypting" << std::endl;
	}
}

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int commandShow)
{
	log << "WhatsApp Reader" << std::endl;

	unsigned char key[] = { 141, 75, 21, 92, 201, 255, 129, 229, 203, 246, 250, 120, 25, 54, 106, 62, 198, 33, 166, 86, 65, 108, 215, 147 };
	unsigned char iv[] = { 0x1E,0x39,0xF3,0x69,0xE9,0xD,0xB3,0x3A,0xA7,0x3B,0x44,0x2B,0xBB,0xB6,0xB0,0xB9 };

	const char *accountName = "neonew.mobile@googlemail.com";
	unsigned char accountNameMd5[] = { 0x47, 0xac, 0xfb, 0x8f, 0xa4, 0x42, 0x68, 0x87, 0x75, 0x6c, 0x74, 0xd2, 0xee, 0x90, 0x17, 0x78 };

	for (int i = 0; i < 24; i++)
	{
		key[i] ^= accountNameMd5[i & 0xF];
	}

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

	decrypt_aes_cbc_192(database, filesize, key, iv);
	outputHexBuffer(database, filesize);

	delete[] database;

	return 0;
}
