// LyraEquipmentInstance.h

#pragma once

#include "UObject/Object.h"
#include "UObject/UObjectGlobals.h"
#include "Containers/Array.h"
#include "LyraEquipmentInstance.generated.h"

struct FLyraEquipmentActorToSpawn;

UCLASS(BlueprintType, Blueprintable)
class SHOOTERX_API ULyraEquipmentInstance : public UObject
{
	GENERATED_BODY()

public:
	ULyraEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintPure, Category=Equipment)
	TArray<AActor*> GetSpawnedActors() const { return SpawnedActors; }

	UFUNCTION(BlueprintPure, Category=Equipment)
	UObject* GetInstigator() const { return Instigator; }

	UFUNCTION(BlueprintPure, Category=Equipment)
	APawn* GetPawn() const;

	UFUNCTION(BlueprintPure, Category=Equipment, meta=(DeterminesOutputType=PawnType))
	APawn* GetTypedPawn(TSubclassOf<APawn> PawnType) const;
	// DeterminesOutputType은 C++ 정의에는 APawn*를 반환하지만, BP에서는 PawnType에 따라 OutputType이 결정되도록 리다이렉트함.

	void SpawnEquipmentActors(const TArray<FLyraEquipmentActorToSpawn>& ActorsToSpawn);

	UFUNCTION(BlueprintImplementableEvent, Category=Equipment, meta=(DisplayName="OnEquipped"))
	void K2_OnEquipped();
	
	virtual void OnEquipped();

	void DestroyEquipmentActors();

	UFUNCTION(BlueprintImplementableEvent, Category=Equipment, meta=(DisplayName="OnUnequipped"))
	void K2_OnUnequipped();

	virtual void OnUnequipped();

public:
	UPROPERTY()
	TObjectPtr<UObject> Instigator;
	// 어떤 인벤토리아이템인스턴스에 의해 활성화되었는지. 추후 퀵바컴포넌트에서 보게 될 예정.

	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;
	// 이큅먼트데피니션에 따라 스폰된 액터 개체들
	// 나중에 이 어레이를 순회하며 파괴하면 됨.
	
};
