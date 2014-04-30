#include <windows.h>

#include "Brush.h"

Brush::Brush(HBRUSH brush)
	: Handle(brush)
{
}

Brush::~Brush()
{
}

HBRUSH Brush::get()
{
	return static_cast<HBRUSH>(handle);
}
