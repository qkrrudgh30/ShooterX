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

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	// 오너 액터와 아바타 액터가 정해질 때마다 내부적으로 호출되는 함수.

public:
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	// Ability Input을 처리할 Pending Queue
	
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
	
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	
};
