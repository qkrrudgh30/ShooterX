// LyraPawnComponent_CharacterParts.h

#pragma once

#include "Components/PawnComponent.h"
#include "LyraCharacterPartTypes.h"
#include "LyraCosmeticAnimationTypes.h"
#include "LyraPawnComponent_CharacterParts.generated.h"

class ULyraPawnComponent_CharacterParts;

// 인스턴스화된 캐릭터 파츠의 단위
USTRUCT()
struct FLyraAppliedCharacterPartEntry
{
	GENERATED_BODY()

public:
	// 캐릭터 파츠의 정의(메타 데이터)
	UPROPERTY()
	FLyraCharacterPart Part;

	// LyraCharacterBasePartList에서 할당 받은 Part 핸들값. FLyraControllerCharacterPartEntry의 Handle 값과 같아야함. 같아야 동일한 파츠로 인정.
	UPROPERTY()
	int32 PartHandle = INDEX_NONE;

	// 인스턴스화 된 캐릭터 파츠용 컴포넌트?
	UPROPERTY()
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;
};

// PawnComponent_CharacterParts에서 실질적 캐릭터 파츠를 관리하는 클래스
USTRUCT(BlueprintType)
struct FLyraCharacterPartList
{
	GENERATED_BODY()

public:
	FLyraCharacterPartList()
		: OwnerComponent(nullptr)
	{

	}

	FLyraCharacterPartList(ULyraPawnComponent_CharacterParts* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{

	}

	FLyraCharacterPartHandle AddEntry(FLyraCharacterPart NewPart);

	bool SpawnActorForEntry(FLyraAppliedCharacterPartEntry& Entry);

	FGameplayTagContainer CollectCombinedTags() const;

	void RemoveEntry(FLyraCharacterPartHandle Handle);

	void DestroyActorForEntry(FLyraAppliedCharacterPartEntry& Entry);

public:
	// 현재 인스턴스화 된 캐릭터 파츠
	UPROPERTY()
	TArray<FLyraAppliedCharacterPartEntry> Entries;

	// 해당 LyraCharacterBasePartList의 Owner인 PawnComponent
	UPROPERTY()
	TObjectPtr<ULyraPawnComponent_CharacterParts> OwnerComponent;

	// 앞서 보았던 PartHandle의 값을 할당 및 관리하는 변수
	int32 PartHandleCounter = 0;

};

// PawnComponent로서, Character Parts를 인스턴스화하여 관리함.
UCLASS(meta=(BlueprintSpawnableComponent))
class ULyraPawnComponent_CharacterParts : public UPawnComponent
{
	GENERATED_BODY()

public:
	ULyraPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	FLyraCharacterPartHandle AddCharacterPart(const FLyraCharacterPart& NewPart);

	USkeletalMeshComponent* GetParentMeshComponent() const;

	USceneComponent* GetSceneComponentToAttachTo() const;

	void BroadcastChanged();

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category=Cosmetic)
	FGameplayTagContainer GetCombinedTags(FGameplayTag RequiredPrefix) const;

	void RemoveCharacterPart(FLyraCharacterPartHandle Handle);

public:
	// 인스턴스화 된 캐릭터파츠.
	UPROPERTY()
	FLyraCharacterPartList CharacterPartList;

	// 애니메이션 적용을 위한 메시와 연결고리
	UPROPERTY(EditAnywhere, Category=Cosmetics)
	FLyraAnimBodyStyleSelectionSet BodyMeshes;

};