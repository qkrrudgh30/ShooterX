// LyraGameplayAbility_RangedWeapon.h

#pragma once

#include "LyraGameplayAbility_FromEquipment.h"
#include "Engine/EngineTypes.h"
#include "LyraGameplayAbility_RangedWeapon.generated.h"

class ULyraRangedWeaponInstance;

// 어디 대상으로 Target으로 설정할지 옵션들. 라이라 프로젝트의 경우엔 다양한 옵션이 있음.
UENUM(BlueprintType)
enum class ELyraAbilityTargetingSource : uint8
{
	// Camera 기준으로 진행
	CameraTowardsFocus,
};

struct FRangedWeaponFiringInput
{
	FVector StartTrace;
	FVector EndAim;
	FVector AimDir;
	ULyraRangedWeaponInstance* WeaponData = nullptr;
	bool bCanPlayBulletFX = false;

	FRangedWeaponFiringInput()
		: StartTrace(ForceInitToZero)
		, EndAim(ForceInitToZero)
		, AimDir(ForceInitToZero)
	{

	}
};

UCLASS()
class SHOOTERX_API ULyraGameplayAbility_RangedWeapon : public ULyraGameplayAbility_FromEquipment
{
	GENERATED_BODY()

public:
	ULyraGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	ULyraRangedWeaponInstance* GetWeaponInstance();

	UFUNCTION(BlueprintCallable)
	void StartRangedWeaponTargeting();
	// 초기화 및 타게팅 시작

	void PerformLocalTargeting(TArray<FHitResult>& OutHits);
	// 로컬 타게팅 수행

	FTransform GetTargetingTransform(APawn* SourcePawn, ELyraAbilityTargetingSource Source);
	// 타게팅 트랜스폼 계산

	FVector GetWeaponTargetingSourceLocation() const;
	// 무기 위치 반환(카메라로부터 포커스 계산)

	void TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData, TArray<FHitResult>& OutHits);
	// 총알 트레이스 수행
	
	FHitResult DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHits);
	// 총알 한 발에 대한 트레이스 수행

	int32 FindFirstPawnHitResult(const TArray<FHitResult>& HitResults);
	// 피격 대상이 Pawn인지 판별

	FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHitResults);
	// 실제 트레이스 수행 (Line/Sweep)

	void AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParams) const;
	// 트레이스 충돌 제외 대상 설정

	ECollisionChannel DetermineTraceChannel(FCollisionQueryParams& TraceParams, bool bIsSimulated) const;
	// 트레이스 채널 결정

	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);
	// 최종 타겟 데이터 처리 콜백

	UFUNCTION(BlueprintImplementableEvent)
	void OnRangeWeaponTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);

};
