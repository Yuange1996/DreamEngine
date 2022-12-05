#pragma once

#if defined(_WIN32)
#include "../EngineMinimal.h"

class FWinMainCommandParameters
{
public:
	FWinMainCommandParameters(HINSTANCE InInstance,HINSTANCE InPrevInstance,LPSTR InCmdLine,int InShowCmd);

	HINSTANCE Instance;
	HINSTANCE PrevInstance;
	LPSTR CmdLine;
	int	ShowCmd;
};
#elif 1

#endif //0
