#pragma once

void decryptWhatsappDatabase7(const std::string &filename, const std::string &filenameDecrypted, const std::string &keyFilename);
void extractKey(const std::string &keyFilename, unsigned char *key, unsigned char *iv);
