// LyraReticleWidgetBase.h

#pragma once

#include "CommonUserWidget.h"
#include "LyraReticleWidgetBase.generated.h"

class ULyraWeaponInstance;
class ULyraInventoryItemInstance;

UCLASS(Abstract)
class SHOOTERX_API ULyraReticleWidgetBase : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	ULyraReticleWidgetBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	void InitializeFromWeapon(ULyraWeaponInstance* InWeapon);

public:
	// WeaponInstance/InventoryInstance를 상태 추적용으로 캐싱 목적
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULyraWeaponInstance> WeaponInstance;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULyraInventoryItemInstance> InventoryInstance;

};
