#pragma once

class Handle
{
protected:
	HANDLE handle;
public:
	Handle(HANDLE handle);
	virtual ~Handle();
};
