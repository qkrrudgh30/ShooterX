// LyraInputConfig.cpp


#include "Input/LyraInputConfig.h"
#include "ShooterX.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraInputConfig)

ULyraInputConfig::ULyraInputConfig(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const UInputAction* ULyraInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	// NativeInputActions를 순회하며 Input으로 들어온 InputTag가 있는지 체크.
	// - 있으면 그에 따른 InputAction을 반환하지만 없다면 그냥 nullptr을 반환함. 허용된 InputAction을 사용하는지 Validation 해주는 함수.
	for (const FLCInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (true == bLogNotFound)
	{
		UE_LOG(LogSX, Error, TEXT("can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}

const UInputAction* ULyraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FLCInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (true == bLogNotFound)
	{
		UE_LOG(LogSX, Error, TEXT("can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}