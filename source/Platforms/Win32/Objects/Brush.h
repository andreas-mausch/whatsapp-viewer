#pragma once

#include "../Handle.h"

class Brush : public Handle
{
public:
	Brush(HBRUSH brush);
	~Brush();

	HBRUSH get();
};
