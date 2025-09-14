// LyraEquipmentInstance.cpp

#include "LyraEquipmentInstance.h"
#include "Equipment/LyraEquipmentDefinition.h"
#include "GameFramework/Character.h"

ULyraEquipmentInstance::ULyraEquipmentInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

APawn* ULyraEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
	// 우리는 EquipmentInstance를 생성할 당시에 Outer를 Pawn으로 두엇음.
}

APawn* ULyraEquipmentInstance::GetTypedPawn(TSubclassOf<APawn> PawnType) const
{
	APawn* Result = nullptr;
	if (UClass* ActualPawnType = PawnType)
	{
		if (GetOuter()->IsA(ActualPawnType))
		{
			Result = Cast<APawn>(GetOuter());
		}
	}

	return Result;
}

void ULyraEquipmentInstance::SpawnEquipmentActors(const TArray<FLyraEquipmentActorToSpawn>& ActorsToSpawn)
{
	if (APawn* OwningPawn = GetPawn())
	{
		USceneComponent* AttachTarget = OwningPawn->GetRootComponent();
		// 현재 Outer인 Pawn의 RootComponent를 AttachTarget 대상으로 삼음.
		if (ACharacter* Char = Cast<ACharacter>(OwningPawn))
		{
			AttachTarget = Char->GetMesh();
			// 만약 캐릭터라면 SkeletalMeshComponent가 있으면 GetMesh로 반환하여 여기에 Attach
		}

		for (const FLyraEquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
		{
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn);
			NewActor->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/true);
			// SpawnActorDeferred는 FinishSpawning을 호출해야 Spawn 완성. 즉 작성자에세 코드로서 Ownership이 있다는 의미.

			NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);
			// Actor의 RelativeTransform을 AttachTransform으로 설정

			NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);
			// AttachTarget에 부착(Actor->Actor)

			SpawnedActors.Add(NewActor);
		}
	}
}

void ULyraEquipmentInstance::OnEquipped()
{
	K2_OnEquipped();
}

void ULyraEquipmentInstance::DestroyEquipmentActors()
{
	// 참고로 장착물이 복수 개의 Actor Mesh로 구성되어 있을 수 있다.
	// - 갑옷 Lv10이엇지만, 상하체로 같이 구성되어 있을 수도 있음.
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}

void ULyraEquipmentInstance::OnUnequipped()
{
	K2_OnUnequipped();
}
