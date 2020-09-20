#include <iostream>
#include <fstream>

#include "../Libraries/AES/MyAES.h"
#include "../Exceptions/Exception.h"
#include "../Libraries/MD5/MD5.h"
#include "Crypt5.h"
#include "Crypt7.h"

const int skipBytesCrypt7 = 67;

void loadKey(const std::string &filename, unsigned char *key, unsigned char *iv)
{
	std::ifstream file(filename, std::ios::binary);
	file.seekg(0, std::ios::end);
	std::streamoff filesize = file.tellg();

	if (filesize != 158)
	{
		throw Exception("Expected key filesize of 158 bytes does not match.");
	}

	file.seekg(110, std::ios::beg);
	file.read(reinterpret_cast<char*>(iv), 16);
	file.read(reinterpret_cast<char*>(key), 32);
}

void decryptWhatsappDatabase7(const std::string &filename, const std::string &filenameDecrypted, unsigned char *key)
{
	std::ifstream file(filename, std::ios::binary);

	file.seekg(0, std::ios::end);
	std::streamoff filesize = file.tellg();

	unsigned char initVector[16];
	file.seekg(51, std::ios::beg);
	file.read(reinterpret_cast<char*>(initVector), 16);

	std::streamoff databaseSize = filesize - skipBytesCrypt7;

	{
		std::ofstream decryptedFile(filenameDecrypted, std::ios::binary);
		decrypt_aes_cbc(256, file, databaseSize, key, initVector, decryptedFile);
	}

	{
		std::ifstream decryptedFile(filenameDecrypted, std::ios::binary);
		validateOutput(decryptedFile);
	}
}

void decryptWhatsappDatabase7(const std::string &filename, const std::string &filenameDecrypted, const std::string &keyFilename)
{
	unsigned char key[32];
	unsigned char iv[16];

	loadKey(keyFilename, key, iv);
	decryptWhatsappDatabase7(filename, filenameDecrypted, key);
}
