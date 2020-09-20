#pragma once

void decryptWhatsappDatabase8(const std::string &filename, const std::string &filenameDecrypted, const std::string &keyFilename);
void uncompressGzipBuffer(std::istream& compressed, std::ostream& uncompressed);
