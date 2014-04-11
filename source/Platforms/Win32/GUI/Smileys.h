#pragma once

class Smiley;

#include <vector>
#include "../ImageDecoder.h"

class Smileys
{
private:
	std::vector<Smiley *> smileys;
	ImageDecoder &imageDecoder;

	Smiley &find(int character);
	void load(int character, const WCHAR *name);

public:
	Smileys(ImageDecoder &imageDecoder);
	~Smileys();

	void renderSmiley(int character, HDC deviceContext, int x, int y);
};
