#include <fstream>
#include <iostream>
#include <vector>

#include "../Exceptions/Exception.h"
#include "../Libraries/AES/MyAES.h"
#include "../Libraries/Zip/zlib.h"
#include "Crypt5.h"
#include "Crypt7.h"
#include "Crypt8.h"

const int chunk = 16384;

void decryptWhatsappDatabase12(const std::string &filename, const std::string &filenameDecrypted, unsigned char *key);

void decryptWhatsappDatabase12(const std::string &filename, const std::string &filenameDecrypted, const std::string &keyFilename)
{
	unsigned char key[32];
	unsigned char iv[16];

	extractKey8(keyFilename, filename, key, iv);
	decryptWhatsappDatabase12(filename, filenameDecrypted, key);
}

void decryptWhatsappDatabase12(const std::string &filename, const std::string &filenameDecrypted, unsigned char *key)
{
	unsigned char *fileBytes;
	int filesize = loadFile(filename, &fileBytes);
	int databaseSize = filesize - skipBytesCrypt7 - 20;
	unsigned char *cryptedDatabaseBytes = new unsigned char[databaseSize];
	memcpy(cryptedDatabaseBytes, &fileBytes[skipBytesCrypt7], databaseSize);

	unsigned char initVector[16];
	memcpy(initVector, &fileBytes[51], 16);
	unsigned char *decryptedDatabaseBytes = new unsigned char[databaseSize];

	decrypt_aes_gcm(cryptedDatabaseBytes, decryptedDatabaseBytes, databaseSize, key, initVector);

	std::vector<unsigned char> uncompressed;
	uncompressGzipBuffer(decryptedDatabaseBytes, databaseSize, uncompressed);

	validateOutput(&uncompressed[0]);
	saveOutputToFile(&uncompressed[0], uncompressed.size(), filenameDecrypted);

	delete[] fileBytes;
	delete[] cryptedDatabaseBytes;
	delete[] decryptedDatabaseBytes;
}
