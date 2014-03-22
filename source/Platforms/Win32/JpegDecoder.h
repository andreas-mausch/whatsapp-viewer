#pragma once

struct IWICImagingFactory;
struct IWICBitmapDecoder;

class JpegDecoder
{
private:
	IWICImagingFactory *factory;

	HBITMAP loadImage(IWICBitmapDecoder *decoder);

public:
	JpegDecoder();
	~JpegDecoder();

	HBITMAP loadImage(const std::string &filename);
	HBITMAP loadImage(unsigned char *bytes, int size);
};
