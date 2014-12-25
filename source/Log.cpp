#include "Log.h"
#include "Libraries/AES/AES.h"
#include "Exceptions/Exception.h"

Log::Log()
{
	output.open("WhatsApp Viewer.log");

	if (!output)
	{
		throw Exception("Could not open log file");
	}
}

Log::~Log()
{
	output.close();
}

Log& Log::operator <<(std::ios_base& (*value)(std::ios_base&))
{
	output << value;
	return *this;
}

Log& Log::operator<< (std::ios& (*value)(std::ios&))
{
	output << value;
	return *this;
}
Log& Log::operator<< (std::ostream& (*value)(std::ostream&))
{
	output << value;
	return *this;
}
Log& Log::operator<< (std::streambuf* value)
{
	output << value;
	return *this;
}

void Log::logHexBuffer(const char *buffer, int length)
{
	int blocks = (length + aesBlocksize - 1) / aesBlocksize;

	for (int block = 0; block < blocks; block++)
	{
		for (int byte = 0; byte < aesBlocksize; byte++)
		{
			*this << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << ((int)buffer[block * aesBlocksize + byte] & 0xFF) << " ";
		}

		*this << std::endl;
	}
}

void Log::logHexBuffer(const unsigned char *buffer, int length)
{
	int blocks = (length + aesBlocksize - 1) / aesBlocksize;

	for (int block = 0; block < blocks; block++)
	{
		for (int byte = 0; byte < aesBlocksize; byte++)
		{
			*this << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << ((int)buffer[block * aesBlocksize + byte] & 0xFF) << " ";
		}

		*this << std::endl;
	}
}
