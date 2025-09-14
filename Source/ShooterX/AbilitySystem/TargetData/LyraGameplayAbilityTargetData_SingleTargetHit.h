// LyraGameplayAbilityTargetData_SingleTargetHit.h

#pragma once

#include "Abilities/GameplayAbilityTargetTypes.h"
#include "LyraGameplayAbilityTargetData_SingleTargetHit.generated.h"

USTRUCT()
struct SHOOTERX_API FLyraGameplayAbilityTargetData_SingleTargetHit : public FGameplayAbilityTargetData_SingleTargetHit
{
	GENERATED_BODY()

public:
	FLyraGameplayAbilityTargetData_SingleTargetHit()
		: CartridgeID(-1)
	{

	}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FLyraGameplayAbilityTargetData_SingleTargetHit::StaticStruct();
	}

	UPROPERTY()
	int32 CartridgeID;
	// 탄약 ID(카트리지)

};
