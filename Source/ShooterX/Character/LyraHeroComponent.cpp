// LyraHeroComponent.cpp


#include "LyraHeroComponent.h"
#include "ShooterX.h"
#include "LyraPawnExtensionComponent.h"
#include "LyraGameplayTags.h"
#include "Player/LyraPlayerState.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Character/LyraPawnData.h"
#include "Camera/LyraCameraComponent.h"

const FName ULyraHeroComponent::NAME_ActorFeatureName("Hero");

ULyraHeroComponent::ULyraHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void ULyraHeroComponent::OnRegister()
{
	Super::OnRegister();

	{
		APawn* OwnerPawn = GetPawn<APawn>();
		if (false == ::IsValid(OwnerPawn))
		{
			UE_LOG(LogSX, Error, TEXT("This component has been added to a BP whose base class is not a APawn."));
			return;
		}
	}

	RegisterInitStateFeature();
}

void ULyraHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	BindOnActorInitStateChanged(ULyraPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);
	// PawnExtensionComponent에 대해서 (PawnExtension Feature) OnActorInitStateChanged() 관찰하도록 (Observing)

	ensure(TryToChangeInitState(FLyraGameplayTags::Get().InitState_Spawned));

	CheckDefaultInitialization();
}

void ULyraHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void ULyraHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	const FLyraGameplayTags& InitTags = FLyraGameplayTags::Get();
	if (Params.FeatureName == ULyraPawnExtensionComponent::NAME_ActorFeatureName)
	{
		// PawnExtensionComponent의 DataInitialized 상태 변화 관찰 후 HeroComponent도 DataInitialized 상태로 변경.
		// CanChangeInitState() 함수로 확인.
		
		if (Params.FeatureState == InitTags.InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

bool ULyraHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	check(Manager);

	const FLyraGameplayTags& Tags = FLyraGameplayTags::Get();
	APawn* Pawn = GetPawn<APawn>();
	ALyraPlayerState* LCPS = GetPlayerState<ALyraPlayerState>();

	// Spawned 초기화
	if (false == CurrentState.IsValid() && DesiredState == Tags.InitState_Spawned)
	{
		if (true == ::IsValid(Pawn))
		{
			return true;
		}
	}

	// Spawned -> DataAavailable
	if (CurrentState == Tags.InitState_Spawned && DesiredState == Tags.InitState_DataAvailable)
	{
		if (false == ::IsValid(LCPS))
		{
			return false;
		}

		return true;
	}

	// DataAvailable -> DataInitialized
	if (CurrentState == Tags.InitState_DataAvailable && DesiredState == Tags.InitState_DataInitialized)
	{
		return (true == ::IsValid(LCPS)) && (true == Manager->HasFeatureReachedInitState(Pawn, ULyraPawnExtensionComponent::NAME_ActorFeatureName, Tags.InitState_DataInitialized));
		// PawnExtensionComponent가 DataInitialized 될 때까지 기다림(== 모든 Feature Component가 DataAvailable인 상태)
	}

	// DataInitialized -> GameplayReady
	if (CurrentState == Tags.InitState_DataInitialized && DesiredState == Tags.InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void ULyraHeroComponent::CheckDefaultInitialization()
{
	// 앞서 BindOnActorInitStateChanged에서 보았듯이 Hero Feature는 Pawn Extension Feature에 종속되어 있으므로, CheckDefaultInitializationForImplementers 호출하지 않음.
	// 즉, 다른 Feature Component들은 관심 없고, PawnExtensionComponent에 맞춰서 움직이겠다는 뜻. 걔가 모든걸 관장하고 있을 테니.

	// ContinueInitStateChain은 앞서 PawnExtComponent와 같음.
	const FLyraGameplayTags& InitTags = FLyraGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };
	ContinueInitStateChain(StateChain);
}

void ULyraHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState)
{
	const FLyraGameplayTags& InitTags = FLyraGameplayTags::Get();

	// DataAvailable -> DataInitialized 단계에서만 실행시킬 예정.
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ALyraPlayerState* LCPS = GetPlayerState<ALyraPlayerState>();
		if (false == ::IsValid(Pawn) || false == ::IsValid(LCPS))
		{
			return;
		}

		// TODO: Input과 Camera에 대한 핸들링

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const ULyraPawnData* PawnData = nullptr;
		if (ULyraPawnExtensionComponent* PawnExtComp = ULyraPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<ULyraPawnData>();
		}

		if (bIsLocallyControlled && PawnData)
		{
			// 현재 LyraCharacterBase에 Attach된 CameraComponent를 찾음
			if (ULyraCameraComponent* CameraComponent = ULyraCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}
	}
}

TSubclassOf<ULyraCameraMode> ULyraHeroComponent::DetermineCameraMode() const
{
	const APawn* Pawn = GetPawn<APawn>();
	if (false == ::IsValid(Pawn))
	{
		return nullptr;
	}

	ULyraPawnExtensionComponent* PawnExtComp = ULyraPawnExtensionComponent::FindPawnExtensionComponent(Pawn);
	if (true == ::IsValid(PawnExtComp))
	{
		const ULyraPawnData* PawnData = PawnExtComp->GetPawnData<ULyraPawnData>();
		if (true == ::IsValid(PawnData))
		{
			return PawnData->DefaultCameraMode;
		}
	}

	return nullptr;
}
