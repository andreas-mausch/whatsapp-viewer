#pragma once

#include <string>

namespace WhatsApp::Crypt {

void decrypt12(const std::string &filename, unsigned char *key, const std::string &filenameDecrypted);
void decrypt12(const std::string& filename, const std::string& keyFilename, const std::string& filenameDecrypted);
void loadKey(const std::string &filename, unsigned char *key, unsigned char *iv);
void validateOutput(std::istream &input);

}
