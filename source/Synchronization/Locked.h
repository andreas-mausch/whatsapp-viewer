#pragma once

class Lock;

class Locked
{
private:
	Lock &lock;
public:
	Locked(Lock &lock);
	~Locked();
};
