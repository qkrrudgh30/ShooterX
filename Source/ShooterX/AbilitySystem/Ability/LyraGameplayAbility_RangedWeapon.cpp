// LyraGameplayAbility_RangedWeapon.cpp

#include "LyraGameplayAbility_RangedWeapon.h"
#include "Equipment/Weapon/LyraRangedWeaponInstance.h"
#include "Physics/LyraCollisionChannel.h"
#include "AbilitySystem/TargetData/LyraGameplayAbilityTargetData_SingleTargetHit.h"
#include "AbilitySystemComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraGameplayAbility_RangedWeapon)

ULyraGameplayAbility_RangedWeapon::ULyraGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

ULyraRangedWeaponInstance* ULyraGameplayAbility_RangedWeapon::GetWeaponInstance()
{
	return Cast<ULyraRangedWeaponInstance>(GetAssociatedEquipment());
}

void ULyraGameplayAbility_RangedWeapon::StartRangedWeaponTargeting()
{
	// ActorInfo는 어빌리티셋에서 GiveAbility() 함수의 호출로 설정됨.
	// - UGameplayAbility::OnGiveAbility() 함수에서 SetCurrentActorInfo() 함수가 호출되며 설정됨.
	// - AbilitySystemComponent::GiveAbility() 함수에서 OnGiveAbility() 함수를 호출함.
	// - LyraAbilitySet::GiveToAbiltiySystem() 함수에서 GiveAbility() 함수 호출.
	check(CurrentActorInfo);

	AActor* AvatarActor = CurrentActorInfo->AvatarActor.Get();
	check(AvatarActor);

	UAbilitySystemComponent* MyAbilityComponent = CurrentActorInfo->AbilitySystemComponent.Get();
	check(MyAbilityComponent);

	// 여기서 라이라 프로젝트의 샷건 처리와 같은 탄착 처리를 생략하고 권총으로 진행.

	// 총알의 궤적 Hit 정보를 계산
	TArray<FHitResult> FoundHits;
	PerformLocalTargeting(FoundHits);

	// GameplayAbilityTargetData는 Server/Client 간 Ability의 공유 데이터로 이해하면 됨.
	// - 허나, 우리는 싱글플레이이므로 Ability의 데이터로 생각하면 됨. 현재는큰 의미가 없음.
	FGameplayAbilityTargetDataHandle TargetData;
	TargetData.UniqueId = 0;

	if (0 < FoundHits.Num())
	{
		// Catridge란 일반 권총의 경우, 탄약 하나에 총알 하나. 샷건의 경우 탄약 하나에 여러 총알. 탄약이 카트리지.
		const int32 CartridgeID = FMath::Rand();
		for (const FHitResult& FoundHit : FoundHits)
		{
			// AbilityTargetData에 SingleTargetHit 정보를 담음.
			FLyraGameplayAbilityTargetData_SingleTargetHit* NewTargetData = new FLyraGameplayAbilityTargetData_SingleTargetHit();
			NewTargetData->HitResult = FoundHit;
			NewTargetData->CartridgeID = CartridgeID;
			TargetData.Add(NewTargetData);
		}
	}

	// 가공된 AbilityTargetData가 준비되었음. OnTargetDataReadyCallback() 함수 호출.
	OnTargetDataReadyCallback(TargetData, FGameplayTag());

}

void ULyraGameplayAbility_RangedWeapon::PerformLocalTargeting(TArray<FHitResult>& OutHits)
{
	APawn* const AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());

	ULyraRangedWeaponInstance* WeaponData = GetWeaponInstance();
	if (AvatarPawn && AvatarPawn->IsLocallyControlled() && WeaponData)
	{
		FRangedWeaponFiringInput InputData;
		InputData.WeaponData = WeaponData;
		InputData.bCanPlayBulletFX = true;

		const FTransform TargetTransform = GetTargetingTransform(AvatarPawn, ELyraAbilityTargetingSource::CameraTowardsFocus);
		// 언리얼은 ForwardVector가 (1, 0, 0)임. EAxis::X
		// - GetUnitAxis()를 살펴보자.
		InputData.AimDir = TargetTransform.GetUnitAxis(EAxis::X);
		InputData.StartTrace = TargetTransform.GetTranslation();
		InputData.EndAim = InputData.StartTrace + InputData.AimDir * WeaponData->MaxDamageRange;

#if 1
		{
			static float DebugThickness = 2.f;
			DrawDebugLine(GetWorld(), InputData.StartTrace, InputData.StartTrace + (InputData.AimDir * 100.f), FColor::Yellow, false, 10.f, 0, DebugThickness);
		}
#endif

		TraceBulletsInCartridge(InputData, OutHits);
	}
}

FTransform ULyraGameplayAbility_RangedWeapon::GetTargetingTransform(APawn* SourcePawn, ELyraAbilityTargetingSource Source)
{
	check(SourcePawn);
	check(Source == ELyraAbilityTargetingSource::CameraTowardsFocus);

	// 참고로 아래 로직은 CameraTowardsFocus만 추출한 로직.
	// - 완전한 로직은 라이라 프로젝트 참고.

	AController* Controller = SourcePawn->Controller;
	if (Controller == nullptr)
	{
		return FTransform();
	}

	double FocalDistance = 1024.f;
	FVector FocalLoc;
	FVector CamLoc;
	FRotator CamRot;

	// PlayerController로부터 Location과 Rotation 정보를 가져옴.
	APlayerController* PC = Cast<APlayerController>(Controller);
	check(PC);
	PC->GetPlayerViewPoint(CamLoc, CamRot);

	FVector AimDir = CamRot.Vector().GetSafeNormal();
	FocalLoc = CamLoc + (AimDir * FocalDistance);

	// WeaponLoc이 아닌 Pawn의 Loc
	const FVector WeaponLoc = GetWeaponTargetingSourceLocation();
	FVector FinalCamLoc = FocalLoc + (((WeaponLoc - FocalLoc) | AimDir) * AimDir);

#if 1
	{
		//WeaponLoc(사실상 ActorLoc)
		DrawDebugPoint(GetWorld(), WeaponLoc, 10.f, FColor::Red, false, 60.f);

		DrawDebugPoint(GetWorld(), CamLoc, 10.f, FColor::Yellow, false, 60.f);

		DrawDebugPoint(GetWorld(), FinalCamLoc, 10.f, FColor::Magenta, false, 60.f);

		// (WeaponLoc - FocalLoc)
		DrawDebugLine(GetWorld(), FocalLoc, WeaponLoc, FColor::Yellow, false, 60.f, 0, 2.f);

		// AimDir
		DrawDebugLine(GetWorld(), CamLoc, FocalLoc, FColor::Blue, false, 60.f, 0, 2.f);

		// Project Direction Line
		DrawDebugLine(GetWorld(), WeaponLoc, FinalCamLoc, FColor::Red, false, 60.f, 0, 2.f);
	}
#endif
	
	// Camera -> Focus 계산 완료
	return FTransform(CamRot, FinalCamLoc);
}

FVector ULyraGameplayAbility_RangedWeapon::GetWeaponTargetingSourceLocation() const
{
	// 미구현인듯 함. Weapon 위치가 아닌 그냥 Pawn의 위치를 가져옴.
	APawn* const AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	check(AvatarPawn);

	const FVector SourceLoc = AvatarPawn->GetActorLocation();
	return SourceLoc;
}

void ULyraGameplayAbility_RangedWeapon::TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData,
	TArray<FHitResult>& OutHits)
{
	ULyraRangedWeaponInstance* WeaponData = InputData.WeaponData;
	check(WeaponData);

	// MaxDamageRange를 고려하여 EndTrace를 정의.
	const FVector BulletDir = InputData.AimDir;
	const FVector EndTrace = InputData.StartTrace + (BulletDir * WeaponData->MaxDamageRange);

	// HitLocation의 초기화 값으로 EndTrace로 설정.
	FVector HitLocation = EndTrace;

	// 총알 하나의 Trace 수행
	// - 참고로 라이라 프로젝트의 경우 샷건과 같은 Cartridge에 여러 개의 총알이 있을 경우를 처리하기 위해 for를 활용하여 복수개의 Bullet을 Trace.
	TArray<FHitResult> AllImpacts;
	FHitResult Impact = DoSingleBulletTrace(InputData.StartTrace, EndTrace, WeaponData->BulletTraceWeaponRadius, /*bIsSimulated=*/false, /*out*/AllImpacts);

	const AActor* HitActor = Impact.GetActor();
	if (HitActor)
	{
		if (0 < AllImpacts.Num())
		{
			OutHits.Append(AllImpacts);
		}

		HitLocation = Impact.ImpactPoint;
	}

	// OutHits가 적어도 하나가 존재하도록 EndTrace를 활용하여 OutHits에 추가해줌.
	if (OutHits.Num() == 0)
	{
		if (!Impact.bBlockingHit)
		{
			Impact.Location = EndTrace;
			Impact.ImpactPoint = EndTrace;
		}

		OutHits.Add(Impact);
	}
}

FHitResult ULyraGameplayAbility_RangedWeapon::DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace,
	float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHits)
{
	FHitResult Impact;

	// 우선 SweepRadius 없이 한 번 Trace 수행. SweepTrace는 무겁기 때문.
	// - FindFirstPawnHitResult() 함수를 여러 번 Trace 수행되는 것을 막기 위해 OutHits를 확인해서 APawn 충돌 정보가 있으면 더이상 Trace 하지 않음.
	if (FindFirstPawnHitResult(OutHits) == INDEX_NONE)
	{
		Impact = WeaponTrace(StartTrace, EndTrace, /*SweepRadius=*/0.f, bIsSimulated, /*out*/ OutHits);
	}
	
	if (FindFirstPawnHitResult(OutHits) == INDEX_NONE)
	{
		// 만약 SweepRadius가 0보다 크면 0.0일때 대비 충돌 가능성이 커지므로 한 번 더 Trace 수행
		if (0.f < SweepRadius)
		{
			// SweepHits에 Trace의 OutHits 정보를 저장.
			TArray<FHitResult> SweepHits;
			Impact = WeaponTrace(StartTrace, EndTrace, SweepRadius, bIsSimulated, SweepHits);

			// Sphere Trace로 진행한 결과인 SweepHits를 검색하여 Pawn이 있는가 검색.
			const int32 FirstPawnIdx = FindFirstPawnHitResult(SweepHits);
			if (SweepHits.IsValidIndex(FirstPawnIdx))
			{
				// 만약 있다면, SweepHits를 FirstPawnIdx까지 순회하며 bBlockingHit과 기존 OutHits에 없을 경우 체크.
				bool bUseSweepHits = true;
				for (int32 Idx = 0; Idx < FirstPawnIdx; ++Idx)
				{
					const FHitResult& CurHitResult = SweepHits[Idx];

					auto Pred = [&CurHitResult](const FHitResult& Other)
					{
						return Other.HitObjectHandle == CurHitResult.HitObjectHandle;
					};

					// OutHits에 뭔가 있다면 SweepHits를 OutHits로 업데이트 하지 않음. 이미 충돌했던 정보가 있으니까? Early-Exit용.
					// - OutHits에 bBlockingHits가 SweepHits로 있음을 알게되었음.
					if (CurHitResult.bBlockingHit && OutHits.ContainsByPredicate(Pred))
					{
						bUseSweepHits = false;
						break;
					}
				}

				// SweepHits
				if (bUseSweepHits)
				{
					OutHits = SweepHits;
				}
			}
		}
	}

	return Impact;
}

int32 ULyraGameplayAbility_RangedWeapon::FindFirstPawnHitResult(const TArray<FHitResult>& HitResults)
{
	for (int32 Idx = 0; Idx < HitResults.Num(); ++Idx)
	{
		const FHitResult& CurHitResult = HitResults[Idx];
		if (CurHitResult.HitObjectHandle.DoesRepresentClass(APawn::StaticClass()))
		{
			return Idx;
		}
		else
		{
			AActor* HitActor = CurHitResult.HitObjectHandle.FetchActor();

			// 한 단계 AttachParent() 함수에 Actor가 Pawn이라면?
			// - 보통 복수 개 단계로 AttachParent() 함수를 호출하지 않으므로, AttachParent() 함수 대상이 APawn이라고 생각할 수도 있음.
			if ((HitActor != nullptr) && (HitActor->GetAttachParentActor() != nullptr) && (Cast<APawn>(HitActor->GetAttachParentActor()) != nullptr))
			{
				return Idx;
			}
		}
	}

	return INDEX_NONE;
}

FHitResult ULyraGameplayAbility_RangedWeapon::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace,
	float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHitResults)
{
	TArray<FHitResult> HitResults;

	// Complext Geometry로 Trace를 진행. AvatarActor를 AttachParent로 가지는 오브젝트와의 충돌은 무시.
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), /*bTraceComplex*/true, /*IgnoredActor*/GetAvatarActorFromActorInfo());
	TraceParams.bReturnPhysicalMaterial = true;

	// AvatarActor에 부착된 Actors를 찾아 IgnoredActors에 추가.
	AddAdditionalTraceIgnoreActors(TraceParams);

	// Weapon 관련 Collision Channel로 Trace 진행.
	const ECollisionChannel TraceChannel = DetermineTraceChannel(TraceParams, bIsSimulated);
	if (0.f < SweepRadius)
	{
		GetWorld()->SweepMultiByChannel(HitResults, StartTrace, EndTrace, FQuat::Identity, TraceChannel, FCollisionShape::MakeSphere(SweepRadius), TraceParams);
	}
	else
	{
		GetWorld()->LineTraceMultiByChannel(HitResults, StartTrace, EndTrace, TraceChannel, TraceParams);
	}

	FHitResult Hit(ForceInit);
	if (0 < HitResults.Num())
	{
		// HitResults 중에 중복 Object의 HitResult 정보 제거
		for (FHitResult& CurHitResult : HitResults)
		{
			auto Pred = [&CurHitResult](const FHitResult& Other)
			{
				return Other.HitObjectHandle == CurHitResult.HitObjectHandle;
			};

			if (!OutHitResults.ContainsByPredicate(Pred))
			{
				OutHitResults.Add(CurHitResult);
			}
		}

		// Hit의 가장 마지막 값을 Impact로 저장.
		Hit = OutHitResults.Last();
	}
	else
	{
		// Hit의 결과 값을 기본 값으로 캐싱
		Hit.TraceStart = StartTrace;
		Hit.TraceEnd = EndTrace;
	}

	return Hit;
}

void ULyraGameplayAbility_RangedWeapon::AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParams) const
{
	if (AActor* Avatar = GetAvatarActorFromActorInfo())
	{
		TArray<AActor*> AttachedActors;

		// GetAttachedActors() 함수를 살펴보자.
		// - 해당 함수는 Recursively하게 모든 Actors를 추출.
		// - 근데 앞서 FindFirstPawnHitResult() 함수는 왜 한단계만 순회했는지 의문.
		Avatar->GetAttachedActors(AttachedActors);

		TraceParams.AddIgnoredActors(AttachedActors);
	}
}

ECollisionChannel ULyraGameplayAbility_RangedWeapon::DetermineTraceChannel(FCollisionQueryParams& TraceParams,
	bool bIsSimulated) const
{
	return Lyra_TraceChannel_Weapon;
}

void ULyraGameplayAbility_RangedWeapon::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData,
	FGameplayTag ApplicationTag)
{
	UAbilitySystemComponent* MyAbilitySystemComponent = CurrentActorInfo->AbilitySystemComponent.Get();
	check(MyAbilitySystemComponent);

	if (const FGameplayAbilitySpec* AbilitySpec = MyAbilitySystemComponent->FindAbilitySpecFromHandle(CurrentSpecHandle))
	{
		// 현재 Stack의 InData에서 지금 Local로 OwnerShip을 가져온다.
		FGameplayAbilityTargetDataHandle LocalTargetDataHandle(MoveTemp(const_cast<FGameplayAbilityTargetDataHandle&>(InData)));

		// CommitAbility() 함수 호출로 GameplayEffect를 처리함.
		// - 현재 아직 우리는 GameplayEffect에 대해 처리를 진행하지 않을 것.
		if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
		{
			// OnRangeWeaponTargetDataReady BP 노드 호출.
			// - 후일 여기서 우리는 GCN(GameplayCueNotify)를 처리할 것.
			OnRangeWeaponTargetDataReady(LocalTargetDataHandle);
		}
		else
		{
			// CommitAbility() 함수 호출에 실패 했다면 EndAbility BP Node 호출.
			K2_EndAbility();
		}
	}
}