// LyraAnimInstance.h

#pragma once

#include "Animation/AnimInstance.h"
#include "GameplayEffectTypes.h"
#include "LyraAnimInstance.generated.h"

UCLASS()
class ULyraAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	ULyraAnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void NativeInitializeAnimation() override;

	void InitializeWithAbilitySystem(UAbilitySystemComponent* ASC);

	// 해당 속성값은 라이라 프로젝트의 애니메이션 블루프린트에 사용되는 값이므로 정의 필수.
	UPROPERTY(BlueprintReadOnly, Category="Character State Data")
	float GroundDistance = -1.f;

	// GameplayTag와 AnimInstance의 속성 값을 매핑해줌.
	UPROPERTY(EditDefaultsOnly, Category="GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;

};