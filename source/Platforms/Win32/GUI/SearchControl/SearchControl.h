#pragma once

class ImageDecoder;
class Bitmap;

LRESULT CALLBACK clearButtonCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

class SearchControl
{
private:
	Bitmap *searchIcon;
	Bitmap *clearIcon;
	Bitmap *clearIconHover;

	HWND window;
	HWND editControl;
	HWND clearButton;

	void onCreate();
	void onPaint();
	void onPaintClearButton(HDC deviceContext);

	void show(bool visible);
	void resizeChildWindows();

	static LRESULT CALLBACK callback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

public:
	SearchControl(HWND window);
	~SearchControl();

	static void registerControl();

};
