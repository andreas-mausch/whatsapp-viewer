#pragma once

extern const int skipBytesCrypt7;

void decryptWhatsappDatabase7(const std::string &filename, const std::string &filenameDecrypted, const std::string &keyFilename);
void extractKey(const std::string &keyFilename, unsigned char *key, unsigned char *iv);
void decryptAes(unsigned char *crypted, unsigned char *uncrypted, unsigned char *key, unsigned char *initVector, int size);
