// LyraQuickBarComponent.cpp

#include "LyraQuickBarComponent.h"
#include "LyraEquipmentManagerComponent.h"
#include "LyraEquipmentInstance.h"
#include "LyraEquipmentDefinition.h"
#include "Inventory/LyraInventoryItemInstance.h"
#include "Inventory/LyraInventoryItemFragment_EquippableItem.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraQuickBarComponent)

ULyraQuickBarComponent::ULyraQuickBarComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ULyraQuickBarComponent::BeginPlay()
{
	Super::BeginPlay();

	// NumSlots에 따라 미리 Slots를 할당.
	if (Slots.Num() < NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}
}

void ULyraQuickBarComponent::AddItemToSlot(int32 SlotIndex, ULyraInventoryItemInstance* Item)
{
	// 해당 로직을 보면, Slots는 Add로 동적 추가가 아닌 Index에 바로 넣음.
	// - 그럼 미리 Pre-Size 했다는 이야기. 이는 BeginPlay()에서 진행.
	if (Slots.IsValidIndex(SlotIndex) && (Item != nullptr))
	{
		if (nullptr == Slots[SlotIndex])
		{
			Slots[SlotIndex] = Item;
		}
	}
}

ULyraEquipmentManagerComponent* ULyraQuickBarComponent::FindEquipmentManager() const
{
	if (AController* OwnerController = Cast<AController>(GetOwner()))
	{
		if (APawn* Pawn = OwnerController->GetPawn())
		{
			return Pawn->FindComponentByClass<ULyraEquipmentManagerComponent>();
		}
	}
	return nullptr;
}

void ULyraQuickBarComponent::EquipItemInSlot()
{
	check(Slots.IsValidIndex(ActiveSlotIndex));
	check(EquippedItem == nullptr);

	// 현재 활성화된 ActiveSlotIndex를 활용하여 활성화된 InventoryItemInstance를 찾는다.
	if (ULyraInventoryItemInstance* SlotItem = Slots[ActiveSlotIndex])
	{
		// Slot Item을 통해 InventoryItemInstance InventoryFragment_EquippableItem의 Fragment를 찾는다.
		// - 찾는 것에 실패 했다면, 장착할 수 없는 Inventory Item임을 의미함.
		if (const ULyraInventoryItemFragment_EquippableItem* EquipInfo = SlotItem->FindFragmentByClass<ULyraInventoryItemFragment_EquippableItem>())
		{
			// EquippableItem에서 EquipmentDefinition을 찾음.
			// - EquipmentDefinition이 있어야 장착 가능.
			TSubclassOf<ULyraEquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition;
			if (EquipDef)
			{
				// 아래는 Unequip()이랑 비슷하게 EquipmentManager를 통해 장착함.
				if (ULyraEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
				{
					EquippedItem = EquipmentManager->EquipItem(EquipDef);
					if (EquippedItem)
					{
						// EquippedItem에는 앞서 보았던 Instigator로 Slot을 대상을 ㅗ널음.
						EquippedItem->Instigator = SlotItem;
					}
				}
			}
		}
	}
}

void ULyraQuickBarComponent::UnequipItemInSlot()
{
	// 참고로 퀵바는 컨트롤러에 붙어 있는 컴포넌트이지만, EquipmentManagerComponent는 컨트롤러가 빙의한 폰의 컴포넌트임.
	if (ULyraEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
	{
		// 현재 장착된 Item에 있다면,
		if (EquippedItem)
		{
			// EquipmentManager를 통해 Pawn의 장비를 해제 시킨다.
			EquipmentManager->UnequipItem(EquippedItem);

			// 그리고 Controller에도 EquipItem의 상태를 없는 것으로 업데이트.
			EquippedItem = nullptr;
		}
	}
}

void ULyraQuickBarComponent::SetActiveSlotIndex(int32 NewIndex)
{
	if (Slots.IsValidIndex(NewIndex) && (ActiveSlotIndex != NewIndex))
	{
		// UnequipItem() / EquipItem() 함수를 통해 NewIndex에 할당된 Item을 장착 및 업데이트 진행.
		UnequipItemInSlot();
		ActiveSlotIndex = NewIndex;
		EquipItemInSlot();
	}
}
