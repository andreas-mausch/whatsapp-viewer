#include <iostream>
#include <fstream>

#include "../Libraries/AES/MyAES.h"
#include "../Exceptions/Exception.h"
#include "../Libraries/MD5/MD5.h"
#include "Crypt5.h"

const unsigned char baseKey[] = { 141, 75, 21, 92, 201, 255, 129, 229, 203, 246, 250, 120, 25, 54, 106, 62, 198, 33, 166, 86, 65, 108, 215, 147 };
const unsigned char initVector[] = { 0x1E,0x39,0xF3,0x69,0xE9,0xD,0xB3,0x3A,0xA7,0x3B,0x44,0x2B,0xBB,0xB6,0xB0,0xB9 };

void buildKey(unsigned char *key, const std::string &accountName)
{
	MD5 md5;
	unsigned char *accountNameMd5 = md5.digestStringRaw(accountName.c_str());

	for (int i = 0; i < 24; i++)
	{
		key[i] = baseKey[i] ^ accountNameMd5[i & 0xF];
	}
}

void validateOutput(std::istream &input)
{
	const char expectedBytes[] = "SQLite format 3";
	char read[sizeof(expectedBytes)];

	input.read(read, sizeof(expectedBytes));

	if (input.gcount() != sizeof(expectedBytes))
	{
		throw Exception("Couldn't validate output file");
	}
	if (memcmp(read, expectedBytes, sizeof(expectedBytes)) != 0)
	{
		throw Exception("Validation failed. Invalid key file or account name?");
	}
}

void decryptWhatsappDatabase5(const std::string &filename, const std::string &filenameDecrypted, unsigned char *key)
{
	std::ifstream file(filename, std::ios::binary);

	file.seekg(0, std::ios::end);
	std::streamoff filesize = file.tellg();

	unsigned char iv[16];
	memcpy(iv, initVector, 16);

	{
		std::ofstream decryptedFile(filenameDecrypted, std::ios::binary);
		file.seekg(0, std::ios::beg);
		decrypt_aes_cbc(192, file, filesize, key, iv, decryptedFile);
	}

	{
		std::ifstream decryptedFile(filenameDecrypted, std::ios::binary);
		validateOutput(decryptedFile);
	}
}

void decryptWhatsappDatabase5(const std::string &filename, const std::string &filenameDecrypted, const std::string &accountName)
{
	unsigned char key[24];
	buildKey(key, accountName);
	decryptWhatsappDatabase5(filename, filenameDecrypted, key);
}
