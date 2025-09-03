// LyraGameModeBase.cpp


#include "GameMode/LyraGameModeBase.h"
#include "GameMode/LyraGameStateBase.h"
#include "Player/LyraPlayerController.h"
#include "Player/LyraPlayerState.h"
#include "Character/LyraCharacterBase.h"
#include "LyraExperienceManagerComponent.h"
#include "ShooterX.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraGameModeBase)

ALyraGameModeBase::ALyraGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = ALyraGameStateBase::StaticClass();
	PlayerControllerClass = ALyraPlayerController::StaticClass();
	PlayerStateClass = ALyraPlayerState::StaticClass();
	DefaultPawnClass = ALyraCharacterBase::StaticClass();
}

void ALyraGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
	// GameInstance::InitializeActorsForPlay()를 통해 아직 초기화 작업이 진행 중.
	// 현 프레임에서는 라이라 프로젝트의 컨셉인 Experience 처리를 할 수 없음. 게임스테이트도 없고 아무 액터가 없음.
	// 이를 처리하기 위해, 한 프레임 뒤에 HandleMatchAssignmentNotExpectingOne() 함수가 호출되게끔 타이머 처리.
}

void ALyraGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
	// 다음 프레임에 수행되는 HandleMatchAssignmentIfNotExpectingOne() 함수 
	// 이전 프레임에서 GameInstance가 관리하는 매니저들을 초기화가 끝난 뒤에
	// Experience의 초기화를 진행하기 위해 다음 프레임에 위 함수를 바인드함.
	// 위 함수는 Experience 로딩의 시작점.

	// 현재 매칭에 필요한 Experience를 로딩하는 과정에 있음.
	// 이 함수에서는 우리가 로딩할 Experience에 대한 PrimaryAssetId를 생성하여, OnMatchAssignmentGiven() 함수로 넘겨줌.
	FPrimaryAssetId ExperienceId;

	// precedence order (highest wins)
	// - matchmaking assignment (if present) -> 포탈에 폰이 접근하는 로직 구현 시에 사용 예정.
	// - default experience

	UWorld* World = GetWorld();

	// fall back to the default experience
	// 일단 기본 옵션으로 B_DefaultExperience로 설정.
	if (false == ExperienceId.IsValid())
	{
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("LyraExperienceDefinition"), FName("B_DefaultExperience"));
	}

	// 필자가 이해한 HandleMatchAssignmentIfNotExpectingOne() 함수와 OnMatchAssignmentGiven() 함수는 아직 직관적으로 이름이 와닿지 않는다고 생각.
	// - 후일 어느정도 Lyra가 구현되면, 해당 함수의 이름을 더 이해할 수 있을 것으로 예상.
	// - 라이라는 기본적으로 멀티플레이 기반. 매치와 Experience 개념을 염두해 두고 함수 명을 짜는듯함.
	OnMatchAssignmentGiven(ExperienceId);
}

void ALyraGameModeBase::InitGameState()
{
	Super::InitGameState();

	ULyraExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	check(ExperienceManagerComponent);
	// Experience 비동기 로딩을 위한 Delegate를 준비함.

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnLyraExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void ALyraGameModeBase::OnExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience)
{
}

APawn* ALyraGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer,
	const FTransform& SpawnTransform)
{
	UE_LOG(LogSX, Log, TEXT("ALyraGameModeBase::SpawnDefaultPawnAtTransform_Implementation() has been called."));
	
	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
}

bool ALyraGameModeBase::IsExperienceLoaded() const
{
	check(GameState);

	ULyraExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	return ExperienceManagerComponent->IsExperienceLoaded();
}

void ALyraGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (true == IsExperienceLoaded())
	{
		// Experience가 로드되어 있지 않으면 Pawn이 스폰되는 것도 막음.
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

void ALyraGameModeBase::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId)
{
	// 해당 함수는 ExperienceManagerComponent를 활용하여 Experience를 로딩하기 위해
	// ExperienceManagerComponent의 SetCurrentExperience를 호출함.

	check(true == ExperienceId.IsValid());

	ULyraExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	check(true == ::IsValid(ExperienceManagerComponent));

	ExperienceManagerComponent->SetCurrentExperience(ExperienceId);
	// 게임모드에서 익스피리언스 로딩을 담당하지 않음을 다시 한 번 보여주는 코드.
	// 로드하고자하는 익스피리언스 아이디를 익스피리언스 매니저에게 넘겨서 로딩을 요청하는 모습.
}

