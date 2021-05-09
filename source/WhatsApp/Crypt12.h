#pragma once

void decryptWhatsappDatabase12_14(const std::string& filename,
								  const std::string& filenameDecrypted,
								  unsigned char* key,
								  std::streamoff initVectorOffset,
								  std::streamoff dataOffset,
								  std::streamoff footerSize);

void decryptWhatsappDatabase12(const std::string &filename, const std::string &filenameDecrypted, const std::string &keyFilename);
