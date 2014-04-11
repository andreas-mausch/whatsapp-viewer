#pragma once

struct IWICImagingFactory;
struct IWICBitmapDecoder;

class ImageDecoder
{
private:
	IWICImagingFactory *factory;

	HBITMAP loadImage(IWICBitmapDecoder *decoder);

public:
	ImageDecoder();
	~ImageDecoder();

	HBITMAP loadImage(const std::string &filename);
	HBITMAP loadImageFromResource(const WCHAR *name, const WCHAR *type);
	HBITMAP loadImage(unsigned char *bytes, int size);
};
