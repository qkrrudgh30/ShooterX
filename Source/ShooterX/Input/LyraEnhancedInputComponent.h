// LyraEnhancedInputComponent.h

#pragma once

#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "LyraInputConfig.h"
#include "LyraEnhancedInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERX_API ULyraEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	ULyraEnhancedInputComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	template<class UserClass, typename FuncType>
	void BindNativeAction(const ULyraInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound);

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const ULyraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);

};

template<class UserClass, typename FuncType>
inline void ULyraEnhancedInputComponent::BindNativeAction(const ULyraInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	check(InputConfig);

	// 여기서 알 수 있듯이 InputConfig는 활성화 가능한 InputAction을 담고 있음.
	// - 만약 InputConfig에 없는 InputAction을 Binding 시키면, nullptr을 반환하여 바인딩하는데 실패하게 됨.
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
inline void ULyraEnhancedInputComponent::BindAbilityActions(const ULyraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);

	// AbilityAction에 대해서는 그냥 모든 InputAction에 다 바인딩 시킴
	for (const FLCInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle());
			}

			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle());
			}
		}
	}
}
