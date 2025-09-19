// LyraEquipmentManagerComponent.h

#pragma once

#include "Components/PawnComponent.h"
#include "AbilitySystem/LyraAbilitySet.h"
#include "LyraEquipmentManagerComponent.generated.h"

class ULyraEquipmentDefinition;
class ULyraEquipmentInstance;

USTRUCT(BlueprintType)
struct FLyraAppliedEquipmentEntry
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TSubclassOf<ULyraEquipmentDefinition> EquipmentDefinition;
	// 장착물에 대한 메타 데이터

	UPROPERTY()
	TObjectPtr<ULyraEquipmentInstance> Instance = nullptr;
	// 이큅먼트데피니션을 통해 생성된 인스턴스

	UPROPERTY()
	FLyraAbilitySet_GrantedHandles GrantedHandles;
	// 무기에 할당된 어빌리티의 핸들들

};

// 참고로 EquipmentInstance의 인스턴스를 Entry에서 관리하고 있음.
// - EquipmentList가 생성된 개체를 관리하고 있음.
USTRUCT(BlueprintType)
struct FLyraEquipmentList
{
	GENERATED_BODY()

public:
	FLyraEquipmentList(UActorComponent* InOwnerComponent = nullptr)
		: OwnerComponent(InOwnerComponent)
	{

	}

	ULyraEquipmentInstance* AddEntry(TSubclassOf<ULyraEquipmentDefinition> EquipmentDefinition);

	void RemoveEntry(ULyraEquipmentInstance* Instance);

	ULyraAbilitySystemComponent* GetAbilitySystemComponent() const;

public:
	UPROPERTY()
	TArray<FLyraAppliedEquipmentEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;

};

// PawnComponent로서 장착물에 대한 관리 담당.
UCLASS(BlueprintType)
class SHOOTERX_API ULyraEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	ULyraEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	ULyraEquipmentInstance* EquipItem(TSubclassOf<ULyraEquipmentDefinition> EquipmentDefinition);

	void UnequipItem(ULyraEquipmentInstance* ItemInstance);

	UFUNCTION(BlueprintCallable)
	TArray<ULyraEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<ULyraEquipmentInstance> InstanceType) const;
	
	// 장착물 중 처음 것을 반환. 없으면 nullptr
	ULyraEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<ULyraEquipmentInstance> InstanceType);

	template <typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType(T::StaticClass());
	}
	
public:
	UPROPERTY()
	FLyraEquipmentList EquipmentList;

};
