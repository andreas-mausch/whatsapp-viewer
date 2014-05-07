#pragma once

class ImageDecoder;
class Bitmap;

class SearchControl
{
private:
	Bitmap *searchIcon;
	Bitmap *clearIcon;

	HWND window;
	HWND editControl;

	void onCreate();
	void onPaint();

	void show(bool visible);
	void resizeChildWindows();

	static LRESULT CALLBACK callback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

public:
	SearchControl(HWND window);
	~SearchControl();

	static void registerControl();

};
