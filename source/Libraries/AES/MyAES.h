#pragma once

void decrypt_aes_cbc(int keySize, std::istream& input, std::streamoff length, const unsigned char* key, unsigned char* initVector, std::ostream& output);
void decrypt_aes_gcm(std::istream& input, std::streamoff length, const unsigned char* key, unsigned char* initVector, std::ostream& output);

extern int aesBlocksize;
