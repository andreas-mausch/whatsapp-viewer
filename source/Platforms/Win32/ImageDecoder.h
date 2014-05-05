#pragma once

#include <vector>

struct IWICStream;
struct IWICImagingFactory;
struct IWICBitmapDecoder;
struct IWICBitmapFrameDecode;
struct AnimatedGifFrame;

class ImageDecoder
{
private:
	IWICImagingFactory *factory;

	void loadResource(const WCHAR *name, const WCHAR *type, unsigned char *&bytes, DWORD &size);
	HBITMAP loadImage(IWICBitmapDecoder *decoder);

	IWICStream *createStreamFromBytes(unsigned char *bytes, int size);
	IWICBitmapDecoder *createDecoderFromStream(IWICStream *stream);
	HBITMAP convertFrameToBitmap(IWICBitmapFrameDecode *frame);

public:
	ImageDecoder();
	~ImageDecoder();

	HBITMAP loadImage(const std::string &filename);
	HBITMAP loadImageFromResource(const WCHAR *name, const WCHAR *type);
	HBITMAP loadImage(unsigned char *bytes, int size);
	void loadGifFromResource(const WCHAR *name, const WCHAR *type, std::vector<AnimatedGifFrame *> &frames);

};
