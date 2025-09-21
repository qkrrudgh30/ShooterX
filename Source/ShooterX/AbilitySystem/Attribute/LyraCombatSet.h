// LyraCombatSet.h

#pragma once

#include "AbilitySystemComponent.h"
#include "LyraAttributeSet.h"
#include "LyraCombatSet.generated.h"

// CombatSet은 이름 그대로 전투와 관련된 Attribute를 담고 있는 Set.
// 현재는 BaseHeal만 있지만, BaseDamage도 추가하여 완전한 CombatSet에 필요한 AttributSet를 정의 가능.
UCLASS(BlueprintType)
class ULyraCombatSet : public ULyraAttributeSet
{
	GENERATED_BODY()

public:
	ULyraCombatSet();

	ATTRIBUTE_ACCESSORS(ULyraCombatSet, BaseHeal);

	// FGameplayAttribute는 실제 AttributeSet에 있는 Attribute를 참고 하고 있음. float을 직접 참조하는 것 보다 이 구조체를 사용하는 것을 추천.
	// - Healing의 단위를 의미함.
	// - ex) 5.f라면 Period 당 5씩 Healing.
	UPROPERTY(BlueprintReadOnly, Category="LyraClone|Combat")
	FGameplayAttributeData BaseHeal;

};
