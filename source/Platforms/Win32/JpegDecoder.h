#pragma once

class IWICImagingFactory;

class JpegDecoder
{
private:
	IWICImagingFactory *factory;

public:
	JpegDecoder();
	~JpegDecoder();

	HBITMAP loadImage(const std::string &filename);
};
