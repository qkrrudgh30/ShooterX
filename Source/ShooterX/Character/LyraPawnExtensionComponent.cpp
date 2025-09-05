// LyraPawnExtensionComponent.cpp


#include "Character/LyraPawnExtensionComponent.h"
#include "ShooterX.h"
#include "Components/GameFrameworkComponentManager.h"
#include "LyraGameplayTags.h"
#include "Character/LyraPawnData.h"
#include "Net/UnrealNetwork.h"

const FName ULyraPawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");
// feature name을 component 단위니까 component를 빼고 pawn extension만 넣은 것을 확인 할 수 있음.

ULyraPawnExtensionComponent::ULyraPawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 기본적으로 Tick() 함수의 호출을 끔.
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void ULyraPawnExtensionComponent::OnRegister()
{
	Super::OnRegister();

	// 올바른 Actor에 등록되었는지 확인
	{
		APawn* OwnerPawn = GetPawn<APawn>();
		if (false == ::IsValid(OwnerPawn))
		{
			UE_LOG(LogSX, Error, TEXT("This component has been added to a BP whose base class is not a APawn."));
			return;
		}
	}

	RegisterInitStateFeature();

	UGameFrameworkComponentManager* Manager = UGameFrameworkComponentManager::GetForActor(GetOwningActor());
	// 디버깅을 위한 함수
}

void ULyraPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);
	// BindOnActorInitStateChanged() 함수를 사용하면 특정 피쳐 컴포넌트의 특정 InitState 변화를 관찰하고 그에 따른 이벤트 함수 바인드 가능.
	// FeatureName에 NAME_None을 넣으면, Actor에 등록된 "나를 제외한 모든 Feature Component의" [확인필요. 나를 제외하지 않은거 같은데.. 그래서 OnActorInitStateChanged()에서 예외처리하는데..]
	// RequiredState에 FGameplayTag()를 넣으면, "모든 InitState 상태"를 관찰하겠다는 의미
	// bCallIfReached는 바로 호출 할지 말지. false라서 바로 호출 안하고 등록만.

	ensure(TryToChangeInitState(FLyraGameplayTags::Get().InitState_Spawned));
	// InitState_Spawned로 상태 변환
	// - TryToChangeInitState() 함수는 아래와 같이 진행됨.
	// -- 1. CanChangeInitState() 함수로 상태 변환 가능성 유무 판단.
	// -- 2. HandleChangeInitState() 함수로 내부 상태 변경(Feature Component)
	// -- 3. BindOnActorInitStateChanged() 함수로 Bind된 Delegate를 조건에 맞게 호출해줌.
	// --- LyraPawnExtensionComponent의 경우, 모든 Actor의 Feature 상태 변화에 대해 OnActorInitStateChanged() 함수가 호출됨.
	// - TryToChangeInitState() 함수가 호출되면 CurrentInitState는 None에서 InitState_Spawned로 전환됨.

	CheckDefaultInitialization();
	// 해당 함수는 우리가 오버라이딩 함.
	// - 이 함수는 ForceUpdateInitState() 함수와 같은 느낌으로 이해해주면 됨.
	// - 현재 강제 업데이트 진행(물론 CanChangeInitState() 함수와 HandleChangeInitState()를 또 진행해줌.)
	// - TryToChangeInitState()는 한 번 상태 전환 시도. CheckDefaultInitialization() 함수는 전환 될 때까지 계속 전환 시도.
	// -- 그래서 조건이 모두 만족한다면 Spawned - DataAvailable - DataInitialized - GameplayReady가 차례대로 전환됨.
}

void ULyraPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	// 앞서, OnRegister() 함수의 RegisterInitStateFeature() 함수의 쌍을 맞추어주자.

	Super::EndPlay(EndPlayReason);
}

void ULyraPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		// BindOnActorInitStateChanged() 함수에서는 막상 모든 피쳐 컴포넌트의 모든 InitState 상태 전환을 관찰하겠다 하고
		// Params.FeatureName != NAME_ActorFeatureName 조건문을 통해 본인의 InitState 전환은 빼버림.
		// 사실 안빼면 무한루프에 빠질 수 있기도 함.
	
		// PawnExtensionComponent는 다른 Feature Component들의 상태가 DataAvailable인지를 관찰하여, Sync를 맞추는 구간이 있었다. (CanChangeInitState() 함수)
		// 이를 가능케하기 위해, OnActorInitStateChanged()에서는 DataAvailable에 대해 지속적으로 CheckDefaultInitialization을 호출하여 상태를 확인한다.
		
		const FLyraGameplayTags& InitTags = FLyraGameplayTags::Get();
		if (Params.FeatureState == InitTags.InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

bool ULyraPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	//bool bCanChange = IGameFrameworkInitStateInterface::CanChangeInitState(Manager, CurrentState, DesiredState);
	//bCanChange = false; // 일단 지금 당장은 initstate 변경 막기.
	//return bCanChange;

	check(nullptr != Manager);

	APawn* Pawn = GetPawn<APawn>();
	const FLyraGameplayTags& InitTags = FLyraGameplayTags::Get();

	// InitState_Spawned 초기화
	if (false == CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		// Pawn이 잘 세팅만 되어 있으면 바로 Spawned로 넘어감.
		if (true == ::IsValid(Pawn))
		{
			return true;
		}
	}

	// Spawned -> DataAvailable
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		if (false == ::IsValid(PawnData))
		{
			return false;
		}
		// 위 조건문을 미루어볼때, InitState_Spawned에서 InitState_DataAvailable로 가기 전에
		// 다른 클래스에서 ULyraPawnExtensionComponent::PawnData를 채워줘야하는 것으로 예상해볼 수 있다.

		const bool bHasAuthority = Pawn->HasAuthority();
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		if (true == bHasAuthority || true == bIsLocallyControlled)
		{
			if (false == ::IsValid(GetController<AController>()))
			{
				return false;
			}
			// LocallyControlled인 Pawn이 Controller가 없으면 false.
		}

		return true;
		// bIsLocallyControlled가 true여도 컨트롤러만 체크할 뿐. 별다른 로직 없이 return true; 구문으로 내려옴.
		// PawnExtensionComponent에서 bIsLocallyControlled가 true인지 false인지는 중요하지 않다는 걸 알 수 있음.
	}

	// DataAvailable -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// Actor에 바인드된 모든 Feature들(나 자신, 즉 PawnExtensionComponent 포함)이 DataAvailable 상태일 때, DataInitialized로 넘어감.
		// - HaveAllFeaturesReachedInitState() 함수 살펴보기.
		return Manager->HaveAllFeaturesReachedInitState(Pawn, InitTags.InitState_DataAvailable);
	}

	// DataInitialized -> GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	// 위의 선형적인(Linear) transition이 아니라면 false.
	return false;
}

void ULyraPawnExtensionComponent::CheckDefaultInitialization()
{
	CheckDefaultInitializationForImplementers();
	// PawnExtensionComponent는 Feature Component들의 초기화를 관장하는 Component이다.
	// - 따라서 Actor에 바인딩된 Feature Component들에 대해 CheckDefaultInitialization을 호출해주도록 한다.
	//   (ForceUpdate 느낌. ex. HeroComponent와 CameraComponent가 모두 Spawned 상태라면, PawnExtensionComponent가 Hero와 Camera 모두를 DataAvailable 상태로 넘겨버리고자 할때.)
	// - 이 메서드를 IGameFrameworkInitStateInterface가 제공하는데, CheckDefaultInitializationForImplementers() 함수이다.
	// - 간단히 CheckDefaultInitializationForImplemeters() 함수를 살펴보자.
			
	// 이러한 방식의 장점은 종속성이 사라졌다는 것.
	// 액터에 어떤 피쳐가 달려있는지는 PawnExtensionComponent가 몰라도됨. 그냥 그 컴포넌트들을 알아서 업데이트 되게끔 구조가 짜져있음.

	const FLyraGameplayTags& InitTags = FLyraGameplayTags::Get();

	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };
	// 사용자 정의 InitState를 직접 넘겨줘야 함.

	ContinueInitStateChain(StateChain);
	// CanChangeInitState() 함수와 HandleChangeInitState() 함수 그리고 ChangeFeatureInitState() 함수 호출을 통한 OnActorInitStateChanged Delegate 호출까지 진행.
	// 아래의 코드를 간단히 살펴보자.
	// - 해당 코드까지 보면 우리는 간단히 짐작 할 수 있음
	// -- 계속 상태를 가능할 때까지 한 번 업데이트 함.
	// -- InitState에 대한 변화는 Linear(선형적)임. -> 나중에 Diagram으로 정리하면서 보자.
	// --- 업데이트가 멈추면 누군가 시작해줘야 함을 의미함.(Chaining)

	// 결국 ULyraPawnExtensionComponent::CheckDefaultInitialization() 함수는
	// CheckDefaultInitializationForImplementers() 함수를 호출해서 나를 제외한 모든 피쳐의 InitState를 업데이트 하고
	// ContinueInitStateChain() 함수를 호출해서 마지막으로 나도 업데이트.
}

void ULyraPawnExtensionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PawnData);
}

void ULyraPawnExtensionComponent::SetPawnData(const ULyraPawnData* InPawnData)
{
	APawn* Pawn = GetPawnChecked<APawn>();
	if (Pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}
	// Pawn에 대해 Authority가 없을 경우, SetPawnData() 함수는 진행하지 않음.

	if (true == ::IsValid(PawnData))
	{
		return;
	}

	PawnData = InPawnData;
	
	// CheckDefaultInitialization();
	// 라이라에서는 위 함수를 호출하는 코드가 있음. 분석해보면 필요 없는 코드.
	// SetPawnData() 함수는 LCGameModeBase::SpawnDefaultPawnAtTransform_Implementation()에서 호출됨.
	// 이 함수가 호출되는 시점은 ULyraPawnExtensionComponent::BeginPlay() 호출 보다도 훨씬 이전임.
	// BeginPlay()가 호출되지 않았으면 InitState가 None이라서 CheckDefaultInitialization()이 호출되어도 아무런 일이 일어나지 않음. 
}

void ULyraPawnExtensionComponent::OnRep_PawnData()
{
	CheckDefaultInitialization();
}

void ULyraPawnExtensionComponent::SetupPlayerInputComponent()
{
	// ForceUpdate로 다시 InitState 상태 변환 시작(연결 고리)
	CheckDefaultInitialization();
}
