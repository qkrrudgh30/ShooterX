// LyraInventoryFragment_ReticleConfig.h

#pragma once

#include "Inventory/LyraInventoryItemDefinition.h"
#include "LyraInventoryFragment_ReticleConfig.generated.h"

class ULyraReticleWidgetBase;

UCLASS()
class SHOOTERX_API ULyraInventoryFragment_ReticleConfig : public ULyraInventoryItemFragment
{
	GENERATED_BODY()

public:
	// 무기에 결합된 ReticleWidget 정보를 가지고 있는 Fragment
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Reticle)
	TArray<TSubclassOf<ULyraReticleWidgetBase>> ReticleWidgets;

};
