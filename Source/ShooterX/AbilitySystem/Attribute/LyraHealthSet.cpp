// LyraHealthSet.cpp

#include "LyraHealthSet.h"
#include "GameplayAbilities/Public/GameplayEffectExtension.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraHealthSet)

ULyraHealthSet::ULyraHealthSet()
	: Super()
	, Health(50.f)
	, MaxHealth(100.f)
{
	
}


void ULyraHealthSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	// HealthAttribute는 [0, GetMaxHealth]로 설정
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	// MaxHealthAttribute는 [1.f, inf]로 설정. 즉, MaxHealth는 1 미만이 될 수 없음.
	else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
	}
}

void ULyraHealthSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void ULyraHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

bool ULyraHealthSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	return Super::PreGameplayEffectExecute(Data);
}

void ULyraHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinimumHealth = 0.f;

	// Healing이 업데이트 될 경우 Healing을 Health에 적용하고 Healing을 초기화 해줌.
	if (Data.EvaluatedData.Attribute == GetHealingAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth() + GetHealing(), MinimumHealth, GetMaxHealth()));
		SetHealing(0.f);
	}
	// Health 업데이트의 경우 [0, MaxHealth]로 맞추어주자
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::ClampAngle(GetHealth(), MinimumHealth, GetMaxHealth()));
	}
}
