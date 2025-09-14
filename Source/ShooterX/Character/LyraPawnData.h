// LyraPawnData.h

#pragma once

#include "Engine/DataAsset.h"
#include "LyraPawnData.generated.h"

class ULyraCameraMode;
class ULyraInputConfig;
class ULyraAbilitySet;

/**
 * ULyraPawnData
 *
 *	Non-mutable data asset that contains properties used to define a pawn.
 */
UCLASS()
class SHOOTERX_API ULyraPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	ULyraPawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ULyraPawnData")
	TSubclassOf<APawn> PawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ULyraPawnData")
	TSubclassOf<ULyraCameraMode> DefaultCameraMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LyraClone|InputConfig")
	TObjectPtr<ULyraInputConfig> InputConfig;

	// 해당 Pawn의 AbilitySystem에 허용할 AbilitySet
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LC|Abilities")
	TArray<TObjectPtr<ULyraAbilitySet>> AbilitySets;

};
