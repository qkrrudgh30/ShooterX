// LyraHealExecution.cpp

#include "AbilitySystem/Execution/LyraHealExecution.h"
#include "AbilitySystem/Attribute/LyraHealthSet.h"
#include "AbilitySystem/Attribute/LyraCombatSet.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraHealExecution)

// 해당 구조체를 사용하여 FGameplayEffectAttributeCaptureDefinition 인스턴스를 전달
struct FHealStatics
{
	// AttributeSet의 어떤 Attribute를 캡쳐할 것인지와 어떻게 캡쳐할지 정의를 담고 있음. 한번 보고 오면 좋음.
	FGameplayEffectAttributeCaptureDefinition BaseHealDef;

	FHealStatics()
	{
		BaseHealDef = FGameplayEffectAttributeCaptureDefinition(ULyraCombatSet::GetBaseHealAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	}
};

static FHealStatics& HealStatics()
{
	// 계속 FHealStatics를 생성하는 것은 부하이니 한 번만 생성하고 재사용.
	static FHealStatics Statics;
	return Statics;
}

ULyraHealExecution::ULyraHealExecution()
	: Super()
{
	// Source로 입력값 Attribute를 캡쳐를 정의하자
	// - CombatSet::BaseHeal을 통해 Healing 값을 정의하고 최종 Execute할 때 값을 가져와서 Health에 Healing을 적용.
	RelevantAttributesToCapture.Add(HealStatics().BaseHealDef);
}

void ULyraHealExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	
	// GameplayEffectSpec은 GameplayEffect의 핸들로 생각하면 됨.
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	float BaseHeal = 0.f;
	{
		FAggregatorEvaluateParameters EvaluateParameters;

		// 해당 함수 호출을 통해 LyraCombatSet의 BaseHeal 값을 가져옴. 혹은 뭔가 Modifier에 누적되어 있다면 최종 계산 결과가 나옴.
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealStatics().BaseHealDef, EvaluateParameters, BaseHeal);
	}

	// RelevantAttributesCapture를 통해 최종 계산된 BaseHeal을 0 이하가 되지 않도록 함. Healing이기 때문.
	const float HealingDone = FMath::Max(0.f, BaseHeal);
	if (0.f < HealingDone)
	{
		// GameplayEffectCalculation 이후 Modifier로서 추가함.
		// - 해당 Modifier는 CombatSet에서 가져온 BaseHeal을 활용하여 HealthSet의 Healing에 추가해줌.
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(ULyraHealthSet::GetHealingAttribute(), EGameplayModOp::Additive, HealingDone));
	}
}