// LyraGameModeBase.cpp


#include "GameMode/LyraGameModeBase.h"
#include "GameMode/LyraGameStateBase.h"
#include "Player/LyraPlayerController.h"
#include "Player/LyraPlayerState.h"
#include "Character/LyraCharacterBase.h"
#include "LyraExperienceManagerComponent.h"

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
	// 여기서 익스피리언스의 로딩이 진행될 예정.
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
