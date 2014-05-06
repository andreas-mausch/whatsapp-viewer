#include <windows.h>

#include "LoadingAnimationThread.h"
#include "../../AnimatedGif.h"

LoadingAnimationThread::LoadingAnimationThread(HWND window, AnimatedGif &loadingAnimation)
	: window(window), loadingAnimation(loadingAnimation)
{
}

LoadingAnimationThread::~LoadingAnimationThread()
{
}

void LoadingAnimationThread::run()
{
	int frameCount = loadingAnimation.getFrameCount();

	while (running)
	{
		for (int frameIndex = 0; frameIndex < frameCount; frameIndex++)
		{
			if (!running)
			{
				break;
			}

			RECT clientRect;
			GetClientRect(window, &clientRect);

			int x = (clientRect.right - 128) / 2;
			int y = (clientRect.bottom - 128) / 2;

			HDC deviceContext = GetDC(window);
			loadingAnimation.renderFrame(deviceContext, frameIndex, x, y);
			ReleaseDC(window, deviceContext);

			Sleep(120);
		}
	}
}
