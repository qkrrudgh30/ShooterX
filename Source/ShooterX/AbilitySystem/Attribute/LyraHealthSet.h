// LyraHealthSet.h

#pragma once

#include "LyraAttributeSet.h"
#include "LyraHealthSet.generated.h"

UCLASS(BlueprintType)
class LC_API ULyraHealthSet : public ULyraAttributeSet
{
	GENERATED_BODY()

public:
	ULyraHealthSet();

	// LyraAttributeSet에서 정의했던 ATTRIBUTE_ACCESSORS() 매크로 함수를 통해 아래 멤버 변수 관련 멤버 함수 정의.
	ATTRIBUTE_ACCESSORS(ULyraHealthSet, Health);
	ATTRIBUTE_ACCESSORS(ULyraHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(ULyraHealthSet, Healing);

public:
	UPROPERTY(BlueprintReadOnly, Category="LyraClone|Health")
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category="LyraClone|Health")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category="LyraClone|Health")
	FGameplayAttributeData Healing;

};
