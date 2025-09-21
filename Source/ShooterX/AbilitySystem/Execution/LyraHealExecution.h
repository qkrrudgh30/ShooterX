// LyraHealExecution.h

#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "LyraHealExecution.generated.h"

// ULyraHealExecution은 GameplayEffect의 Execution을 사용자 정의에 따라 GameplayEffect의 처리할 수 있음.
// - HealExecution 이름에서 알 수 있듯이, HealthAttribute의 Healing을 적용함.
UCLASS()
class SHOOTERX_API ULyraHealExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	ULyraHealExecution();

	// 해당 메서드는 GameplayEffectExecutionCalculation의 Execute() BlueprintNativeEvent를 오버라이드 함.
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
