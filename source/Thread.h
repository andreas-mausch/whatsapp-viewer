#pragma once

class Thread
{
protected:
	volatile bool running;
	virtual void run() = 0;

public:
	Thread() { }
	virtual ~Thread() { }

	virtual void start() = 0;
	virtual void interrupt() = 0;
	virtual void join() = 0;
	virtual bool joinFor(int milliseconds) = 0;

};
