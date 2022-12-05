#include "EngineRenderConfig.h"

FEngineRenderConfig* FEngineRenderConfig::RenderConfig = nullptr;

FEngineRenderConfig::FEngineRenderConfig()
	:ScrrenWidth(1280)
	,ScrrenHight(720)
	,RefreshRate(60)
	,SwapChainCount(2)
{

}

void FEngineRenderConfig::Destroy()
{
	if (RenderConfig)
	{
		RenderConfig = nullptr;
		delete RenderConfig;
	}
}

FEngineRenderConfig* FEngineRenderConfig::GetRenderConfig()
{
	if (!RenderConfig)
	{
		RenderConfig = new FEngineRenderConfig();
	}
	return RenderConfig;
}

