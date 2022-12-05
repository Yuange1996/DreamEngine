#pragma once

#include "EngineMinimal.h"
#include "EngineFactory.h"

int Init(CEngine* InEngine, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
#if defined(_WIN32)
	FWinMainCommandParameters WinaMainParamters = FWinMainCommandParameters(hInstance,hPrevInstance,lpCmdLine,nShowCmd);
#endif
	int ReturnValue = InEngine->PreInit(
#if defined(_WIN32)
	WinaMainParamters
#endif //1
	);

	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine pre initialization error",ReturnValue)
		return ReturnValue;
	}
	ReturnValue = InEngine->Init(
#if defined(_WIN32)
	WinaMainParamters
#endif
	);

	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine initialization error",ReturnValue)
		return ReturnValue;
	}

	ReturnValue = InEngine->PostInit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine post initialization error",ReturnValue)
		return ReturnValue;
	}
	return ReturnValue;
};

void Tick(CEngine* InEngine)
{
	float DeltaTime = 0.03f;
	InEngine->Tick(DeltaTime);
	//Sleep(30);
};

int Exit(CEngine* InEngine)
{
	int	ReturnValue = InEngine->PreExit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine pre exit error",ReturnValue)
		return ReturnValue;
	}
	ReturnValue = InEngine->Exit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine exit error",ReturnValue)
		return ReturnValue;
	}
	ReturnValue = InEngine->PostExit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine post exit error",ReturnValue)
		return ReturnValue;
	}
	return ReturnValue;
}

CEngine* Engine = NULL;

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
	int ReturnValue = 0;

	Engine = FEngineFactory::CreateEngine();
	if (Engine)
	{
		//初始化
		ReturnValue = Init(Engine,hInstance,hPrevInstance,lpCmdLine,nShowCmd);

		MSG EngineMsg = { 0 };

		//渲染
		while (EngineMsg.message != WM_QUIT)
		{
			//PM_NOREMOVE 消息不从队列里除掉。
			//PM_REMOVE   消息从队列里除掉。
			//PM_NOYIELD  此标志使系统不释放等待调用程序空闲的线程
			// 
			//PM_QS_INPUT 处理鼠标和键盘消息。
			//PM_QS_PAINT 处理画图消息。
			//PM_QS_POSTMESSAGE 处理所有被寄送的消息，包括计时器和热键。
			//PM_QS_SENDMESSAGE 处理所有发送消息。
			if (PeekMessage(&EngineMsg,0,0,0, PM_REMOVE))
			{
				TranslateMessage(&EngineMsg);
				DispatchMessage(&EngineMsg);
			}
			else
			{
				Tick(Engine);
			}
		}
		//退出
		ReturnValue = Exit(Engine);
	}
	else
	{
		ReturnValue = 1;
	}
	Engine_Log("[%i]The Engine Exit",ReturnValue);
	return ReturnValue;
}