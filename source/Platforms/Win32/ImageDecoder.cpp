#include <string>
#include <vector>
#include <windows.h>
#include <wincodec.h>

#include "../../Exceptions/Exception.h"
#include "StringHelper.h"
#include "ImageDecoder.h"

ImageDecoder::ImageDecoder()
{
	if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void **>(&factory))))
	{
		throw Exception("could not get factory");
	}
}

ImageDecoder::~ImageDecoder()
{
	factory->Release();
}

HBITMAP ImageDecoder::loadImage(const std::string &filename)
{
	WCHAR *wcharFilename = buildWcharString(filename);
	IWICBitmapDecoder *decoder = NULL;
	HRESULT result = factory->CreateDecoderFromFilename(wcharFilename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);

	delete[] wcharFilename;

	if (FAILED(result))
	{
		throw Exception("could not create decoder");
	}

	HBITMAP bitmap = loadImage(decoder);

	decoder->Release();

	return bitmap;
}

HBITMAP ImageDecoder::loadImageFromResource(const WCHAR *name, const WCHAR *type)
{
	HRSRC resource = FindResource(NULL, name, type);

	if (resource == NULL)
	{
		throw Exception("could not find resource");
	}

	HGLOBAL resourceData = LoadResource(NULL, resource);

	if (resourceData == NULL)
	{
		throw Exception("could not load resource");
	}

	unsigned char *bytes = reinterpret_cast<unsigned char *>(LockResource(resourceData));

	if (bytes == NULL)
	{
		throw Exception("could not lock resource");
	}

	DWORD size = SizeofResource(NULL, resource);

	if (size == NULL)
	{
		throw Exception("could not get size of resource");
	}

	HBITMAP bitmap = loadImage(bytes, size);

	return bitmap;
}

HBITMAP ImageDecoder::loadImage(unsigned char *bytes, int size)
{
	IWICStream *stream = NULL;
	if (FAILED(factory->CreateStream(&stream)))
	{
		throw Exception("could not create stream");
	}

	if (FAILED(stream->InitializeFromMemory(bytes, size)))
	{
		throw Exception("could not initialize stream");
	}

	IWICBitmapDecoder *decoder = NULL;
	if (FAILED(factory->CreateDecoderFromStream(stream, NULL, WICDecodeMetadataCacheOnLoad, &decoder)))
	{
		throw Exception("loadImage() failed: could not create decoder");
	}

	HBITMAP bitmap = loadImage(decoder);

	decoder->Release();
	stream->Release();

	return bitmap;
}

HBITMAP ImageDecoder::loadImage(IWICBitmapDecoder *decoder)
{
	IWICBitmapFrameDecode *frame = NULL;
	if (FAILED(decoder->GetFrame(0, &frame)))
	{
		throw Exception("could not get frame");
	}

	IWICBitmapSource *bitmapSource = NULL;
	if (FAILED(WICConvertBitmapSource(GUID_WICPixelFormat32bppPBGRA, frame, &bitmapSource)))
	{
		throw Exception("could not convert bitmap");
	}

	UINT width = 0;
    UINT height = 0;
    if (FAILED(bitmapSource->GetSize(&width, &height)) || width == 0 || height == 0)
	{
		throw Exception("could not get image size");
	}

	std::vector<BYTE> buffer(width * height * 4);
    if (FAILED(bitmapSource->CopyPixels(0, width * 4, buffer.size(), &buffer[0])))
	{
		throw Exception("could not get image size");
	}

	bitmapSource->Release();
	frame->Release();

	HBITMAP bitmap = CreateBitmap(width, height, 1, 32, &buffer[0]);
	return bitmap;
}
