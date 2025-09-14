// LyraGameplayAbility.h

#pragma once

#include "Abilities/GameplayAbility.h"
#include "LyraGameplayAbility.generated.h"

UCLASS(Abstract)
class SHOOTERX_API ULyraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	ULyraGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
