// LyraAnimInstance.cpp

#include "LyraAnimInstance.h"
#include "AbilitySystemGlobals.h"

ULyraAnimInstance::ULyraAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ULyraAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			InitializeWithAbilitySystem(ASC);
			// 근데 이 시점에는 ASC가 없을 수도 있음. 그래서 다른 곳에서도 InitializeWithAbilitySystem() 함수를 호출해줄 필요가 있음.
		}
	}
}

void ULyraAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	GameplayTagPropertyMap.Initialize(this, ASC);
}
