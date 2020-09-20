#pragma once

void decryptWhatsappDatabase5(const std::string &filename, const std::string &filenameDecrypted, unsigned char *key);
void decryptWhatsappDatabase5(const std::string &filename, const std::string &filenameDecrypted, const std::string &accountName);

void validateOutput(std::istream& input);
