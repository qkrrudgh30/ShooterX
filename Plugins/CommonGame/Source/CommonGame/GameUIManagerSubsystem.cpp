// GameUIManagerSubsystem.cpp

#include "GameUIManagerSubsystem.h"
#include "GameUIPolicy.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GameUIManagerSubsystem)

UGameUIManagerSubsystem::UGameUIManagerSubsystem()
	: Super()
{
}

void UGameUIManagerSubsystem::SwitchToPolicy(UGameUIPolicy* InPolicy)
{	
	if (CurrentPolicy != InPolicy)
	{
		CurrentPolicy = InPolicy;
	}
}

void UGameUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (nullptr == CurrentPolicy && false == DefaultUIPolicyClass.IsNull())
	{
		// CurrentPolicy가 설정되어 있지 않고, DefaultUIPolicyClass가 제대로 설정되어 있을 경우.
		
		TSubclassOf<UGameUIPolicy> PolicyClass = DefaultUIPolicyClass.LoadSynchronous();
		// UIPolicyClass는 BP 애셋이기에 로딩해야함.

		SwitchToPolicy(NewObject<UGameUIPolicy>(this, PolicyClass));
		// UIPolicyClass를 통해 NewObject로 인스턴싱해서 CurrentPolicy에 설정.
	}
}

void UGameUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
	SwitchToPolicy(nullptr);
}

bool UGameUIManagerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	bool bResult = Super::ShouldCreateSubsystem(Outer);
	// 기본값이 true임.
	
	if (false == CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> ChildClasses;
		GetDerivedClasses(GetClass(), ChildClasses, false);
		// 만약 어떠한 Subsystem도 UGameUIManagerSubsystem을 상속 받지 않았다면 해당 GameInstanceSubsystem은 활성화 안함.

		return ChildClasses.Num() == 0;
	}
	else
	{
		// DedicatedServer의 경우 GameUIManagerSubsystem을 활성화하지 않음.
		bResult = false;
	}

	return bResult;
}


void UGameUIManagerSubsystem::NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer)
{
	if (ensure(LocalPlayer) && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerAdded(LocalPlayer);
	}
}

void UGameUIManagerSubsystem::NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer)
{
	if (ensure(LocalPlayer) && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerRemoved(LocalPlayer);
	}
}

void UGameUIManagerSubsystem::NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer)
{
	if (LocalPlayer && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerDestroyed(LocalPlayer);
	}
}
