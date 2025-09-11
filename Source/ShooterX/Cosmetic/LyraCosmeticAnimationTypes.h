// LyraCosmeticAnimationTypes.h

#pragma once

#include "GameplayTagContainer.h"
#include "LyraCosmeticAnimationTypes.generated.h"

class USkeletalMesh;
class UPhysicsAsset;

// 스켈레탈메시에 관한 정보. 스켈레탈메시와 태그를 묶어둠.
// ex) 전사라면 전사에 맞는 스켈레탈메시, 괴물이라면 괴물에 맞는 스켈레탈메시를 분간하기 위해 태그도 포함.
USTRUCT(BlueprintType)
struct FLyraAnimBodyStyleSelectionEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> Mesh = nullptr;
	// AnimLayer를 적용할 대상 SkeletalMesh

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Categories="Cosmetic"))
	FGameplayTagContainer RequiredTags;
	// Cosmetic Tag라고 생각하면 됨.

};

// PawnComponent_CharacterPart에 의해 파츠액터가 부착될 예정.
// 액터는 부착되어봐야 아무것도 없음. 결국 일은 컴포넌트가 다하게됨. 그 중에서 AnimBodyStyleSelectionSet은 스켈레탈메시컴포넌트에 관한 정보.
USTRUCT(BlueprintType)
struct FLyraAnimBodyStyleSelectionSet
{
	GENERATED_BODY()

public:
	// GameplayTag를 통해(CosmeticTags) MeshRules에 따라 알맞은 BodyStyle을 반환
	USkeletalMesh* SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLyraAnimBodyStyleSelectionEntry> MeshRules;
	// AnimLayer 적용할 SkeletalMesh를 들고 있음 -> Animation과 Mesh 간의 Rules를 MeshRules라고 생각하면 됨.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> DefaultMesh = nullptr;
	// 디폴트로 적용할 SkeletalMesh

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPhysicsAsset> ForcedPhysicsAsset = nullptr;
	// Physics Asset은 하나로 통일함. 즉 모든 Animation의 Physics 속성은 공유됨.

};