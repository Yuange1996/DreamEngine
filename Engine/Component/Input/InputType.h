#pragma once
#include "../../EngineMinimal.h"

enum EPressstate
{
	PRESS,//����
	RELEASE,//����
};

struct FInputKey
{
	FInputKey();
	string KeyName;
	EPressstate Presssate;
};