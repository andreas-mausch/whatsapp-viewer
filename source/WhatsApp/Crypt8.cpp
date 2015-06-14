#include <fstream>
#include <iostream>
#include <vector>

#include "../Exceptions/Exception.h"
#include "../Libraries/AES/AES.h"
#include "../Libraries/Zip/zlib.h"
#include "Crypt5.h"
#include "Crypt7.h"

const int chunk = 16384;

void extractKey8(const std::string &keyFilename, const std::string &dbFilename, unsigned char *key, unsigned char *iv)
{
	unsigned char *keyBytes;
	int filesize = loadFileUnsigned(keyFilename, &keyBytes);

	if (filesize != 158)
	{
		throw Exception("Expected key filesize of 158 bytes does not match.");
	}

	unsigned char *dbBytes;
	int filesizeDB = loadFileUnsigned(dbFilename, &dbBytes);

	// Initialisation vector is stored in the msgstore after v2-12-38
	// hexdump -n 67 -e '2/1 "%02x"' msgstore.db.crypt8 | cut -b 103-134 > iv.txt
	// last 16 bytes from the db store starting at byte 51
	// see http://forum.xda-developers.com/android/apps-games/decrypting-whatsapp-crypt8-v2-12-38-t3083847
	
	memcpy(iv, &dbBytes[51], 16);
	memcpy(key, &keyBytes[126], 32);

	delete[] dbBytes;
	delete[] keyBytes;
}

void uncompressBlock(z_stream &stream, std::vector<unsigned char> &uncompressed)
{
	unsigned char out[chunk];

	do
	{
		stream.avail_out = chunk;
		stream.next_out = out;
		int ret = inflate(&stream, Z_NO_FLUSH);

		if (ret != Z_OK && ret != Z_STREAM_END)
		{
			inflateEnd(&stream);
			throw Exception("Decryption failed. Error during unzipping (inflate).");
		}

		unsigned int have = chunk - stream.avail_out;

		for (unsigned int i = 0; i < have; i++)
		{
			uncompressed.push_back(out[i]);
		}
	} while (stream.avail_out == 0);
}

void uncompressGzipBuffer(unsigned char *compressedBytes, int size, std::vector<unsigned char> &uncompressed)
{
	unsigned char in[chunk];

	z_stream stream;
	memset(&stream, 0, sizeof(z_stream));

	int ret = inflateInit2(&stream, 16 + MAX_WBITS);
	if (ret != Z_OK)
	{
		throw Exception("Decryption failed. Error during unzipping (inflateInit). Invalid key?");
	}

	unsigned char *currentPosition = compressedBytes;

	do
	{
		int bytesLeft = size - (currentPosition - compressedBytes);
		if (bytesLeft == 0)
		{
            break;
		}

		int bytesToCopy = bytesLeft;
		if (bytesLeft > chunk)
		{
			bytesToCopy = chunk;
		}

		memcpy(in, currentPosition, bytesToCopy);
        stream.avail_in = bytesToCopy;
        stream.next_in = in;

		uncompressBlock(stream, uncompressed);

		currentPosition += bytesToCopy;
	} while (ret != Z_STREAM_END);

	inflateEnd(&stream);
}

void decryptWhatsappDatabase8(const std::string &filename, const std::string &filenameDecrypted, unsigned char *key, unsigned char *initVector)
{
	unsigned char *fileBytes;
	int filesize = loadFileUnsigned(filename, &fileBytes);
	int databaseSize = filesize - skipBytesCrypt7;
	unsigned char *databaseBytes = &fileBytes[skipBytesCrypt7];

	decryptAes(databaseBytes, databaseBytes, key, initVector, databaseSize);

	std::vector<unsigned char> uncompressed;
	uncompressGzipBuffer(databaseBytes, databaseSize, uncompressed);

	validateOutput(&uncompressed[0]);
	saveOutputToFile(&uncompressed[0], uncompressed.size(), filenameDecrypted);

	delete[] fileBytes;
}

void decryptWhatsappDatabase8(const std::string &filename, const std::string &filenameDecrypted, const std::string &keyFilename)
{
	unsigned char key[32];
	unsigned char iv[16];

	extractKey8(keyFilename, filename, key, iv);
	decryptWhatsappDatabase8(filename, filenameDecrypted, key, iv);
}
