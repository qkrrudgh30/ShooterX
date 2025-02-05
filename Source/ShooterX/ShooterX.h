// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

SHOOTERX_API DECLARE_LOG_CATEGORY_EXTERN(LogSX, Log, All);

#define SXCheckF(Expression, Format, ...) \
{ \
	if ((Expression) == false) \
	{ \
		UE_LOG(LogSX, Error, TEXT("[%s(%d)] " Format), *FString(__FUNCTION__), __LINE__, ##__VA_ARGS__); \
		check(Expression); \
	} \
}

extern TAutoConsoleVariable<bool> CVarDebugGeneral;
