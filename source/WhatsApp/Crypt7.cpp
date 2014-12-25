#include <iostream>
#include <fstream>

#include "../Libraries/AES/AES.h"
#include "../Exceptions/Exception.h"
#include "../Libraries/MD5/MD5.h"
#include "Crypt5.h"
#include "Crypt7.h"

const int skipBytesCrypt7 = 67;

void extractKey(const std::string &keyFilename, unsigned char *key, unsigned char *iv)
{
	unsigned char *keyBytes;
	int filesize = loadFileUnsigned(keyFilename, &keyBytes);

	if (filesize != 158)
	{
		throw Exception("Expected key filesize of 158 bytes does not match.");
	}

	memcpy(iv, &keyBytes[110], 16);
	memcpy(key, &keyBytes[126], 32);

	delete[] keyBytes;
}

void decryptAes(unsigned char *crypted, unsigned char *uncrypted, unsigned char *key, unsigned char *initVector, int size)
{
	unsigned char iv[16];
	memcpy(iv, initVector, 16);

	decrypt_aes_cbc_256(crypted, uncrypted, size, key, iv);
}

void decryptWhatsappDatabase7(const std::string &filename, const std::string &filenameDecrypted, unsigned char *key, unsigned char *initVector)
{
	unsigned char *fileBytes;
	int filesize = loadFileUnsigned(filename, &fileBytes);
	int databaseSize = filesize - skipBytesCrypt7;
	unsigned char *databaseBytes = &fileBytes[skipBytesCrypt7];

	decryptAes(databaseBytes, databaseBytes, key, initVector, databaseSize);
	validateOutput(databaseBytes);
	saveOutputToFile(databaseBytes, databaseSize, filenameDecrypted);

	delete[] fileBytes;
}

void decryptWhatsappDatabase7(const std::string &filename, const std::string &filenameDecrypted, const std::string &keyFilename)
{
	unsigned char key[32];
	unsigned char iv[16];

	extractKey(keyFilename, key, iv);
	decryptWhatsappDatabase7(filename, filenameDecrypted, key, iv);
}
