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

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Animation")
	TSubclassOf<UAnimInstance> PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const;	
	// Weapon에 적용할 AnimLayer를 선택하여 반환.

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	FLyraAnimLayerSelectionSet EquippedAnimSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	FLyraAnimLayerSelectionSet UnequippedAnimSet;
	
};
