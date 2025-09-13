// AsyncAction_ExperienceReady.cpp

#include "AsyncAction_ExperienceReady.h"
#include "LyraExperienceManagerComponent.h"
#include "LyraExperienceDefinition.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(AsyncAction_ExperienceReady)

UAsyncAction_ExperienceReady::UAsyncAction_ExperienceReady(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

UAsyncAction_ExperienceReady* UAsyncAction_ExperienceReady::WaitForExperienceReady(UObject* WorldContextObject)
{
	UAsyncAction_ExperienceReady* Action = nullptr;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		Action = NewObject<UAsyncAction_ExperienceReady>();
		Action->WorldPtr = World;
		Action->RegisterWithGameInstance(World);
	}
	return Action;
}

void UAsyncAction_ExperienceReady::Activate()
{
	if (UWorld* World = WorldPtr.Get())
	{
		// GameState가 이미 World에 준비되어 있다면 Step1을 스킵하고 바로 Step2를 진행
		if (AGameStateBase* GameState = World->GetGameState())
		{
			Step2_ListenToExperienceLoading(GameState);
		}
		else
		{
			// 아직 준비되어 있지 않으면, UWorld::GameStateSetEvent() 함수에 Step1_HandleGameStateSet() 함수를 Delegate로 바인딩.
			// - Step1부터 진행하게끔 유도.
			World->GameStateSetEvent.AddUObject(this, &ThisClass::Step1_HandleGameStateSet);
		}
	}
	else
	{
		// 현재 AsyncAction 대상이었던 World가 더이상 유효하지 않으므로 제거 진행
		// - 코드를 타고 들어가다보면 GameInstance에서 레퍼런스 대상으로 제거시킴.
		// - UAsyncAction의 유일한 캐싱 위치가 GameInstance임을 알 수 있음.
		SetReadyToDestroy();
	}
}

void UAsyncAction_ExperienceReady::Step1_HandleGameStateSet(AGameStateBase* GameState)
{
	if (UWorld* World = WorldPtr.Get())
	{
		World->GameStateSetEvent.RemoveAll(this);
	}

	Step2_ListenToExperienceLoading(GameState);
}

void UAsyncAction_ExperienceReady::Step2_ListenToExperienceLoading(AGameStateBase* GameState)
{
	check(GameState);

	// 여기서 우리가 왜 GameStateSetEvent에 Delegate로 바인딩 시켰는지 이유가 나옴.
	// - Experience 로딩 상태 정보가 GameState에 붙어 있는 ExperienceManagerComponent에서 가져올 수 있기 때문.
	ULyraExperienceManagerComponent* ExperienceManangerComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	check(ExperienceManangerComponent);

	// 만약 이미 Experience가 준비되어 있다면 Step3를 스킵하고 Step4로 이동.
	if (ExperienceManangerComponent->IsExperienceLoaded())
	{
		UWorld* World = GameState->GetWorld();
		check(World);

		// 이미 Experience가 준비되었다고 해도, 아직 준비되지 않은 상태가 있을 수도 있으니(혹은 진행중이거나) 그래서 그냥 다음 Tick에 실행되게끔 Timer Set
		World->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &ThisClass::Step4_BroadcastReady));
	}
	else
	{
		// 준비가 안되었다면, OnExperienceLoaded에 바인딩시킴. 후에 로딩이 끝나면 Step3부터 실행 될 예정.
		ExperienceManangerComponent->CallOrRegister_OnExperienceLoaded(FOnLyraExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::Step3_HandleExperienceLoaded));
	}
}

void UAsyncAction_ExperienceReady::Step3_HandleExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience)
{
	// 현재 따로 ExperienceDefinition에 대해 처리할 로직은 없음.
	Step4_BroadcastReady();
}

void UAsyncAction_ExperienceReady::Step4_BroadcastReady()
{
	// 바인딩된 BP 혹은 UFUNCTION을 호출.
	OnReady.Broadcast();
	SetReadyToDestroy();
}