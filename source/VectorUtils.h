#pragma once

template<class T>
void clearVector(std::vector<T*> &v)
{
	for(std::vector<T*>::iterator it = v.begin(); it != v.end(); ++it)
	{
		T* pointer = *it;
		delete pointer;
	}
	v.clear();
}
