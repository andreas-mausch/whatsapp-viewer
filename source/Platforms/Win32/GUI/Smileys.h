#pragma once

class Smiley;

#include <vector>
#include "../JpegDecoder.h"

class Smileys
{
private:
	std::vector<Smiley *> smileys;
	JpegDecoder jpegDecoder;

	Smiley &find(int character);
	void load(int character, const WCHAR *name);

public:
	Smileys(JpegDecoder &jpegDecoder);
	~Smileys();

	void renderSmiley(int character, HDC deviceContext, int x, int y);
};
