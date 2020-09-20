#include <iostream>

#include "mbedtls/aes.h"
#include "mbedtls/gcm.h"
#include "../../Exceptions/Exception.h"

int aesBlocksize = 16;

void decrypt_aes_cbc(int keySize, std::istream& input, std::streamoff length, const unsigned char* key, unsigned char* initVector, std::ostream& output)
{
	mbedtls_aes_context context;
	mbedtls_aes_setkey_dec(&context, key, keySize);

	std::streamoff totalRead = 0;

	do {
		const int chunk = 16 * 1024;
		unsigned char inputBuffer[chunk];
		unsigned char outputBuffer[chunk];
		std::streamoff toRead = (length - totalRead) < chunk ? (length - totalRead) : chunk;

		if (toRead == 0)
		{
			break;
		}

		input.read(reinterpret_cast<char*>(inputBuffer), toRead);
		std::streamsize bytesRead = input.gcount();

		if (bytesRead == 0 && !input.eof())
		{
			throw Exception("Could not read crypted file.");
		}

		totalRead += bytesRead;

		if (mbedtls_aes_crypt_cbc(&context, MBEDTLS_AES_DECRYPT, bytesRead, initVector, inputBuffer, outputBuffer) != 0) {
			throw Exception("Could not decrypt (mbedtls_aes_crypt_cbc)");
		}
		output.write(reinterpret_cast<char*>(outputBuffer), input.gcount());
	} while (!input.eof());

	mbedtls_aes_free(&context);
}

void decrypt_aes_gcm(std::istream &input, std::streamoff length, const unsigned char* key, unsigned char* initVector, std::ostream& output)
{
	mbedtls_gcm_context context;

	mbedtls_gcm_init(&context);
	if (mbedtls_gcm_setkey(&context, MBEDTLS_CIPHER_ID_AES, key, 256) != 0) {
		throw Exception("Could not decrypt (mbedtls_gcm_setkey)");
	}

	if (mbedtls_gcm_starts(&context, MBEDTLS_GCM_DECRYPT, initVector, 16, NULL, 0) != 0) {
		throw Exception("Could not decrypt (mbedtls_gcm_starts)");
	}

	std::streamoff totalRead = 0;

	do {
		const int chunk = 16 * 1024;
		unsigned char inputBuffer[chunk];
		unsigned char outputBuffer[chunk];
		std::streamoff toRead = (length - totalRead) < chunk ? (length - totalRead) : chunk;

		if (toRead == 0)
		{
			break;
		}

		input.read(reinterpret_cast<char *>(inputBuffer), toRead);
		std::streamsize bytesRead = input.gcount();

		if (bytesRead == 0 && !input.eof())
		{
			throw Exception("Could not read crypted file.");
		}

		totalRead += bytesRead;

		if (mbedtls_gcm_update(&context, bytesRead, inputBuffer, outputBuffer) != 0) {
			throw Exception("Could not decrypt (mbedtls_gcm_update)");
		}
		output.write(reinterpret_cast<char *>(outputBuffer), input.gcount());
	} while (!input.eof());

	mbedtls_gcm_finish(&context, NULL, 0);
	mbedtls_gcm_free(&context);
}
