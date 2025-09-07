// LyraCameraComponent.h

#pragma once

#include "Camera/CameraComponent.h"
#include "LyraCameraComponent.generated.h"

class ULyraCameraModeStack;
class ULyraCameraMode;

DECLARE_DELEGATE_RetVal(TSubclassOf<ULyraCameraMode>, FLyraCameraModeDelegate);

UCLASS()
class SHOOTERX_API ULyraCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	ULyraCameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void OnRegister() final;

	static ULyraCameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<ULyraCameraComponent>() : nullptr); } 

	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) final;

	void UpdateCameraModes();

	AActor* GetTargetActor() const { return GetOwner(); }
    
public:
  	UPROPERTY()
   	TObjectPtr<ULyraCameraModeStack> CameraModeStack;

	FLyraCameraModeDelegate DetermineCameraModeDelegate;

};
