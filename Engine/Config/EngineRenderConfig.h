#pragma once

struct FEngineRenderConfig
{
	FEngineRenderConfig();

	int ScrrenWidth;
	int ScrrenHight;
	int RefreshRate;
	int SwapChainCount;

	static FEngineRenderConfig* GetRenderConfig();
	static void Destroy();
private:
	static FEngineRenderConfig* RenderConfig;
};