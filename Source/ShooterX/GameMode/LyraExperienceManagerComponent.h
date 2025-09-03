// LyraExperienceManagerComponent.h

#pragma once

#include "Components/GameStateComponent.h"
#include "LyraExperienceManagerComponent.generated.h"

class ULyraExperienceDefinition;

enum class ELyraExperienceLoadState
{
	Unloaded,
	Loading,
	Loaded,
	Deactivating,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLyraExperienceLoaded, const ULyraExperienceDefinition*);

/**
* ExperienceManagerComponent
*	해당 컴포넌트는 게임스테이트를 Owner로 가지면서 Experience의 상태 정보를 가지고 있는 컴포넌트.
*	뿐만 아니라, Manager 라는 단어가 포함되어 있듯, Experience 로딩 상태 업데이트 및 이벤트를 관리함.
*/

UCLASS()
class SHOOTERX_API ULyraExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	ULyraExperienceManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	bool IsExperienceLoaded() { return (LoadState == ELyraExperienceLoadState::Loaded) && (nullptr != CurrentExperience); }

	void CallOrRegister_OnExperienceLoaded(FOnLyraExperienceLoaded::FDelegate&& Delegate);
	// 이미 Experience 로딩이 완료되었다면 바로 호출되고, 아니면 OnExperienceLoaded에 바인딩 됨.

public:
	UPROPERTY()
	TObjectPtr<const ULyraExperienceDefinition> CurrentExperience;
	// 라이라 프로젝트에서는 Replicated 속성임. 이번 강의에서는 레플리케이션을 고려하지 않고 진행.

	ELyraExperienceLoadState LoadState = ELyraExperienceLoadState::Unloaded;
	// Experience의 로딩 상태 모니터링.

	FOnLyraExperienceLoaded OnExperienceLoaded;
	// Experience 로딩이 완료된 이후 Broadcasting Delegate.

};