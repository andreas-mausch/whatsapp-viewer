#pragma once

#include "../ChatControlElement.h"

class Font;

class DayBreak : public ChatControlElement
{
private:
	std::wstring dayStringW;
	int width;
	int height;
	Font &font;

public:
	DayBreak(const std::string &dayString, int width, Font &font);
	virtual ~DayBreak();

	void updateWidth(HWND window, int width);
	void calculateHeight();

	int getHeight();

	void render(HDC deviceContext, int y, int clientHeight);

};
