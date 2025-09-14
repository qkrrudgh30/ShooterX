// LyraRangedWeaponInstance.h

#pragma once

#include "LyraWeaponInstance.h"
#include "LyraRangedWeaponInstance.generated.h"

UCLASS()
class SHOOTERX_API ULyraRangedWeaponInstance : public ULyraWeaponInstance
{
	GENERATED_BODY()

public:
	ULyraRangedWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	// 유효 사거리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WeaponConfig", meta=(ForceUnits=cm))
	float MaxDamageRange = 25000.f;

	// 총탄 범위(Sphere Trace Sweep)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WeaponConfig", meta=(ForceUnits=cm))
	float BulletTraceWeaponRadius = 0.f;
	
};
