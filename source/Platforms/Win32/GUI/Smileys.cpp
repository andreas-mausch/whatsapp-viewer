#include <windows.h>
#include <vector>

#include "Smileys.h"
#include "Smiley.h"
#include "../../../Exceptions/Exception.h"
#include "../../../../resources/resource.h"

Smileys::Smileys(JpegDecoder &jpegDecoder) : jpegDecoder(jpegDecoder)
{
	load(0x0E418, MAKEINTRESOURCE(IDB_0E418));
	load(0x1F61F, MAKEINTRESOURCE(IDB_1F61F));
}

Smileys::~Smileys()
{
	for (std::vector<Smiley *>::iterator it = smileys.begin(); it != smileys.end(); it++)
	{
		delete *it;
	}
}

void Smileys::load(int character, const WCHAR *name)
{
	HBITMAP bitmap = jpegDecoder.loadImageFromResource(name, L"PNG");
	Smiley *smiley = new Smiley(character, bitmap);
	smileys.push_back(smiley);
}

void Smileys::renderSmiley(int character, HDC deviceContext, int x, int y)
{
	Smiley &smiley = find(character);
	smiley.render(deviceContext, x, y);
}

Smiley &Smileys::find(int character)
{
	for (std::vector<Smiley *>::iterator it = smileys.begin(); it != smileys.end(); it++)
	{
		Smiley &smiley = **it;
		if (smiley.getCharacter() == character)
		{
			return smiley;
		}
	}

	return *smileys[0];
	// throw Exception("could not find smiley");
}
