// LyraHealthSet.h

#pragma once

#include "LyraAttributeSet.h"
#include "LyraHealthSet.generated.h"

UCLASS(BlueprintType)
class SHOOTERX_API ULyraHealthSet : public ULyraAttributeSet
{
	GENERATED_BODY()

public:
	ULyraHealthSet();

	// LyraAttributeSet에서 정의했던 ATTRIBUTE_ACCESSORS() 매크로 함수를 통해 아래 멤버 변수 관련 멤버 함수 정의.
	ATTRIBUTE_ACCESSORS(ULyraHealthSet, Health);
	ATTRIBUTE_ACCESSORS(ULyraHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(ULyraHealthSet, Healing);

	// Attribute의 값을 ClampAttribute() 함수를 활용하여 값의 범위를 유지시켜주기 위해
	// PreAttributeBaseChange() 함수와 PreAttributeChange() 함수 오버라이드
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// GameplayEffect가 HealthSet의 Attribute를 수정하기 전에 호출되는 콜백함수.
	// 이는 AttributeSet의 주석에도 나와 있듯이, Healing이 업데이트되면 Health를 Healing을 적용하여 업데이트 가능.
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

public:
	UPROPERTY(BlueprintReadOnly, Category="LyraClone|Health")
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category="LyraClone|Health")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category="LyraClone|Health")
	FGameplayAttributeData Healing;

};
