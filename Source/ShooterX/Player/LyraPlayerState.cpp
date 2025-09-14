// LyraPlayerState.cpp


#include "Player/LyraPlayerState.h"
#include "GameMode/LyraExperienceDefinition.h"
#include "GameMode/LyraExperienceManagerComponent.h"
#include "GameMode/LyraGameModeBase.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "AbilitySystem/LyraAbilitySet.h"
#include "Character/LyraPawnData.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraPlayerState)

ALyraPlayerState::ALyraPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<ULyraAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
}

void ALyraPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UWorld* World = GetWorld();
	if (IsValid(World) && World->IsGameWorld() == true && World->GetNetMode() != NM_Client)
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		check(GameState);

		ULyraExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
		check(ExperienceManagerComponent);

		ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnLyraExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
		// 라이라 프로젝트에서 바인드를 PostInitializeComponents()에서 하는 이유는?
		// ULevel::RouteActorInitialize() 함수에서 레벨에 있는 모든 액터를 순회하며 초기화되기 시작함.
		// 이때 따로 순서가 있지 않음. 그래서 플레이어스테이트가 게임모드보다 먼저 초기화될 수 있음.
		// 그래서 그냥 시점을 GameMode::Login() 함수가 호출 된 이후인 PlayerState::PostInitializeComponents()에서 바인드 진행하는 거라고 추측 중.
		// 헤이커님의 생각으론 생성자에서 바인드해도 딱히 문제가 되진 않을거라고 하심.
		// 다만 생성자에서 할 때, 한 가지 걱정은 위에 있는 GameState나 ExperienceManagerComponent 지역 변수들이 정상적으로 접근 가능하겠느냐의 문제가 확인 되지 않음.
	}

	check(AbilitySystemComponent);
	// 아래의 코드는 우리가 InitAbilityActorInfo를 재호출을 통하는 이유를 설명하는 코드.
	{
		// 처음 InitAbilityActorInfo를 호출하는 당시에 OwnerActor와 AvatarActor가 같은 Actor를 가리키고 있음. 이는 PlayerState 액터.
		// - OwnerActor는 PlayerState 액터를 가르키는게 맞지만, AvatarActor는 PlayerController가 소유하고 있는 폰을 가리키는게 맞음.
		// - 지금은 GetPawn()의 반환값이 nullptr임. 나중에 다시 세팅해줌.
		FGameplayAbilityActorInfo* ActorInfo = AbilitySystemComponent->AbilityActorInfo.Get();
		check(ActorInfo->OwnerActor == this);
		check(ActorInfo->OwnerActor == ActorInfo->AvatarActor);
	}
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());
}

void ALyraPlayerState::OnExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience)
{
	ALyraGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ALyraGameModeBase>();
	if (true == ::IsValid(GameMode))
	{
		// ALyraGameMode에서 GetPawnDataForController()를 구현해야 함.
		// - GetPawnDataForController() 함수에서 우리는 아직 PawnData를 설정하지 않았으므로, ExperienceManagerComponent의 DefaultPawnData로 설정함.
		const ULyraPawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(nullptr != NewPawnData);

		SetPawnData(NewPawnData);
	}
}

void ALyraPlayerState::SetPawnData(const ULyraPawnData* InPawnData)
{
	check(InPawnData);

	// PawnData가 두 번 설정되는 것은 원하지 않음.
	check(nullptr == PawnData);

	PawnData = InPawnData;

	// PawnData가 AbilitySet을 순회하며 ASC에 어빌리티를 부여한다.
	// - 이 과정에서 ASC의 ActivatableAbilities에 추가됨.
	for (ULyraAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}
}
