// LyraInventoryManagerComponent.cpp

#include "LyraInventoryManagerComponent.h"
#include "LyraInventoryItemInstance.h"
#include "LyraInventoryItemDefinition.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraInventoryManagerComponent)

ULyraInventoryItemInstance* FLyraInventoryList::AddEntry(TSubclassOf<ULyraInventoryItemDefinition> ItemDef)
{
	ULyraInventoryItemInstance* Result = nullptr;
	check(ItemDef);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	FLyraInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<ULyraInventoryItemInstance>(OwningActor);
	NewEntry.Instance->ItemDef = ItemDef;

	Result = NewEntry.Instance;
	return Result;
}

ULyraInventoryManagerComponent::ULyraInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{
}

ULyraInventoryItemInstance* ULyraInventoryManagerComponent::AddItemDefinition(
	TSubclassOf<ULyraInventoryItemDefinition> ItemDef)
{
	ULyraInventoryItemInstance* Result = nullptr;
	if (ItemDef)
	{
		Result = InventoryList.AddEntry(ItemDef);
	}

	return Result;
}
