#include "PolarSSL.h"

int aesBlocksize = 16;

void encrypt_aes_cbc_192(const unsigned char *input, unsigned char *output, int length, const unsigned char *key, unsigned char *initVector)
{
	aes_context ctx;

	aes_setkey_enc(&ctx, key, 192);
	if (aes_crypt_cbc(&ctx, AES_ENCRYPT, length, initVector, input, output) != 0)
	{
		throw 1;
	}
}

void decrypt_aes_cbc_192(const unsigned char *input, unsigned char *output, int length, const unsigned char *key, unsigned char *initVector)
{
	aes_context ctx;

	aes_setkey_dec(&ctx, key, 192);
	if (aes_crypt_cbc(&ctx, AES_DECRYPT, length, initVector, input, output) != 0)
	{
		throw 1;
	}
}
