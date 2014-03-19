#include <windows.h>
#include <iostream>
#include <iomanip>

#include "Log.h"
#include "Main.h"

#include "AES/AES.h"
#include "AES/PolarSSL.h"

Log log;
int blocksize = 16;

void outputHexBuffer(const char *buffer, int length)
{
	int blocks = (length + blocksize - 1) / blocksize;

	for (size_t block = 0; block < blocks; block++)
	{
		for (int byte = 0; byte < blocksize; byte++)
		{
			log << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << ((int)buffer[block * blocksize + byte] & 0xFF) << " ";
		}

		log << std::endl;
	}
}

void outputHexBuffer(const unsigned char *buffer, int length)
{
	int blocks = (length + blocksize - 1) / blocksize;

	for (size_t block = 0; block < blocks; block++)
	{
		for (int byte = 0; byte < blocksize; byte++)
		{
			log << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << ((int)buffer[block * blocksize + byte] & 0xFF) << " ";
		}

		log << std::endl;
	}
}

void encrypt(unsigned char *buffer, int length, const char *password)
{
	aes_context ctx;
	unsigned char key[32];
	unsigned char iv[16];
	memset( key, 0, 32 );
	memset( iv , 0, 16 );
	memcpy(key, password, strlen(password));

	log << "Key: " << password << std::endl;
	outputHexBuffer(key, 32);
	log << std::endl;

	log << "Length: " << length << std::endl;
	log << "Init Vector: " << std::endl;
	outputHexBuffer(iv, 16);
	log << std::endl;

	aes_setkey_enc( &ctx, key, 256);
	if (aes_crypt_cbc( &ctx, AES_ENCRYPT, length, iv, buffer, buffer ) != 0)
	{
		log << "error encrypting" << std::endl;
	}
}

void decrypt(char *buffer, int length, const char *password)
{
}

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int commandShow)
{
	log << "WhatsApp Reader" << std::endl;

	const char *plaintext = "Hello World!123";
	char *password = "MyPasswordMyPasswordMyPassword12";

	int length = strlen(plaintext) + 1;

	log << "Plaintext: " << std::endl;
	outputHexBuffer(plaintext, length);
	log << std::endl;

	unsigned char *buffer = new unsigned char[length];
	memcpy(buffer, plaintext, length);

	encrypt(buffer, length, password);

	outputHexBuffer(buffer, 16);
	log << std::endl;

	//aes_context ctx;
	//unsigned char key[32];
	//unsigned char iv[16];
	//unsigned char prv[16];
	//unsigned char buf[64];
	//memset( key, 0, 32 );
	//int u, v;

	//for(int i = 0; i < 6; i++ )
 //   {
 //       u = i >> 1;
 //       v = i  & 1;

	//	log << "  AES-CBC-" << 128 + u * 64 << " (" << (( v == AES_DECRYPT ) ? "dec" : "enc") << "): ";

 //       memset( iv , 0, 16 );
 //       memset( prv, 0, 16 );
 //       memset( buf, 0, 16 );

 //       if( v == AES_DECRYPT )
 //       {
 //           aes_setkey_dec( &ctx, key, 128 + u * 64 );

 //           for(int j = 0; j < 10000; j++ )
 //               aes_crypt_cbc( &ctx, v, 16, iv, buf, buf );

 //       }
 //       else
 //       {
 //           aes_setkey_enc( &ctx, key, 128 + u * 64 );

 //           for(int j = 0; j < 10000; j++ )
 //           {
 //               unsigned char tmp[16];

 //               aes_crypt_cbc( &ctx, v, 16, iv, buf, buf );

 //               memcpy( tmp, prv, 16 );
 //               memcpy( prv, buf, 16 );
 //               memcpy( buf, tmp, 16 );
 //           }

 //       }
 //   }

	/*AES aes;
	int blocks = aes.encrypt(&buffer, length, password);

	log << "Encrypted block count: " << blocks << std::endl;
	outputHexBuffer(buffer, blocks * blocksize);
	log << std::endl;

	int decryptedLength = aes.decrypt(&buffer, length, password);
	log << "Decrypted length: " << decryptedLength << std::endl;
	outputHexBuffer(buffer, decryptedLength);*/

	return 0;
}
