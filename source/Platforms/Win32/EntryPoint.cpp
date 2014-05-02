#include <windows.h>

#include "../../Main.h"
#include "../../VectorUtils.h"
#include "StringHelper.h"

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int commandShow)
{
	int argumentsCount;
	WCHAR **argumentsW = CommandLineToArgvW(GetCommandLineW(), &argumentsCount);
	std::vector<std::string *> arguments;
	for (int i = 0; i < argumentsCount; i++)
	{
		std::string *argument = new std::string(wstrtostr(argumentsW[i]));
		arguments.push_back(argument);
	}
	LocalFree(argumentsW);

	entryPoint(arguments);
	clearVector(arguments);
}
