// LyraGameplayAbility_RangedWeapon.h

#pragma once

#include "LyraGameplayAbility_FromEquipment.h"
#include "LyraGameplayAbility_RangedWeapon.generated.h"

UCLASS()
class SHOOTERX_API ULyraGameplayAbility_RangedWeapon : public ULyraGameplayAbility_FromEquipment
{
	GENERATED_BODY()

public:
	ULyraGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
