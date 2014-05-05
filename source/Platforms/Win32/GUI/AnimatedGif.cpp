#include <windows.h>

#include "AnimatedGif.h"
#include "../ImageDecoder.h"
#include "../../../VectorUtils.h"

void renderBitmap2(HDC deviceContext, HBITMAP bitmapHandle, int x, int y);

AnimatedGif::AnimatedGif(const WCHAR *name, const WCHAR *type, ImageDecoder &imageDecoder)
{
	imageDecoder.loadGifFromResource(name, type, frames);
}

AnimatedGif::~AnimatedGif()
{
	clearVector(frames);
}

int AnimatedGif::getFrameCount()
{
	return frames.size();
}

void AnimatedGif::renderFrame(HDC deviceContext, int index, int x, int y)
{
	renderBitmap2(deviceContext, frames[index]->bitmap.get(), x, y);
}

int AnimatedGif::getDelay(int frameIndex)
{
	return -1;
}
