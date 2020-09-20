#include <iostream>
#include <fstream>

#include "../Libraries/AES/MyAES.h"
#include "../Exceptions/Exception.h"
#include "../Libraries/MD5/MD5.h"
#include "Crypt5.h"
#include "Crypt7.h"

const int skipBytesCrypt7 = 67;

void extractKey(const std::string &keyFilename, unsigned char *key, unsigned char *iv)
{
	unsigned char *keyBytes;
	int filesize = loadFile(keyFilename, &keyBytes);

	if (filesize != 158)
	{
		throw Exception("Expected key filesize of 158 bytes does not match.");
	}

	memcpy(iv, &keyBytes[110], 16);
	memcpy(key, &keyBytes[126], 32);

	delete[] keyBytes;
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
		decrypt_aes_cbc_256(file, databaseSize, key, initVector, decryptedFile);
	}

	{
		std::ifstream uncompressedFile(filenameDecrypted, std::ios::binary);
		validateOutput(uncompressedFile);
	}
}

void decryptWhatsappDatabase7(const std::string &filename, const std::string &filenameDecrypted, const std::string &keyFilename)
{
	unsigned char key[32];
	unsigned char iv[16];

	extractKey(keyFilename, key, iv);
	decryptWhatsappDatabase7(filename, filenameDecrypted, key);
}
