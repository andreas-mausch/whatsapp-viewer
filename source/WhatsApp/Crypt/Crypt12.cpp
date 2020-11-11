#include "Crypt12.h"

#include <fmt/core.h>

#include <cstring>
#include <fstream>

#include "../../Compression/gzip.h"
#include "../../Crypt/AES.h"

using namespace Crypt;
using namespace Compression;

namespace WhatsApp::Crypt {

const int chunk = 16384;
const int skipBytesCrypt7 = 67;

void decrypt12(const std::string &filename, unsigned char *key,
               const std::string &filenameDecrypted) {
  std::ifstream file(filename, std::ios::binary);

  if (!file) {
    throw std::runtime_error(
        fmt::format("Could not open database file '{}'.", filename));
  }

  file.seekg(0, std::ios::end);
  std::streamoff filesize = file.tellg();

  unsigned char initVector[16];
  file.seekg(51, std::ios::beg);
  file.read(reinterpret_cast<char *>(initVector), 16);

  std::streamoff databaseSize = filesize - skipBytesCrypt7 - 20;
  const std::string tempFilename = filenameDecrypted + ".temp";

  {
    std::ofstream decryptedFile(tempFilename, std::ios::binary);
    decrypt_aes_gcm(file, databaseSize, key, initVector, decryptedFile);
  }

  {
    std::ifstream decryptedFile(tempFilename, std::ios::binary);
    std::ofstream unzippedFile(filenameDecrypted, std::ios::binary);
    unzipBuffer(decryptedFile, unzippedFile);
  }

  std::remove(tempFilename.c_str());

  {
    std::ifstream unzippedFile(filenameDecrypted, std::ios::binary);
    validateOutput(unzippedFile);
  }
}

void decrypt12(const std::string &filename, const std::string &keyFilename,
               const std::string &filenameDecrypted) {
  unsigned char key[32];
  unsigned char iv[16];

  loadKey(keyFilename, key, iv);
  decrypt12(filename, key, filenameDecrypted);
}

void loadKey(const std::string &filename, unsigned char *key,
             unsigned char *iv) {
  std::ifstream file(filename, std::ios::binary);

  if (!file) {
    throw std::runtime_error(
        fmt::format("Could not open key file '{}'.", filename));
  }

  file.seekg(0, std::ios::end);
  std::streamoff filesize = file.tellg();

  if (filesize != 158) {
    throw std::runtime_error(fmt::format(
        "Expected key filesize of 158 bytes does not match (was: {:L}).",
        filesize));
  }

  file.seekg(110, std::ios::beg);
  file.read(reinterpret_cast<char *>(iv), 16);
  file.read(reinterpret_cast<char *>(key), 32);
}

void validateOutput(std::istream &input) {
  const char expectedBytes[] = "SQLite format 3";
  char read[sizeof(expectedBytes)];

  input.read(read, sizeof(expectedBytes));

  if (input.gcount() != sizeof(expectedBytes)) {
    throw std::runtime_error("Couldn't validate output file");
  }
  if (std::memcmp(read, expectedBytes, sizeof(expectedBytes)) != 0) {
    throw std::runtime_error(
        "Validation failed. Invalid key file or account name?");
  }
}

} // namespace WhatsApp::Crypt
