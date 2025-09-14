// LyraGameplayFeaturePolicy.h

#pragma once

#include "GameFeaturesProjectPolicies.h"
#include "GameFeatureStateChangeObserver.h"
#include "LyraGameplayFeaturePolicy.generated.h"

UCLASS()
class SHOOTERX_API ULyraGameplayFeaturePolicy : public UDefaultGameFeaturesProjectPolicies
{
	GENERATED_BODY()

public:
	ULyraGameplayFeaturePolicy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitGameFeatureManager() override;
	
	virtual void ShutdownGameFeatureManager() override;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UObject>> Observers;
	// Observer로서 등록한 개체를 관리(아래 LyraGameFeature_AddGameplayCuePaths가 하나의 예시)

};

/*
*	GameFeature 플러그인이 Register / Unregister 단계에서 GameplayCuePath가 등록되어 GameplayCueManager가 관련 GameplayCue 애셋을 스캔할 수 있도록 해야함.
*	이를 위해 GameFeatureStateChangeObserver를 활용하여 옵저버 패턴으로 이를 가능하게 하도록 함.
*	아래에 우리가 Override 할 메서드에서 유추할 수 있다시피 GameFeatureStateChangeObserver를 활용하면 가능함.
*	이를 위해 추가적 구현이 필요한데, 이는 ULyraGameplayFeaturePolicy를 참고하자.
*	
*	참고로 GameFeatureAction에도 Registering / Unregistering이 있긴하다.
*	허나 우리가 앞서 구현했다시피, 직접 Registering / Unregistering 호출 해줘야 함.
*	Registering/ Unregistering 구현 해봐야 호출하지 않으면 아무 의미없음.
*	Lyra에서는 GameplayFeaturePolicy를 오버라이딩하는 방식으로 이를 진행함. 이 함수들은 자동으로 호출됨.
*/
UCLASS()
class ULyraGameFeature_AddGameplayCuePaths
	: public UObject
	, public IGameFeatureStateChangeObserver
{
	GENERATED_BODY()

public:
	virtual void OnGameFeatureRegistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL) override;
	
	virtual void OnGameFeatureUnregistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL) override;

};
