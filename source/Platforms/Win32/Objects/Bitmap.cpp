#include <windows.h>

#include "Bitmap.h"

Bitmap::Bitmap(HBITMAP bitmap)
	: Handle(bitmap)
{
}

Bitmap::~Bitmap()
{
}

HBITMAP Bitmap::get()
{
	return static_cast<HBITMAP>(handle);
}
