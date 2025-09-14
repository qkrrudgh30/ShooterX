// LyraAbilitySet.h

#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "LyraAbilitySet.generated.h"

class ULyraGameplayAbility;
struct FGameplayAbilitySpecHandle;
class ULyraAbilitySystemComponent;

// GameplayAbility의 Wrapper 클래스
// - 추가적인 커스터마이징이 가능함.
USTRUCT(BlueprintType)
struct FLyraAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:
	// 허용된 GameplayAbility
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULyraGameplayAbility> Ability = nullptr;

	// Input 처리를 위한 GameplayTag
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;

	// Ability의 허용 조건(Level)
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

};

USTRUCT(BlueprintType)
struct FLyraAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);

	void TakeFromAbilitySystem(ULyraAbilitySystemComponent* LCASC);

protected:
	// 허용된 GameplayAbilitySpecHandle(int32)
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

};

// GameplayAbility를 좀 더 쉽게 관리하기 위한 Set
UCLASS(BlueprintType)
class SHOOTERX_API ULyraAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	ULyraAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// ASC에 허용가능한 어빌리티를 부여함.
	void GiveToAbilitySystem(ULyraAbilitySystemComponent* LCASC, FLyraAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr);

public:
	UPROPERTY(EditDefaultsOnly, Category="Gameplay Abilities")
	TArray<FLyraAbilitySet_GameplayAbility> GrantedGameplayAbilities;

};
