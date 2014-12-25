#include <string>
#include <vector>
#include <windows.h>
#include <wincodec.h>

#include "../../Exceptions/Exception.h"
#include "ImageDecoder.h"
#include "Resource.h"
#include "StringHelper.h"
#include "GUI/AnimatedGif.h"

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

std::string ImageDecoder::loadString(const WCHAR *name, const WCHAR *type)
{
	unsigned char *bytes = NULL;
	DWORD size = 0;
	loadResource(name, type, bytes, size);

	std::string text(reinterpret_cast<const char *>(bytes), size);
	return text;
}

HBITMAP ImageDecoder::loadImageFromResource(const WCHAR *name, const WCHAR *type)
{
	unsigned char *bytes = NULL;
	DWORD size = 0;
	loadResource(name, type, bytes, size);

	return loadImage(bytes, size);
}

IWICStream *ImageDecoder::createStreamFromBytes(unsigned char *bytes, int size)
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

	return stream;
}

IWICBitmapDecoder *ImageDecoder::createDecoderFromStream(IWICStream *stream)
{
	IWICBitmapDecoder *decoder = NULL;
	if (FAILED(factory->CreateDecoderFromStream(stream, NULL, WICDecodeMetadataCacheOnLoad, &decoder)))
	{
		throw Exception("loadImage() failed: could not create decoder");
	}

	return decoder;
}

HBITMAP ImageDecoder::loadImage(unsigned char *bytes, int size)
{
	IWICStream *stream = createStreamFromBytes(bytes, size);
	IWICBitmapDecoder *decoder = createDecoderFromStream(stream);

	HBITMAP bitmap = loadImage(decoder);

	decoder->Release();
	stream->Release();

	return bitmap;
}

HBITMAP ImageDecoder::convertFrameToBitmap(IWICBitmapFrameDecode *frame)
{
	IWICBitmapSource *bitmapSource = frame;
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
	if (FAILED(bitmapSource->CopyPixels(NULL, width * 4, buffer.size(), &buffer[0])))
	{
		throw Exception("could not get image size");
	}

	bitmapSource->Release();

	return CreateBitmap(width, height, 1, 32, &buffer[0]);
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

	HBITMAP bitmap = convertFrameToBitmap(frame);
	frame->Release();

	return bitmap;
}

void ImageDecoder::loadGifFromResource(const WCHAR *name, const WCHAR *type, std::vector<AnimatedGifFrame *> &frames)
{
	unsigned char *bytes = NULL;
	DWORD size = 0;
	loadResource(name, type, bytes, size);

	IWICStream *stream = createStreamFromBytes(bytes, size);
	IWICBitmapDecoder *decoder = createDecoderFromStream(stream);

	unsigned int frameCount = 0;
	if (FAILED(decoder->GetFrameCount(&frameCount)))
	{
		throw Exception("could not get gif frame count");
	}

	if (frameCount <= 1)
	{
		throw Exception("not a gif animation");
	}

	for (unsigned int frameIndex = 0; frameIndex < frameCount; frameIndex++)
	{
		IWICBitmapFrameDecode *bitmapFrame = NULL;
		if (FAILED(decoder->GetFrame(frameIndex, &bitmapFrame)))
		{
			throw Exception("could not get frame");
		}

		HBITMAP bitmap = convertFrameToBitmap(bitmapFrame);
		bitmapFrame->Release();

		AnimatedGifFrame *frame = new AnimatedGifFrame(bitmap);
		frames.push_back(frame);

	}

	decoder->Release();
	stream->Release();
}
