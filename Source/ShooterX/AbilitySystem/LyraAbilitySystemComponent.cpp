// LyraAbilitySystemComponent.cpp

#include "LyraAbilitySystemComponent.h"
#include "Ability/LyraGameplayAbility.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraAbilitySystemComponent)

ULyraAbilitySystemComponent::ULyraAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

void ULyraAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		// 부여된 GameplayAbilitySpec을 순회
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			// Ability가 존재하고 DynamicAbilityTags에 InputTag가 있을 경우 InputPressed/Held에 넣어 어빌리티 처리를 대기.
			if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void ULyraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
			{
				// Released에 추가하고 Held에서는 제거.
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void ULyraAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;

	// InputHeldSpecHandles에 대해 Ability 처리를 위해 AbilityToActivate에 추가함.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		// FindAbilitySpecFromHandle() 함수 확인
		// - ActivatableAbilities의 Handle 값 비교를 통해 GameplayAbilitySpec을 반환
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const ULyraGameplayAbility* LCAbilityCDO = CastChecked<ULyraGameplayAbility>(AbilitySpec->Ability);

				// ActivationPolicy가 WhileInputActive면 활성화로 등록
				if (LCAbilityCDO->ActivationPolicy == ELyraAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					// 이미 Ability가 활성화 되어 있을 경우 Input Event(InputPressed)만 호출
					// - AbilitySpecInputPressed 확인
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const ULyraGameplayAbility* LCAbilityCDO = CastChecked<ULyraGameplayAbility>(AbilitySpec->Ability);

					// ActivationPolicy가 OnInputTriggered면 활성화로 등록
					if (LCAbilityCDO->ActivationPolicy == ELyraAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}

	// 등록된 AbilitiesToActivate를 한꺼번에 등록 시작.
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		// 모든 것이 잘 진행되었다면 CallActivate() 호출로 Blueprint의 Activate 노드가 실행될 예정.
		TryActivateAbility(AbilitySpecHandle);
	}

	// 이번 프레임에 Release 되었다면 관련 GameplayAbility 처리
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;
				if (AbilitySpec->IsActive())
				{
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	// InputHeldSpecHandles는 InputReleasedSpecHandles 추가 될 때 제거됨.
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}
