// LyraHeroComponent.cpp


#include "LyraHeroComponent.h"
#include "ShooterX.h"
#include "LyraPawnExtensionComponent.h"
#include "LyraGameplayTags.h"
#include "Player/LyraPlayerState.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Character/LyraPawnData.h"
#include "Camera/LyraCameraComponent.h"
#include "Player/LyraPlayerController.h"
#include "Input/LyraEnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

const FName ULyraHeroComponent::NAME_ActorFeatureName("Hero");
const FName ULyraHeroComponent::NAME_BindInputsNow("BindInputsNow");
// InputConfig의 GameFeatureAction 활성화 ExtensionEvent 이름.

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

		if (ALyraPlayerController* PC = GetController<ALyraPlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
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

void ULyraHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (nullptr == Pawn)
	{
		return;
	}

	// LocalPlayer를 가져오기 위해
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	// EnhancedInputLocalPlayerSubsystem을 가져오기 위해
	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	// EnhancedInputLocalPlayerSubsystem에 MappingContext를 비워줌.
	Subsystem->ClearAllMappings();

	if (const ULyraPawnExtensionComponent* PawnExtComp = ULyraPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const ULyraPawnData* PawnData = PawnExtComp->GetPawnData<ULyraPawnData>())
		{
			if (const ULyraInputConfig* InputConfig = PawnData->InputConfig)
			{
				const FLyraGameplayTags& GameplayTags = FLyraGameplayTags::Get();

				FModifyContextOptions Options = {};
				Options.bIgnoreAllPressedKeysUntilRelease = false;

				// 내부적으로 Input Mapping Context를 추가.
				Subsystem->AddMappingContext(InputMappingContext.LoadSynchronous(), 0, Options);

				ULyraEnhancedInputComponent* IC = CastChecked<ULyraEnhancedInputComponent>(PlayerInputComponent);
				{
					// InputTag_Move와 InputTag_Look_Mouse에 대해 각각 Input_Move()와 Input_LookMouse() 멤버 함수에 바인딩시킴.
					// - 바인딩한 이후, Input 이벤트에 따라 멤버 함수가 트리거됨.
					IC->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, false);
					IC->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, false);
				}
			}
		}
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
	// GameFeatureAction_AddInputConfig의 HandlePawnExtension 콜백 함수 전달	
	// 특이하게 Receiver(여기선 Pawn)를 지정해줄 수있음. GFA_AddInputConfig::AddToWorld() 코드를 참고해보자.
}

void ULyraHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

		if (Value.X != 0.f)
		{
			// Left/Right -> X 값에 들어있음.
			// MovementDirection은 현재 카메라 RightVector를 의미함 (World-Space)
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);

			// AddMovementInput() 함수를 살펴보자
			// - 내부적으로 MovementDirection * Value.X를 MovementComponent에 적용(더하기) 해줌.
			Pawn->AddMovementInput(MovementDirection, Value.X);

		}

		if (Value.Y != 0.f) // 앞서 우리는 Forward 적용을 위해 Swizzle Input Modifier를 사용했음.
		{
			// 앞서 Left/Right와 마찬가지로 Forward/Backward를 적용함.
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void ULyraHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	if (nullptr == Pawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();
	if (Value.X != 0.f)
	{
		// X에는 Yaw 값이 있음
		// - Camera에 대해 Yaw 적용
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.f)
	{
		// Y에는 Pitch 값
		double AimInversionValue = -Value.Y;
		Pawn->AddControllerPitchInput(AimInversionValue);
	}
}
