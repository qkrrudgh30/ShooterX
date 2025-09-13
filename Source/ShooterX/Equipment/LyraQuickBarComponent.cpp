// LyraQuickBarComponent.cpp

#include "LyraQuickBarComponent.h"
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
