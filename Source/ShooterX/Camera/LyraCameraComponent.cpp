// LyraCameraComponent.cpp

#include "LyraCameraComponent.h"
#include "Camera/LyraCameraMode.h"

ULyraCameraComponent::ULyraCameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, CameraModeStack(nullptr)
{

}

void ULyraCameraComponent::OnRegister()
{
	Super::OnRegister();

	if (false == ::IsValid(CameraModeStack))
	{
		CameraModeStack = NewObject<ULyraCameraModeStack>(this);
			// 초기화(BeginPlay() 함수와 같은)가 딱히 필요없는 개체로 NewObject() 함수로 할당.
			// ULyraCameraModeStack 클래스도 UObject 클래스를 상속 받음.
	}
}

void ULyraCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	Super::GetCameraView(DeltaTime, DesiredView);

	check(CameraModeStack);
	
	UpdateCameraModes();
	// 카메라 컴포넌트의 Tick() 함수.
	// 여기서 UpdateCameraModes() 함수를 호출함. 이건 어디서 CameraMode를 가져온다는 뜻.
	// CameraMode는 HeroComponent에서 가져옴. 델리게이트를 바인드 하고, 브로드캐스트 되면서 가져오게 됨.
	// 해당 함수는 HeroComponent의 PawnData에서 DefaultCameraMode를 가져와서 CameraModeStack에 추가하여 CameraMode 블랜딩 준비.

	// EvaluateStack() 함수는 CameraModeStack에 있는 CameraMode를 업데이트와 블랜딩하고 CameraModeStack을 Bottom-Top까지 업데이트된 CameraMode들을 Lerp 진행.
	// - 이에 대한 결과는 CameraModeView에 캐싱.
	FLyraCameraModeView CameraModeView;
	CameraModeStack->EvaluateStack(DeltaTime, CameraModeView);

	if (APawn* TargetPawn = Cast<APawn>(GetTargetActor()))
	{
		if (APlayerController* PC = TargetPawn->GetController<APlayerController>())
		{
			// PlayerController의 ControlRotation을 계산된 CameraModeView의 ControlRotation으로 업데이트.
			// - SetControlRotation을 외우기보다 한번 코드를 보자.
			// -- 해당 함수는 PC가 빙의 하고 있는 Pawn의 RootComponent에 ControlRotation을 반영.(== 조정하고 있는 캐릭터에 회전을 시킨다)
			PC->SetControlRotation(CameraModeView.ControlRotation);
		}
	}

	// Camera의 Location과 Rotation을 반영하자.
	// - 시간 있으면, SetWorldLocationAndRotation() 함수를 보면서 UpdateChildTransform() 함수까지 보는 것을 추천.
	// - SceneGraph 관계 업데이트를 이해할 수 있음.
	// 간단하게 정리하면 CameraComponent에 대해 Parent의 SceneGraph 관계를 고려하여 업데이트 진행.
	SetWorldLocationAndRotation(CameraModeView.Location, CameraModeView.Rotation);

	// FOV 업데이트
	FieldOfView = CameraModeView.FieldOfView;

	// 여기서 우리는 ControlRotation
	// ControlRotation은 PC가 조종하는 Pawn의 Rotation을 적용할 값이다.
	// 그에반해, Rotation은 Camera에 반영할 값.

	// FMinimalViewInfo를 업데이트.
	// - CameraComponent의 변화 사항을 반영해서 최종 랜더링까지 반영하게 됨.
	DesiredView.Location = CameraModeView.Location;
	DesiredView.Rotation = CameraModeView.Rotation;
	DesiredView.FOV = CameraModeView.FieldOfView;
	DesiredView.OrthoWidth = OrthoWidth;
	DesiredView.OrthoNearClipPlane = OrthoNearClipPlane;
	DesiredView.OrthoFarClipPlane = OrthoFarClipPlane;
	DesiredView.AspectRatio = AspectRatio;
	DesiredView.bConstrainAspectRatio = bConstrainAspectRatio;
	DesiredView.bUseFieldOfViewForLOD = bUseFieldOfViewForLOD;
	DesiredView.ProjectionMode = ProjectionMode;
	DesiredView.PostProcessBlendWeight = PostProcessBlendWeight;
	if (0.f < PostProcessBlendWeight)
	{
		DesiredView.PostProcessSettings = PostProcessSettings;
	}
}

void ULyraCameraComponent::UpdateCameraModes()
{
	check(CameraModeStack);

	// DetermineCameraModeDelegate는 CameraMode Class를 반환함.
	// 해당 Delegate는 HeroComponent의 멤버 함수로 바인딩 되어 있음.
	
	if (true == DetermineCameraModeDelegate.IsBound())
	{
		TSubclassOf<ULyraCameraMode> CameraMode = DetermineCameraModeDelegate.Execute();
		if (true == ::IsValid(CameraMode))
		{
			CameraModeStack->PushCameraMode(CameraMode);
		}
	}
}
