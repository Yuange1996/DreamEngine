#include "WinMainCommandParameters.h"

#if defined(_WIN32)
FWinMainCommandParameters::FWinMainCommandParameters(HINSTANCE InInstance, HINSTANCE InPrevInstance, LPSTR InCmdLine, int InShowCmd)
:Instance(InInstance)
,PrevInstance(InPrevInstance)
,CmdLine(InCmdLine)
,ShowCmd(InShowCmd)
{

}
#endif