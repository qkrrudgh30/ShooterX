// LyraInventoryItemDefinition.h

#pragma once

#include "UObject/Object.h"
#include "LyraInventoryItemDefinition.generated.h"

// Inventory에 대한 Fragment는 와닿지 않을 수 있음.
// 라이라 프로젝트에서 사용하는 예시
// ULyraInventoryFragment_EquippableItem은 EquipmentItemDefinition을 가지고 있음. 장착 가능한 아이템을 뜻함.
// ULyraInventoryFragment_SetState는 아이템에 대한 정보를 가지고 있음.
// - 라이플에 대한 SetStats으로는 총알(Ammo)에 대한 장착 최대치와 현재 남은 잔탄 수 등등
UCLASS(Abstract, DefaultToInstanced, EditInlineNew)
class SHOOTERX_API ULyraInventoryItemFragment : public UObject
{
	GENERATED_BODY()
};

UCLASS(Blueprintable)
class SHOOTERX_API ULyraInventoryItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	ULyraInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display)
	FText DisplayName;
	// 인벤토리 아이템의 정의(메타) 이름

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category=Display)
	TArray<TObjectPtr<ULyraInventoryItemFragment>> Fragments;
	// 액터에 컴포넌트가 달려있는 것처럼, 인벤토리아이템에는 프레그먼트가 달리는 거라고 생각하면 됨.
	
};