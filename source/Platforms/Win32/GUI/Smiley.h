#pragma once

class Smiley
{
private:
	int character;
	HBITMAP bitmap;

public:
	Smiley(int character, HBITMAP bitmap);
	~Smiley();

	void render(HDC deviceContext, int x, int y);
	int getCharacter() const;
};
