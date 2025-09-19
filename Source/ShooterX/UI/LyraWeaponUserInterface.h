// LyraWeaponUserInterface.h

#pragma once

#include "CommonUserWidget.h"
#include "GameplayAbilities/Public/GameplayAbilitySpecHandle.h" // 왜인지 모르겠으나, LyraWeaponUserInterface 클래스에서 FGameplayAbilitySpecHandle을 필요로함.
#include "LyraWeaponUserInterface.generated.h"

class ULyraWeaponInstance;

UCLASS()
class SHOOTERX_API ULyraWeaponUserInterface : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	ULyraWeaponUserInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Weapon 변경에 따른 BP Event
	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponChanged(ULyraWeaponInstance* OldWeapon, ULyraWeaponInstance* NewWeapon);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// 현재 장착된 WeaponInstnace를 추적. NativeTick() 함수를 활용하여 주기적 업데이트를 함. 별로 좋은 로직은 아님.
	UPROPERTY(Transient)
	TObjectPtr<ULyraWeaponInstance> CurrentInstance;

};
