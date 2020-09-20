#pragma once

extern const int skipBytesCrypt7;

void decryptWhatsappDatabase7(const std::string &filename, const std::string &filenameDecrypted, const std::string &keyFilename);
void loadKey(const std::string &keyFilename, unsigned char *key, unsigned char *iv);
