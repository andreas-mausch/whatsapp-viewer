#include <fstream>
#include <iostream>
#include <vector>

#include "../Exceptions/Exception.h"
#include "../Libraries/AES/MyAES.h"
#include "../Libraries/Zip/zlib.h"
#include "Crypt5.h"
#include "Crypt7.h"

const int chunk = 16 * 1024;

int uncompressBlock(z_stream& stream, std::ostream &uncompressed)
{
	unsigned char out[chunk];
	int ret = 0;

	do
	{
		stream.avail_out = chunk;
		stream.next_out = out;
		ret = inflate(&stream, Z_NO_FLUSH);

		if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR)
		{
			inflateEnd(&stream);
			throw Exception("Decryption failed. Error during unzipping (inflate). In 99% this means you tried to use an invalid key.");
		}

		unsigned int have = chunk - stream.avail_out;
		uncompressed.write(reinterpret_cast<char*>(out), have);
	} while (stream.avail_out == 0);

	return ret;
}

void uncompressGzipBuffer(std::istream &compressed, std::ostream &uncompressed)
{
	unsigned char in[chunk];

	z_stream stream;
	memset(&stream, 0, sizeof(z_stream));

	int ret = inflateInit2(&stream, 15 | 32);
	if (ret != Z_OK)
	{
		throw Exception("Decryption failed. Error during unzipping (inflateInit). Invalid key?");
	}

	do
	{
		if (!compressed || compressed.eof())
		{
			throw Exception("Couldn't read compressed file (unexpected EOF).");
		}

		compressed.read(reinterpret_cast<char*>(in), chunk);
		std::streamsize bytesRead = compressed.gcount();
		stream.avail_in = bytesRead;
		stream.next_in = in;

		ret = uncompressBlock(stream, uncompressed);
	} while (ret != Z_STREAM_END);

	inflateEnd(&stream);
}

void decryptWhatsappDatabase8(const std::string &filename, const std::string &filenameDecrypted, unsigned char *key)
{
	std::ifstream file(filename, std::ios::binary);

	file.seekg(0, std::ios::end);
	std::streamoff filesize = file.tellg();

	unsigned char initVector[16];
	file.seekg(51, std::ios::beg);
	file.read(reinterpret_cast<char*>(initVector), 16);

	std::streamoff databaseSize = filesize - skipBytesCrypt7;
	const std::string tempFilename = filenameDecrypted + ".temp";

	{
		std::ofstream decryptedFile(tempFilename, std::ios::binary);
		decrypt_aes_cbc(256, file, databaseSize, key, initVector, decryptedFile);
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

void decryptWhatsappDatabase8(const std::string &filename, const std::string &filenameDecrypted, const std::string &keyFilename)
{
	unsigned char key[32];
	unsigned char iv[16];

	loadKey(keyFilename, key, iv);
	decryptWhatsappDatabase8(filename, filenameDecrypted, key);
}
