#pragma once

class Bitmap;

enum ButtonState
{
	BUTTON_NORMAL,
	BUTTON_OVER
};

class HoverButton
{
private:
	HWND window;

	Bitmap &normal;
	Bitmap &hover;
	ButtonState state;
	WNDPROC buttonProc;
	bool mouseTracking;

	void onMouseMove(POINT &point);
	void onMouseEnter();
	void onMouseLeave();

	static LRESULT CALLBACK callback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

public:
	HoverButton(HWND window, Bitmap &normal, Bitmap &hover);
	~HoverButton();

	void onPaint(HDC deviceContext);
	HWND getHandle();
	void show(bool visible);

};
