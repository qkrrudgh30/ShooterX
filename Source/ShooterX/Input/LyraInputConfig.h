// LyraInputConfig.h

#pragma once

#include "Containers/Array.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "LyraInputConfig.generated.h"

class UInputAction;

// LCInputAction은 GameplayTag와 InputAction을 연결하는 Wrapper 클래스.
USTRUCT(BlueprintType)
struct FLCInputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

};

// LyraInputConfig는 활성화 가능한 InputAction들을 가지고 있는 클래스.
// - 이는 코드를 보면서 무슨 설명인지 와닿게됨.
UCLASS(BlueprintType)
class SHOOTERX_API ULyraInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	ULyraInputConfig(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;
	
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FLCInputAction> NativeInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FLCInputAction> AbilityInputActions;

};
