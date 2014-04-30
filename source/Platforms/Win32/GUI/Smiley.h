#pragma once

class Bitmap;

#include "../Objects/Bitmap.h"

class Smiley
{
private:
	int character;
	Bitmap bitmap;

public:
	Smiley(int character, HBITMAP bitmap);
	~Smiley();

	void render(HDC deviceContext, int x, int y);
	int getCharacter() const;
};
