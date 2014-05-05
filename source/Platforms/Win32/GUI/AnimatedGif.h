#pragma once

#include <vector>
#include "../Objects/Bitmap.h"

class ImageDecoder;

struct AnimatedGifFrame
{
	AnimatedGifFrame(HBITMAP bitmap) : bitmap(bitmap) { }

	Bitmap bitmap;
	int delay;
	int disposal;
	int frameSize;
	int frameOffset;
};

class AnimatedGif
{
private:
	std::vector<AnimatedGifFrame *> frames;
public:
	AnimatedGif(const WCHAR *name, const WCHAR *type, ImageDecoder &imageDecoder);
	~AnimatedGif();

	int getFrameCount();
	void renderFrame(HDC deviceContext, int index, int x, int y);
	int getDelay(int frameIndex);
};
