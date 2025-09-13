// LyraInventoryItemFragment_EquippableItem.h

#pragma once

#include "LyraInventoryItemDefinition.h"
#include "Templates/SubclassOf.h"
#include "LyraInventoryItemFragment_EquippableItem.generated.h"

class ULyraEquipmentDefinition;

UCLASS()
class SHOOTERX_API ULyraInventoryItemFragment_EquippableItem : public ULyraInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ULyraEquipmentDefinition> EquipmentDefinition;
	
};
