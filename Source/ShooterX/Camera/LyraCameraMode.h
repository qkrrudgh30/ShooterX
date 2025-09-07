// LyraCameraMode.h

#pragma once

#include "LyraCameraMode.generated.h"

class ULyraCameraComponent;

// [0, 1]을 BlendFunction에 맞게 재매핑하기 위한 타입
UENUM(BlueprintType)
enum class ELyraCameraModeBlendFunction : uint8
{
	Linear,
	// EaseIn/Out은 Exponent 값에 의해 조절됨.
	EaseIn,
	EaseOut,
	EaseInOut,
	COUNT
};

struct FLyraCameraModeView
{
	FLyraCameraModeView();

	void Blend(const FLyraCameraModeView& Other, float OtherWeight);

	// 블랜딩 되어질 값들
	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};

// Camera Blending의 대상이 될 단위 클래스.
UCLASS(Abstract, NotBlueprintable)
class SHOOTERX_API ULyraCameraMode : public UObject
{
	GENERATED_BODY()

public:
	ULyraCameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	ULyraCameraComponent* GetLyraCameraComponent() const;
	
	AActor* GetTargetActor() const;
	
	FVector GetPivotLocation() const;
	
	FRotator GetPivotRotation() const;

	void UpdateCameraMode(float DeltaSecond);
	
	virtual void UpdateView(float DeltaSecond);

	void UpdateBlending(float DeltaSecond);

public:
	// 얼마동안 블랜딩을 진행할지
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	// 선형적인 블랜딩 값 [0, 1]
	float BlendAlpha;

	// 해당 CameraMode의 최종 블랜딩 값
	// 앞서 BlendAlpha의 선형값을 매핑하여 최종 BlendWeight를 계산. 코드를 보며 이해 가능.
	float BlendWeight;

	// CameraMode에 의해 생성된 CameraModeView
	FLyraCameraModeView View;

	// 카메라 모드의 FOV
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170.0", ClampMin = "5.0", ClampMax = "170.0"))
	float FieldOfView;

	// View에 대한 Pitch [Min, Max]
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170.0", ClampMin = "5.0", ClampMax = "170.0"))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170.0", ClampMin = "5.0", ClampMax = "170.0"))
	float ViewPitchMax;

	// EaseIn/Out에 사용할 Exponent. 값이 클수록 더 급격하게 반영됨.
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;

	// Blend function
	ELyraCameraModeBlendFunction BlendFunction;
	
};

// Camera Blending을 담당하는 클래스.
UCLASS()
class SHOOTERX_API ULyraCameraModeStack : public UObject
{
	GENERATED_BODY()

public:
	ULyraCameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	ULyraCameraMode* GetCameraModeInstance(TSubclassOf<ULyraCameraMode>& CameraModeClass);
	
	void PushCameraMode(TSubclassOf<ULyraCameraMode>& CameraModeClass);

	void EvaluateStack(float DeltaSecond, FLyraCameraModeView& OutCameraModeView);
	
	void UpdateStack(float DeltaSecond);
	
	void BlendStack(FLyraCameraModeView& OutCameraModeView) const;	

public:
	UPROPERTY()
	TArray<TObjectPtr<ULyraCameraMode>> CameraModeInstances;
	// 생성된 카메라 모드들을 관리

	UPROPERTY()
	TArray<TObjectPtr<ULyraCameraMode>> CameraModeStack;
	// Camera Matrix Blend 업데이트 진행 큐. ex) Idle -> BoardingVehicle 카메라 블랜딩.

};
