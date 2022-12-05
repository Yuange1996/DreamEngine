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
		//��ʼ��
		ReturnValue = Init(Engine,hInstance,hPrevInstance,lpCmdLine,nShowCmd);

		MSG EngineMsg = { 0 };

		//��Ⱦ
		while (EngineMsg.message != WM_QUIT)
		{
			//PM_NOREMOVE ��Ϣ���Ӷ����������
			//PM_REMOVE   ��Ϣ�Ӷ����������
			//PM_NOYIELD  �˱�־ʹϵͳ���ͷŵȴ����ó�����е��߳�
			// 
			//PM_QS_INPUT �������ͼ�����Ϣ��
			//PM_QS_PAINT ����ͼ��Ϣ��
			//PM_QS_POSTMESSAGE �������б����͵���Ϣ��������ʱ�����ȼ���
			//PM_QS_SENDMESSAGE �������з�����Ϣ��
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
		//�˳�
		ReturnValue = Exit(Engine);
	}
	else
	{
		ReturnValue = 1;
	}
	Engine_Log("[%i]The Engine Exit",ReturnValue);
	return ReturnValue;
}