// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

SHOOTERX_API DECLARE_LOG_CATEGORY_EXTERN(LogSX, Log, All);
SHOOTERX_API DECLARE_LOG_CATEGORY_EXTERN(LogSXNet, Log, All);

#define SXCheckF(Expression, Format, ...) \
{ \
	if ((Expression) == false) \
	{ \
		UE_LOG(LogSX, Error, TEXT("[%s(%d)] " Format), *FString(__FUNCTION__), __LINE__, ##__VA_ARGS__); \
		check(Expression); \
	} \
}

#define NETMODE_TCHAR ((GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("Client%02d"), UE::GetPlayInEditorID()) : ((GetNetMode() == ENetMode::NM_Standalone) ? TEXT("StandAlone") : TEXT("Server")))
#define FUNCTION_TCHAR (ANSI_TO_TCHAR(__FUNCTION__))
#define SX_LOG_NET(LogCategory, Verbosity, Format, ...) UE_LOG(LogCategory, Verbosity, TEXT("[%s] %s %s"), NETMODE_TCHAR, FUNCTION_TCHAR, *FString::Printf(Format, ##__VA_ARGS__))

extern TAutoConsoleVariable<bool> CVarDebugGeneral;
