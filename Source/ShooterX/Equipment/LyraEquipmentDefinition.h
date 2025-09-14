// LyraEquipmentDefinition.h

#pragma once

#include "UObject/Object.h"
#include "Containers/Array.h"
#include "Math/Transform.h"
#include "Templates/SubclassOf.h"
#include "UObject/NameTypes.h"
#include "LyraEquipmentDefinition.generated.h"

class ULyraEquipmentInstance;
class ULyraAbilitySet;

USTRUCT()
struct FLyraEquipmentActorToSpawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category=Equipment)
	TSubclassOf<AActor> ActorToSpawn;
	// 스폰할 대상 액터 클래스(Actor를 상속 받은 Asset으로 생각해도됨.)

	UPROPERTY(EditAnywhere, Category=Equipment)
	FName AttachSocket;
	// 어느 소켓에 붙일지 결정.

	UPROPERTY(EditAnywhere, Category=Equipment)
	FTransform AttachTransform;
	// 소켓으로부터의 오프셋 트랜스폼
	
};

// BlueprintType과 Blueprinttable
// - BlueprintType은 BP에서 해당 클래스를 변수로서 사용 가능하게끔 함.
// - Blueprintable은 해당 클래스를 상속 받는 모든 클래스는 Blueprint 애셋으로 만들 수 있게끔 함.

// LyraEquipmentDefinition은 장착 아이템에 대한 정의 클래스(메타 데이터)
UCLASS(BlueprintType, Blueprintable)
class SHOOTERX_API ULyraEquipmentDefinition : public UObject
{
	GENERATED_BODY()

public:
	ULyraEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TSubclassOf<ULyraEquipmentInstance> InstanceType;
	// 해당 메타 데이터를 사용하면 어떤 인스턴스를 스폰할 지 결정하는 클래스

	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<FLyraEquipmentActorToSpawn> ActorsToSpawn;
	// 해당 장착 아이템을 장착 하여서 스폰"될" 액터의 정보을 담아두는 컨테이너
	// ex) 한벌 옷은 데피니션은 하나지만 상의와 하의 액터가 따로 스폰될 수 있음.

	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<TObjectPtr<ULyraAbilitySet>> AbilitySetsToGrant;
	// 장착을 통해 부여 가능한 AbilitySet

};
