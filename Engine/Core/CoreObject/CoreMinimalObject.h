#pragma once

#include "../../EngineMinimal.h"
#include "../../Platform/Windows/WindowsPlatform.h"
#include "../../CodeReflection/CodeReflectionMacroTag.h"

class CCoreMinimalObject
{
public:
	CCoreMinimalObject();
	virtual ~CCoreMinimalObject();

	virtual void BeginInit(){};
	virtual void Tick(float DeltaTime){};

	template<class T>
	T* CreateObject(CCoreMinimalObject* NewObject)
	{
		return dynamic_cast<T*>(NewObject);
	}

	bool operator == (const CCoreMinimalObject & InOther)
	{
		return guid_equal(&Guid,&InOther.Guid);
	}

private:
	simple_c_guid Guid;
};

extern vector<CCoreMinimalObject*> GObjects;