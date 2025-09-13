// LyraQuickBarComponent.h

#pragma once

#include "Components/ControllerComponent.h"
#include "LyraQuickBarComponent.generated.h"

class ULyraInventoryItemInstance;
class ULyraEquipmentInstance;

// HUD의 QuickBar를 생각하면 됨.
// - MMORPG에서 ShortCut HUD 같은 느낌.

// 해당 Component는 ControllerComponent로서 PlayerController애 의해 조작계 중 하나로 생각하면 됨.
// - HUD(Slate)와 Inventory/Equipment(Gameplay)의 매개역할 Component
// - 해당 Component는 Lyra의 HUD 및 Slate Widget을 다루면서 다시 보게될 예정.
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class ULyraQuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()

public:
	ULyraQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	// HUD QuickBar Slot 개수
	UPROPERTY()
	int32 NumSlots = 3;

	// HUD QuickBar Slot 리스트
	UPROPERTY()
	TArray<TObjectPtr<ULyraInventoryItemInstance>> Slots;

	// 현재 활성화된 Slot Index(아마 Lyra는 딱 한 개만 Slot이 활성화 되는듯?)
	UPROPERTY()
	int32 ActiveSlotIndex = -1;

	// 현재 장착한 장비 정보
	UPROPERTY()
	TObjectPtr<ULyraEquipmentInstance> EquippedItem;

};