// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterX.h"
#include "Modules/ModuleManager.h"

class FShooterXModule : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override
	{
	}

	virtual void ShutdownModule() override
	{
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, ShooterX, "ShooterX" );

DEFINE_LOG_CATEGORY(LogSX);
DEFINE_LOG_CATEGORY(LogSXNet);

TAutoConsoleVariable<bool> CVarDebugGeneral(TEXT("SX.Debug.General"), false, TEXT("일반적인 디버깅 정보를 띄우고자 할 때 사용합니다.\n"), ECVF_Cheat);

