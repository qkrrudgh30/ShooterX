// LyraInventoryManagerComponent.h

#pragma once

#include "Components/ActorComponent.h"
#include "LyraInventoryManagerComponent.generated.h"

class ULyraInventoryItemInstance;

USTRUCT(BlueprintType)
struct FLyraInventoryEntry
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<ULyraInventoryItemInstance> Instance = nullptr;
	
};

USTRUCT(BlueprintType)
struct FLyraInventoryList
{
	GENERATED_BODY()

public:
	FLyraInventoryList(UActorComponent* InOwnerComponent = nullptr)
		: OwnerComponent(InOwnerComponent)
	{

	}

public:
	UPROPERTY()
	TArray<FLyraInventoryEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;

};

// PlayerController의 Component로서 Inventory를 관리
// 사실 UActorComponent 상속이 아닌 UControllerComponent를 상속 받아도 될 것 같은데, 일단 라이라 프로젝트 기준으로 UActorComponent를 상속 받고 있음.
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTERX_API ULyraInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULyraInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UPROPERTY()
	FLyraInventoryList InventoryList;

};
