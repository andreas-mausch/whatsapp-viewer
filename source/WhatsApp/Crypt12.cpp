#include <fstream>
#include <iostream>
#include <vector>

#include "../Exceptions/Exception.h"
#include "../Libraries/AES/MyAES.h"
#include "../Libraries/AES/mbedtls/gcm.h"
#include "../Libraries/Zip/zlib.h"
#include "Crypt5.h"
#include "Crypt7.h"
#include "Crypt8.h"

const int chunk = 16384;

void decryptWhatsappDatabase12(const std::string &filename, const std::string &filenameDecrypted, unsigned char *key);
void decryptAesGcm(const unsigned char *crypted, unsigned char *uncrypted, unsigned char *key, unsigned char *initVector, int size);

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
	int filesize = loadFileUnsigned(filename, &fileBytes);
	int databaseSize = filesize - skipBytesCrypt7 - 20;
	unsigned char *cryptedDatabaseBytes = new unsigned char[databaseSize];
	memcpy(cryptedDatabaseBytes, &fileBytes[skipBytesCrypt7], databaseSize);

	unsigned char initVector[16];
	memcpy(initVector, &fileBytes[51], 16);
	unsigned char *decryptedDatabaseBytes = new unsigned char[databaseSize];

	decryptAesGcm(cryptedDatabaseBytes, decryptedDatabaseBytes, key, initVector, databaseSize);
	saveOutputToFile(decryptedDatabaseBytes, databaseSize, "test.out");

	std::vector<unsigned char> uncompressed;
	uncompressGzipBuffer(decryptedDatabaseBytes, databaseSize, uncompressed);

	validateOutput(&uncompressed[0]);
	saveOutputToFile(&uncompressed[0], uncompressed.size(), filenameDecrypted);

	delete[] fileBytes;
	delete[] cryptedDatabaseBytes;
	delete[] decryptedDatabaseBytes;
}

void decryptAesGcm(const unsigned char *crypted, unsigned char *uncrypted, unsigned char *key, unsigned char *initVector, int size)
{
	mbedtls_gcm_context context;

	mbedtls_gcm_init(&context);
	if (mbedtls_gcm_setkey(&context, MBEDTLS_CIPHER_ID_AES, key, 256) != 0) {
		throw Exception("Could not decrypt (mbedtls_gcm_setkey)");
	}

	if (mbedtls_gcm_starts(&context, MBEDTLS_GCM_DECRYPT, initVector, 16, NULL, 0) != 0) {
		throw Exception("Could not decrypt (mbedtls_gcm_starts)");
	}
	if (mbedtls_gcm_update(&context, size, crypted, uncrypted) != 0) {
		throw Exception("Could not decrypt (mbedtls_gcm_update)");
	}
	mbedtls_gcm_finish(&context, NULL, 0);
	mbedtls_gcm_free(&context);
}
