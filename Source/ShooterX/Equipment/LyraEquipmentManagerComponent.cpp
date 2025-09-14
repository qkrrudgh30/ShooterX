// LyraEquipmentManagerComponent.cpp

#include "LyraEquipmentManagerComponent.h"
#include "LyraEquipmentDefinition.h"
#include "LyraEquipmentInstance.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraEquipmentManagerComponent)

ULyraEquipmentInstance* FLyraEquipmentList::AddEntry(TSubclassOf<ULyraEquipmentDefinition> EquipmentDefinition)
{
	ULyraEquipmentInstance* Result = nullptr;
	check(EquipmentDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());

	const ULyraEquipmentDefinition* EquipmentCDO = GetDefault<ULyraEquipmentDefinition>(EquipmentDefinition);
	// EquipmentDefinition의 멤버 변수들은 EditDefaultsOnly로 정의되어 있어서 GetDefault()로 들고와도 괜찮음.
	TSubclassOf<ULyraEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (!InstanceType)
	{
		InstanceType = ULyraEquipmentInstance::StaticClass();
	}

	FLyraAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<ULyraEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);
	Result = NewEntry.Instance;
	// Entries에 추가

	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);
	// ActorsToSpawn을 통해 Actor들을 개체화. 어디에? EquipmentInstance에.

	ULyraAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	check(ASC)
	{
		for (TObjectPtr<ULyraAbilitySet> AbilitySet : EquipmentCDO->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(ASC, &NewEntry.GrantedHandles, Result);
		}
	}

	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);

	return Result;
}

void FLyraEquipmentList::RemoveEntry(ULyraEquipmentInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FLyraAppliedEquipmentEntry& Entry = *EntryIt;
		// 단순히 그냥 Entries를 순회하며 Instance를 찾아서
		if (Entry.Instance == Instance)
		{
			ULyraAbilitySystemComponent* ASC = GetAbilitySystemComponent();
			check(ASC);
			{
				// TakeFromAbilitySystem() 함수는 GiveToAbilitySystem() 함수의 반대. ActivatableAbilities에서 어빌리티 제거.
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
			}
			
			Instance->DestroyEquipmentActors();
			EntryIt.RemoveCurrent();
			// Actor 제거 작업 및 iterator를 통해 안전하게 Array에서 제거 진행
		}
	}
}

ULyraAbilitySystemComponent* FLyraEquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();

	// GetAbilitySystemComponentFromActor() 함수를 확인해보자.
	// - EquipmentManagerComponent는 ALCCharacter를 Owner로 가지고 있음.
	// - 해당 함수는 IAbilitySystemInterface를 통해 AbilitySystemComponent를 반환함.
	// - GetAbilitySystemComponentFromActor() 함수가 제대로 동작하려면 LyraCharacter가 IAbilitySystemInterface를 상속 받고 GetAbilitySystemComponent() 함수를 재정의 해줘야함.
	return Cast<ULyraAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

ULyraEquipmentManagerComponent::ULyraEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EquipmentList(this)
{
}

ULyraEquipmentInstance* ULyraEquipmentManagerComponent::EquipItem(
	TSubclassOf<ULyraEquipmentDefinition> EquipmentDefinition)
{
	ULyraEquipmentInstance* Result = nullptr;
	if (EquipmentDefinition)
	{
		Result = EquipmentList.AddEntry(EquipmentDefinition);
		if (Result)
		{
			Result->OnEquipped();
		}
	}

	return Result;
}

void ULyraEquipmentManagerComponent::UnequipItem(ULyraEquipmentInstance* ItemInstance)
{
	if (ItemInstance)
	{
		ItemInstance->OnUnequipped();
		// 해당 함수는 BP의 Event 노드를 호출해줌. 자세한건 해당 함수 구현하면서 보면됨.

		EquipmentList.RemoveEntry(ItemInstance);
		// EquipmentList에서 제거
		// - 제거하는 과정을 통해 추가되었던 Actor Instance를 제거 진행
	}
}

TArray<ULyraEquipmentInstance*> ULyraEquipmentManagerComponent::GetEquipmentInstancesOfType(
	TSubclassOf<ULyraEquipmentInstance> InstanceType) const
{
	TArray<ULyraEquipmentInstance*> Results;

	// EquipmentList를 순회하며
	for (const FLyraAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (ULyraEquipmentInstance* Instance = Entry.Instance)
		{
			// InstanceType에 맞는 클래스이면 Results에 추가하여 반환.
			// - 우리의 경우, LyraRangedWeaponInstance
			if (Instance->IsA(InstanceType))
			{
				Results.Add(Instance);
			}
		}
	}

	return Results;
}
