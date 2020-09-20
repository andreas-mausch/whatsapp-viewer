#pragma once

void buildKey(unsigned char *key, const std::string &accountName);
void decryptWhatsappDatabase5(const std::string &filename, const std::string &filenameDecrypted, unsigned char *key);
void decryptWhatsappDatabase5(const std::string &filename, const std::string &filenameDecrypted, const std::string &accountName);

void validateOutput(std::istream& input);

void saveOutputToFile(unsigned char *databaseBytes, int size, const std::string &filename);
