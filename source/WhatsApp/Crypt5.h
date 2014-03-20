#pragma once

long loadFile(const std::string &filename, char **output);
long loadFileUnsigned(const std::string &filename, unsigned char **output);

void buildKey(unsigned char *key, const char *accountName);
void decryptWhatsappDatabase(const std::string &filename, unsigned char *key);
