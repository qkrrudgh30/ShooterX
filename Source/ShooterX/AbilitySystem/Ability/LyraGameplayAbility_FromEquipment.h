// LyraGameplayAbility_FromEquipment.h

#pragma once

#include "LyraGameplayAbility.h"
#include "LyraGameplayAbility_FromEquipment.generated.h"

class ULyraEquipmentInstance;

UCLASS()
class SHOOTERX_API ULyraGameplayAbility_FromEquipment : public ULyraGameplayAbility
{
	GENERATED_BODY()

public:
	ULyraGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	ULyraEquipmentInstance* GetAssociatedEquipment() const;
	
};
