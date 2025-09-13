// LyraRangedWeaponInstance.h

#pragma once

#include "LyraWeaponInstance.h"
#include "LyraRangedWeaponInstance.generated.h"

UCLASS()
class SHOOTERX_API ULyraRangedWeaponInstance : public ULyraWeaponInstance
{
	GENERATED_BODY()

public:
	ULyraRangedWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
