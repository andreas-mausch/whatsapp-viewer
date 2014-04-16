#pragma once

class Counter
{
private:
	long long frequency;
	long long startTime;

public:
	Counter(void);
	~Counter(void);

	double getElapsedTime();
	void showElapsedTime(const std::string &text);

};
