#pragma once
#include "../../EngineMinimal.h"

enum EPressstate
{
	PRESS,//╟╢об
	RELEASE,//киоб
};

struct FInputKey
{
	FInputKey();
	string KeyName;
	EPressstate Presssate;
};