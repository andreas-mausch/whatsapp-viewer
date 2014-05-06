#pragma once

#include "../../../ThreadWindows.h"

class AnimatedGif;

class LoadingAnimationThread : public ThreadWindows
{
private:
	HWND window;
	AnimatedGif &loadingAnimation;

public:
	LoadingAnimationThread(HWND window, AnimatedGif &loadingAnimation);
	virtual ~LoadingAnimationThread();

	virtual void run();
};
