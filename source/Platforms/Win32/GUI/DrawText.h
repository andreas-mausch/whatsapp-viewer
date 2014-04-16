#pragma once

void calculateDrawTextRect(HDC deviceContext, const WCHAR *text, RECT &rect);
int calculateDrawTextHeight(HDC deviceContext, const WCHAR *text, int width, HFONT font);
void drawText(HDC deviceContext, const WCHAR *text, const RECT &rect);
void drawText(HDC deviceContext, const WCHAR *text, int x, int y, int width);
void drawText(HDC deviceContext, const WCHAR *text, int x, int y, int width, HFONT font);
