#pragma once

namespace Crypt {

void decrypt_aes_cbc(std::istream& input, std::streamoff length, const unsigned char* key, int keySize, unsigned char* initVector, std::ostream& output);
void decrypt_aes_gcm(std::istream& input, std::streamoff length, const unsigned char* key, unsigned char* initVector, std::ostream& output);

}
