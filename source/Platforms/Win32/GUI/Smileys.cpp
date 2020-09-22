#include <windows.h>
#include <vector>

#include "Smileys.h"
#include "Smiley.h"
#include "SmileyList.h"
#include "../../../Exceptions/Exception.h"
#include "../../../VectorUtils.h"
#include "../../../../resources/resource.h"

Smileys::Smileys(ImageDecoder &imageDecoder) : imageDecoder(imageDecoder)
{
	for (auto const& smiley : characterToResource)
	{
		load(smiley.first, MAKEINTRESOURCE(smiley.second));
	}
}

Smileys::~Smileys()
{
	clearVector(smileys);
}

void Smileys::load(int character, const WCHAR *name)
{
	HBITMAP bitmap = imageDecoder.loadImageFromResource(name, L"PNG");
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

	throw Exception("could not find smiley");
}
