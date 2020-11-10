#include <cstring>
#include <iostream>
#include <zlib.h>

#include "gzip.h"

namespace Compression {

const int chunk = 16 * 1024;

int unzipBlock(z_stream& stream, std::ostream &uncompressed) {
	unsigned char out[chunk];
	int ret = 0;

	do {
		stream.avail_out = chunk;
		stream.next_out = out;
		ret = inflate(&stream, Z_NO_FLUSH);

		if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
			inflateEnd(&stream);
			throw std::runtime_error("Decryption failed. Error during unzipping (inflate). In most cases this means your key does not match the database file.");
		}

		unsigned int have = chunk - stream.avail_out;
		uncompressed.write(reinterpret_cast<char*>(out), have);
	} while (stream.avail_out == 0);

	return ret;
}

void unzipBuffer(std::istream &compressed, std::ostream &uncompressed) {
	unsigned char in[chunk];

	z_stream stream;
	std::memset(&stream, 0, sizeof(z_stream));

	int ret = inflateInit2(&stream, 15 | 32);
	if (ret != Z_OK) {
		throw std::runtime_error("Decryption failed. Error during unzipping (inflateInit). Invalid key?");
	}

	do {
		if (!compressed || compressed.eof()) {
			throw std::runtime_error("Couldn't read compressed file (unexpected EOF).");
		}

		compressed.read(reinterpret_cast<char*>(in), chunk);
		std::streamsize bytesRead = compressed.gcount();
		stream.avail_in = bytesRead;
		stream.next_in = in;

		ret = unzipBlock(stream, uncompressed);
	} while (ret != Z_STREAM_END);

	inflateEnd(&stream);
}

}
