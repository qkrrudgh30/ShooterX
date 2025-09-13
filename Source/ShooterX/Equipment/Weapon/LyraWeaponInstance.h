// LyraWeaponInstance.h

#pragma once

#include "Equipment/LyraEquipmentInstance.h"
#include "Cosmetic/LyraCosmeticAnimationTypes.h"
#include "LyraWeaponInstance.generated.h"

UCLASS()
class SHOOTERX_API ULyraWeaponInstance : public ULyraEquipmentInstance
{
	GENERATED_BODY()

public:
	ULyraWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	FLyraAnimLayerSelectionSet EquippedAnimSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	FLyraAnimLayerSelectionSet UnequippedAnimSet;
	
};
