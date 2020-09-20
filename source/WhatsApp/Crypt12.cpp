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

void decryptWhatsappDatabase12(const std::string &filename, const std::string &filenameDecrypted, unsigned char *key)
{
	std::ifstream file(filename, std::ios::binary);

	file.seekg(0, std::ios::end);
	std::streamoff filesize = file.tellg();

	unsigned char initVector[16];
	file.seekg(51, std::ios::beg);
	file.read(reinterpret_cast<char*>(initVector), 16);

	std::streamoff databaseSize = filesize - skipBytesCrypt7 - 20;
	const std::string tempFilename = filenameDecrypted + ".temp";

	{
		std::ofstream decryptedFile(tempFilename, std::ios::binary);
		decrypt_aes_gcm(file, databaseSize, key, initVector, decryptedFile);
	}

	{
		std::ifstream decryptedFile(tempFilename, std::ios::binary);
		std::ofstream uncompressedFile(filenameDecrypted, std::ios::binary);
		uncompressGzipBuffer(decryptedFile, uncompressedFile);
	}

	std::remove(tempFilename.c_str());

	{
		std::ifstream uncompressedFile(filenameDecrypted, std::ios::binary);
		validateOutput(uncompressedFile);
	}
}

void decryptWhatsappDatabase12(const std::string& filename, const std::string& filenameDecrypted, const std::string& keyFilename)
{
	unsigned char key[32];
	unsigned char iv[16];

	loadKey(keyFilename, key, iv);
	decryptWhatsappDatabase12(filename, filenameDecrypted, key);
}
