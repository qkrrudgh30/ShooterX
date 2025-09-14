// LyraPawnExtensionComponent.h

#pragma once

#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "LyraPawnExtensionComponent.generated.h"

class ULyraPawnData;
class ULyraAbilitySystemComponent;

/**
 * 폰에 부착될 컴포넌트들의 초기화 과정 전반을 관장하는 컴포넌트.
 */
UCLASS()
class SHOOTERX_API ULyraPawnExtensionComponent
	: public UPawnComponent
	, public IGameFrameworkInitStateInterface // GameInstance에서 등록한 InitState 태그들을 이용해서 초기화를 단계적으로 관리 할 수 있게 해줌.
{
	GENERATED_BODY()

public:
	ULyraPawnExtensionComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }

	virtual void OnRegister() override;

	virtual void BeginPlay() override final;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override final;

	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	// 감시하는 대상의 상태가 변화했을때 호출될 콜백함수

	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	// 이 컴포넌트가 현재 상태 변경이 가능한지 체크하는 함수

	virtual void CheckDefaultInitialization() final;
	// 나를 제외하고 Actor에 등록된 피처 컴포넌트들을 강제로 업데이트 하는 

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	static ULyraPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<ULyraPawnExtensionComponent>() : nullptr); }

	template<class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	void SetPawnData(const ULyraPawnData* InPawnData);
	
	UFUNCTION()
	void OnRep_PawnData();
	
	void SetupPlayerInputComponent();

	ULyraAbilitySystemComponent* GetLyraAbilitySystemComponent() const { return AbilitySystemComponent; }

	void InitializeAbilitySystem(ULyraAbilitySystemComponent* InASC, AActor* InOwnerActor);

	void UninitializeAbilitySystem();

public:
	static const FName NAME_ActorFeatureName;
	// FName 속성은 하나만 선언하는 것이 관례이기 때문에 static 키워드를 붙힘.

	UPROPERTY(EditInstanceOnly, ReplicatedUsing = OnRep_PawnData, Category = "ULyraPawnExtensionComponent")
	TObjectPtr<const ULyraPawnData> PawnData;

	UPROPERTY()
	TObjectPtr<ULyraAbilitySystemComponent> AbilitySystemComponent;
	
};
