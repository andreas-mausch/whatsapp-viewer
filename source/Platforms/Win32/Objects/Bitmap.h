#pragma once

#include "../Handle.h"

class Bitmap : public Handle
{
public:
	Bitmap(HBITMAP bitmap);
	~Bitmap();

	HBITMAP get();
};
