#pragma once

class Icon
{
private:
	HICON handle;
public:
	Icon(const TCHAR *iconName, int width, int height);
	~Icon();

	HICON get();
};
