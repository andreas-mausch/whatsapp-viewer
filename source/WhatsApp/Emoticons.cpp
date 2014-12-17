#include "../Platforms/Win32/GUI/SmileyList.h"

bool isSmiley(int character)
{
	for (int i = 0; i < smileyCount; i++)
	{
		if (smileyList[i].character == character)
		{
			return true;
		}
	}

	return false;
}
