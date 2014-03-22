#include <string>
#include <vector>
#include <windows.h>
#include <wincodec.h>

#include "../../Exceptions/Exception.h"
#include "StringHelper.h"
#include "JpegDecoder.h"

JpegDecoder::JpegDecoder()
{
	if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void **>(&factory))))
	{
		throw Exception("could not get factory");
	}
}

JpegDecoder::~JpegDecoder()
{
	factory->Release();
}

HBITMAP JpegDecoder::loadImage(const std::string &filename)
{
	WCHAR *wcharFilename = buildWcharString(filename);
	IWICBitmapDecoder *decoder = NULL;
	HRESULT result = factory->CreateDecoderFromFilename(wcharFilename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);

	delete[] wcharFilename;

	if (FAILED(result))
	{
		throw Exception("could not get decoder");
	}

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
	decoder->Release();

	HBITMAP bitmap = CreateBitmap(width, height, 1, 32, &buffer[0]);
	return bitmap;
}
