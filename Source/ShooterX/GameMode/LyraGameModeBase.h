// LyraGameModeBase.h

#pragma once

#include "GameFramework/GameModeBase.h"
#include "LyraGameModeBase.generated.h"

class ULyraExperienceDefinition;

/**
 * 
 */
UCLASS()
class SHOOTERX_API ALyraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALyraGameModeBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) final;

	void HandleMatchAssignmentIfNotExpectingOne();

	virtual void InitGameState() final;

	void OnExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience);

	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) final;
	// 일반적인 게임모드에서는 이 함수를 폰을 스폰 시킬 때 사용함.
	// GameModeBase에 UFUNCTION(BlueprintNativeEvent) 매크로가 붙어있는 함수임.
	// 우리는 C++로 함수를 구현할 예정이지만, 필요하다면 블루프린트에서 재정의가 가능하다는 의미.
	// UFUNCTION(BlueprintNativeEvent)
	// void Func()
	// {
	//     if (블루프린트에서 오버라이딩 되었다면) { 블루프린트 함수 호출 }
	//     else { Func_Implementation(); }
	// }
	// 우리는 이 함수를 폰이 스폰 되었는지 안되었는지 트래킹 하기 위해 사용.

	bool IsExperienceLoaded() const;
	
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) final;
	// 이 함수는 특정 상태에 따라 RestartPlayer()를 호출하거나 호출하지 않거나를 결정.
	// 우린 OnExperienceLoaded()에서 강제로 RestartPlayerAtPlayerStart()를 호출하고자 함.
	// RestartPlayer() 함수는 RestartPlayerAtPlayerStart() 함수를 한번 Wrapping한 함수.
	//  1. HandleStartingNewPlayer_Implementation() 함수를 Ctrl + 클릭해서 타고 들어감
	//  2. RestartPlayer() 함수가 if문에 따라 호출될지 말지 결정됨을 확인.
	//    다시 RestartPlayer() 함수를 Ctrl + 클릭해서 타고 들어감.
	//  3. RestartPlayerAtPlayerStart() 함수가 호출됨을 확인.

	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId);
	// ExperienceId를 넘겨 받는 MatchAssignmentGiven() 함수
	// 위 함수도 직접적인 초기화를 수행하진 않음. 
	// SetCurrentExperience() 함수에 넘겨받은 ExperienceId를 다시 넘겨주는 함수.
	
};
