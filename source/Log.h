#pragma once

#include <fstream>
#include <iostream>
#include <iomanip>

class Log
{
private:
	std::ofstream output;

public:
	Log();
	~Log();

	template<class T> Log& operator<<(const T& value)
	{
		output << value;
		return *this;
	}

	template<class Arg> Log& operator<<(std::_Smanip<Arg> Smanip)
	{
		(*Smanip._Pfun)(output, Smanip._Manarg);
		return *this;
	}

	template<class Arg> Log& operator<<(std::_Fillobj<Arg> Fillobj)
	{
		output.fill(Fillobj._Fill);
		return *this;
	}

	void logHexBuffer(const char *buffer, int length);
	void logHexBuffer(const unsigned char *buffer, int length);

	Log& operator<< (std::ios_base& (*value)(std::ios_base&));
	Log& operator<< (std::ios& (*value)(std::ios&));
	Log& operator<< (std::ostream& (*value)(std::ostream&));
	Log& operator<< (std::streambuf* value);

};
