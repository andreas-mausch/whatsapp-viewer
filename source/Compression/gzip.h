#pragma once

#include <string>

namespace Compression {

void unzipBuffer(std::istream &compressed, std::ostream &uncompressed);

};
