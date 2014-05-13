#pragma once

long loadFile(const std::string &filename, char **output);
long loadFileUnsigned(const std::string &filename, unsigned char **output);

void buildKey(unsigned char *key, const std::string &accountName);
void decryptWhatsappDatabase5(const std::string &filename, const std::string &filenameDecrypted, unsigned char *key);
