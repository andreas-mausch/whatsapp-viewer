#pragma once

void decryptWhatsappDatabase8(const std::string &filename, const std::string &filenameDecrypted, const std::string &keyFilename);
void extractKey8(const std::string &keyFilename, const std::string &dbFilename, unsigned char *key, unsigned char *iv);
void uncompressGzipBuffer(unsigned char *compressedBytes, int size, std::vector<unsigned char> &uncompressed);
