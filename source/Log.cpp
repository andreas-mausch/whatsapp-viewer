#include "Log.h"

Log::Log()
{
	output.open("WhatsApp Reader.log");
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
