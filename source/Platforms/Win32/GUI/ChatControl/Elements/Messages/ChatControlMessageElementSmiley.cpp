#include <string>
#include <windows.h>

#include "ChatControlMessageElementSmiley.h"
#include "../../../Smileys.h"
#include "../../../../../../Exceptions/Exception.h"

ChatControlMessageElementSmiley::ChatControlMessageElementSmiley(int smiley)
	: smiley(smiley)
{
}

ChatControlMessageElementSmiley::~ChatControlMessageElementSmiley()
{
}

void ChatControlMessageElementSmiley::calculateHeight(int width)
{
	height = 20;
}

void ChatControlMessageElementSmiley::render(HDC deviceContext, int x, int y, int width, Smileys &smileys)
{
	smileys.renderSmiley(smiley, deviceContext, x, y);
}
