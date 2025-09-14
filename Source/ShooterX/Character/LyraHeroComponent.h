// LyraHeroComponent.h

#pragma once

#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "InputActionValue.h"
#include "LyraHeroComponent.generated.h"

class ULyraCameraMode;
class UInputMappingContext;
class UInputComponent;

/**
 * component that sets up input and camera handling for player controlled panws (or bots that simulate players)
 * - this depends on a PawnExtensionComponent to coordinate initialization.
 *
 * 카메라, 입력 등 플레이어가 제어하는 시스템의 초기화를 처리하는 컴포넌트.
 * 기존에는 ACharacter::SetupPlayerInputComponent() 함수 같은 곳에서 입력 설정 로직을 작성했음.
 * 근데 이렇게 하면 입력에 대한 반응이 동작하려면 캐릭터가 스폰되어야함. 즉, 종속성이 생겨버림.
 * 입력에 대한 로직도 컴포넌트로 빼버리고, 해당 컴포넌트의 초기화 순서까지도 우리가 컨트롤 할 수 있게끔 하고 싶은듯.(개인적 추측)
 *
 * Blueprintable: 블루프린트 클래스를 생성할 때 부모 클래스로 지정 가능하게끔 하는 키워드.
 * BlueprintSpawnableComponent: 블루프린트 클래스에 컴포넌트로 생성 가능하게끔 하는 키워드.
 */
UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class SHOOTERX_API ULyraHeroComponent
	: public UPawnComponent
	, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	ULyraHeroComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnRegister() final;

	virtual void BeginPlay() final;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final;
	
	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }

	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;

	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	
	virtual void CheckDefaultInitialization() final override;

	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) final;
	// LyraPawnExtensionComponent는 모든 피쳐들의 초기화 진행 과정을 관장하는 역할만함. 따로 데이터 핸들링까지 하지 않음. 그래서 HandleChangeInitState() 재정의 하지 않음.
	// 그러나 LyraHeroComponent는 추후에 입력과 카메라 관련 데이터 핸들링도 필요하기 때문에 HandleChangeInitState() 함수도 추가로 재정의.

	TSubclassOf<ULyraCameraMode> DetermineCameraMode() const;

	void InitializePlayerInput(UInputComponent* PlayerInputComponent);

private:
	void Input_Move(const FInputActionValue& InputActionValue);
	
	void Input_LookMouse(const FInputActionValue& InputActionValue);

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);
	
public:
	static const FName NAME_ActorFeatureName;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;

	static const FName NAME_BindInputsNow;
	// Extension Event 이름 정의
	
};
