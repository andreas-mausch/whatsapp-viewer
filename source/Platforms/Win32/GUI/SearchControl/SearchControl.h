#pragma once

class Bitmap;
class ImageDecoder;
class HoverButton;

LRESULT CALLBACK clearButtonCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

class SearchControl
{
private:
	Bitmap *searchIcon;
	Bitmap *clearIcon;
	Bitmap *clearIconHover;

	HWND window;
	HWND editControl;
	HoverButton *clearButton;

	void onCreate();
	void onPaint();

	void showClearButton(const WCHAR *textW);
	void resizeChildWindows();
	void onTextChange();
	void onClear();

	static LRESULT CALLBACK callback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

public:
	SearchControl(HWND window);
	~SearchControl();

	static void registerControl();

};
