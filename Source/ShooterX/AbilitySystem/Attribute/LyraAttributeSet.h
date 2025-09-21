// LyraAttributeSet.h

#pragma once

#include "AttributeSet.h"
#include "GameplayAbilities/Public/AbilitySystemComponent.h"
#include "LyraAttributeSet.generated.h"

// 아래 매크로는 AttributeSet에 Attribute를 추가할 때, 선언 및 정의해야할 메서드에 대한 간략 버전을 제공함.
// 
// ATTRIBUTE_ACCESSORS(ULyraHealthSet, Health)
// 이는 아래의 메서드를 선언 및 정의해줌.
// static FGameplayAttribute GetHealthAttribute() { ... }
// float GetHealth() const { ... }
// void SetHealth(float NewValue) { ... }
// void InitHealth(float NewValue) { ... }

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) 

// LyraAttributeSet
// Lyra와 마찬가지로 LyraClone에서 메인 Attribute Set 클래스임.
UCLASS()
class SHOOTERX_API ULyraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	ULyraAttributeSet();

};
