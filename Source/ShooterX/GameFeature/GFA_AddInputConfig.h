// GFA_AddInputConfig.h

#pragma once

#include "GFA_WorldActionBase.h"
#include "GFA_AddInputConfig.generated.h"

struct FComponentRequestHandle;
class APawn;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class SHOOTERX_API UGFA_AddInputConfig : public UGFA_WorldActionBase
{
	GENERATED_BODY()

public:
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;

private:
	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ExtensionRequestHandles;
		// 이벤트
		
		TArray<TWeakObjectPtr<APawn>> PawnsAddedTo;
		// GFA_AddInputConfig가 적용된 폰을 캐싱. PIE별로 폰이 있을 것임.
	};

	void HandlePawnExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);
	// GameFeatureState가 변경될 때 호출될 콜백 함수(혹은 ExtensionEvent를 GameFrameworkComponentManager를 통해 호출 가능?)

	void AddInputConfig(APawn* Pawn, FPerContextData& ActiveData);
	
	void RemoveInputConfig(APawn* Pawn, FPerContextData& ActiveData);
	// InputConfig를 EnhancedInputLocalPlayerSubsystem에 추가 및 제거

	void Reset(FPerContextData& ActiveData);
	// ActiveData를 초기화

public:
	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;
	// world -> (extension request, pawn)

	// PlayerMappableInputConfig를 들고 있는 멤버 변수
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UInputMappingContext>> InputMappingContexts;

};
