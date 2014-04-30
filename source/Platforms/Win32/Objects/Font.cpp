#include <windows.h>

#include "Font.h"

Font::Font(HFONT font)
	: Handle(font)
{
}

Font::~Font()
{
}

HFONT Font::get()
{
	return static_cast<HFONT>(handle);
}
