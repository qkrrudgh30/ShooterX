// LyraGameplayAbility.h

#pragma once

#include "Abilities/GameplayAbility.h"
#include "LyraGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class ELyraAbilityActivationPolicy : uint8
{
	// Input이 Trigger 되었을 경우(Pressed/Released)
	OnInputTriggered,
	// Input이 Held 되었을 경우
	WhileInputActive,
	// Avatar가 생성되었을 경우, 바로 할당. ex) 패시브 스킬.
	OnSpawn
};

UCLASS(Abstract)
class SHOOTERX_API ULyraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	ULyraGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	// 언제 어빌리티가 활성화될 지에 대한 정책
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LC|AbilityActivation")
	ELyraAbilityActivationPolicy ActivationPolicy;
	
};
