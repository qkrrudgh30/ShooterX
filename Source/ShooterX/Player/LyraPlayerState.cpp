// LyraPlayerState.cpp


#include "Player/LyraPlayerState.h"
#include "GameMode/LyraExperienceDefinition.h"
#include "GameMode/LyraExperienceManagerComponent.h"

ALyraPlayerState::ALyraPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ALyraPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

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

void ALyraPlayerState::OnExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience)
{
	// 추후 여기서 폰데이터를 채워줄 예정.
}
