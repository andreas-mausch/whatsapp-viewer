#include "mbedtls/aes.h"
#include "../../Exceptions/Exception.h"

int aesBlocksize = 16;

void encrypt_aes_cbc_192(const unsigned char *input, unsigned char *output, int length, const unsigned char *key, unsigned char *initVector)
{
	mbedtls_aes_context context;

	mbedtls_aes_init(&context);
	mbedtls_aes_setkey_enc(&context, key, 192);
	if (mbedtls_aes_crypt_cbc(&context, MBEDTLS_AES_ENCRYPT, length, initVector, input, output) != 0)
	{
		throw Exception("Could not encrypt block");
	}
	mbedtls_aes_free(&context);
}

void decrypt_aes_cbc_192(const unsigned char *input, unsigned char *output, int length, const unsigned char *key, unsigned char *initVector)
{
	mbedtls_aes_context context;

	mbedtls_aes_setkey_dec(&context, key, 192);
	if (mbedtls_aes_crypt_cbc(&context, MBEDTLS_AES_DECRYPT, length, initVector, input, output) != 0)
	{
		throw Exception("Could not decrypt block");
	}
	mbedtls_aes_free(&context);
}

void decrypt_aes_cbc_256(const unsigned char *input, unsigned char *output, int length, const unsigned char *key, unsigned char *initVector)
{
	mbedtls_aes_context context;

	mbedtls_aes_setkey_dec(&context, key, 256);
	int result = mbedtls_aes_crypt_cbc(&context, MBEDTLS_AES_DECRYPT, length, initVector, input, output);
	if (result != 0)
	{
		throw Exception("Could not decrypt block");
	}
	mbedtls_aes_free(&context);
}
