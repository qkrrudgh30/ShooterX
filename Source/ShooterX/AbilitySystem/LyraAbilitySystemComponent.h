// LyraAbilitySystemComponent.h

#pragma once

#include "AbilitySystemComponent.h"
#include "LyraAbilitySystemComponent.generated.h"

UCLASS()
class SHOOTERX_API ULyraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	ULyraAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);

public:
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	// Ability Input을 처리할 Pending Queue
	
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
	
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	
};
