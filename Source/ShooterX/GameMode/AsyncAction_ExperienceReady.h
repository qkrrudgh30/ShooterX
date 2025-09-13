// AsyncAction_ExperienceReady.h

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Delegates/Delegate.h"
#include "AsyncAction_ExperienceReady.generated.h"

// BP를 통한 UFUNCTION으로 바인딩하는 Multivase Delegate -> Dynamic
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExperienceReadyAsyncDelegate);

class AGameStateBase;
class ULyraExperienceDefinition;

UCLASS()
class UAsyncAction_ExperienceReady : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UAsyncAction_ExperienceReady(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// UAsyncAction_ExperienceReady를 생성하고 기다리는 BP 호출
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static UAsyncAction_ExperienceReady* WaitForExperienceReady(UObject* WorldContextObject);

	// UBlueprintAsyncActionBase interface
	virtual void Activate() override;

	// Step1~Step4
	void Step1_HandleGameStateSet(AGameStateBase* GameState);
	void Step2_ListenToExperienceLoading(AGameStateBase* GameState);
	void Step3_HandleExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience);
	void Step4_BroadcastReady();

public:
	// BlueprintAssignable은 BP 상에서 할당 가능한 변수로 정의해주는 키워드.
	UPROPERTY(BlueprintAssignable)
	FExperienceReadyAsyncDelegate OnReady;

	// WorldPtr을 단순 로직상 캐싱하는 용도
	TWeakObjectPtr<UWorld> WorldPtr;

};