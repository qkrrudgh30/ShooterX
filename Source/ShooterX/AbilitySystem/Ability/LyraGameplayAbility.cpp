// LyraGameplayAbility.cpp

#include "LyraGameplayAbility.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraGameplayAbility)

ULyraGameplayAbility::ULyraGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ActivationPolicy = ELyraAbilityActivationPolicy::OnInputTriggered;
}
