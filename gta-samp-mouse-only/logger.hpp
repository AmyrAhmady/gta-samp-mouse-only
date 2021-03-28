#pragma once
#include <sstream>

#define LOG Log().Get()

class Log
{
public:
	static void Init();

	Log();
	virtual ~Log();
	std::ostringstream & Get();

protected:
	std::ostringstream os;
private:
	Log(const Log &);
};