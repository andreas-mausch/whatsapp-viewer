#pragma once

#include "../Handle.h"

class Font : public Handle
{
public:
	Font(HFONT font);
	~Font();

	HFONT get();
};
