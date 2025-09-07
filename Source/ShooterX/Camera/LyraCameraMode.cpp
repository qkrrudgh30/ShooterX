// LyraCameraMode.cpp

#include "Camera/LyraCameraMode.h"
#include "Camera/LyraCameraComponent.h"
#include "LyraPlayerCameraManager.h"

FLyraCameraModeView::FLyraCameraModeView()
	: Location(ForceInit)
	, Rotation(ForceInit)
	, ControlRotation(ForceInit)
	, FieldOfView(LYRA_CAMERA_DEFAULT_FOV)
{
}

void FLyraCameraModeView::Blend(const FLyraCameraModeView& Other, float OtherWeight)
{
	if (OtherWeight <= 0.f)
	{
		return;
	}
	else if (1.f <= OtherWeight)
	{
		*this = Other;
		return;
		// Weight가 1.0이면 Other를 덮어씀.
		// 여기서 Other는 바로 위에 위치한 카메라모드뷰. ex) Index == StackSize - 1이라면, Other은 StackSize - 2.
		// 바로 이 부분 때문에 BlendStack() 함수는 Bottom to Top 방향으로 블랜딩 시도함.
		// Top to Bottom 방향으로 블랜딩하고, 3개의 카메라모드뷰가 있고, 각각의 BlendWeight가 1.0f인데다가
		// 차량 탑승 카메라뷰 -> 하차 카메라뷰(Idle 카메라) -> Crouch 카메라뷰 순서로 카메라모드스택에 들어가 있다고 가정해보자.
		// 이때 우리가 원하는건 Crouch 카메라뷰가 남아야하는데, Crouch 카메라뷰가 오히려 하차 카메라뷰에 덮어쓰기 당해버림.
	}

	// Location + OtherWeight * (Other.Location - Location);
	Location = FMath::Lerp(Location, Other.Location, OtherWeight);

	// Location과 같은 방식 Lerp (ControlRotation과 FieldOfView도 같음)
	const FRotator DeltaRotation = (Other.Rotation - Rotation).GetNormalized();
	Rotation = Rotation + (OtherWeight * DeltaRotation);

	const FRotator DeltaControlRotation = (Other.ControlRotation - ControlRotation).GetNormalized();
	ControlRotation = ControlRotation + (OtherWeight * DeltaControlRotation);

	FieldOfView = FMath::Lerp(FieldOfView, Other.FieldOfView, OtherWeight);
}

ULyraCameraMode::ULyraCameraMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FieldOfView = LYRA_CAMERA_DEFAULT_PITCH_MAX;
	ViewPitchMin = LYRA_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = LYRA_CAMERA_DEFAULT_PITCH_MAX;

	BlendTime = 0.f;
	BlendAlpha = 1.f;
	BlendWeight = 1.f;

	BlendFunction = ELyraCameraModeBlendFunction::EaseOut;
	BlendExponent = 4.f;
}

ULyraCameraComponent* ULyraCameraMode::GetLyraCameraComponent() const
{
	return CastChecked<ULyraCameraComponent>(GetOuter());
	// ULyraCameraModeStack::PushCameraMode()에서 카메라모드의 오너로 LyraCameraComponent를 설정해주는 코드를 작성함.
	// 더 이전에는 LyraCameraComponent::OnRegister()에서 카메라모드스택이 만들어지고, 오너로 카메라컴포넌트가 지정됨.
	// 즉, 카메라모드스택과 카메라모드의 오너는 카메라컴포넌트임. 카메라모드의 오너를 카메라모드스택이라고 하면 안됨.
}

AActor* ULyraCameraMode::GetTargetActor() const
{
	const ULyraCameraComponent* LyraCameraComponent = GetLyraCameraComponent();
	return LyraCameraComponent->GetTargetActor();
}

FVector ULyraCameraMode::GetPivotLocation() const
{
	// ULyraCameraMode::GetPivotLocation() 함수의 내용은 훨씬 더 많음. 
	// bCollisionTest 관련 내용들이 더 있음. 혹은 Couch 상태에서의 카메라 보정 로직 등, ...

	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		// BaseEyeHeight를 고려하여 ViewLocation을 반환.
		return TargetPawn->GetPawnViewLocation();
	}

	return TargetActor->GetActorLocation();
}

FRotator ULyraCameraMode::GetPivotRotation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		// GetViewRotation() 확인. 보통 Pawn의 ControlRotation을 반환함.
		return TargetPawn->GetViewRotation();
	}

	return TargetActor->GetActorRotation();
}

void ULyraCameraMode::UpdateCameraMode(float DeltaSecond)
{
	// Actor를 활영하여 PivotLocation/PivotRotation을 계산 및 View 업데이트.
	UpdateView(DeltaSecond);

	UpdateBlending(DeltaSecond);
}

void ULyraCameraMode::UpdateView(float DeltaSecond)
{
	// CameraMode를 가지고 있는 CameraComponent의 Owner인 Character를 활용하여 PivotLocation/PivotRotation을 변환.
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	// Pitch 값에 대해 Min/Max를 Clamp 시킴.
	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	// FLyraCameraModeView에 PitchLocation/PitchRotation 설정.
	View.Location = PivotLocation;
	View.Rotation = PivotRotation;

	// PivotRotation을 똑같이 ControlRotation으로 활용.
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;

	// 정리하면 Character의 Location과 ControlRotation을 활용하여 View를 업데이트
}

void ULyraCameraMode::UpdateBlending(float DeltaSecond)
{
	// BlendAlpha를 DeltaSecond을 통해 계산.
	if (0.f < BlendTime)
	{
		// BlendTime은 Blending 과정 총 시간(초)
		// - BlendAlpha는 0 -> 1로 변화하는 과정
		// - DeltaSecond을 활용하여 BlendTime을 1로 볼 경우, 진행 정도를 누적
		BlendAlpha += (DeltaSecond / BlendTime);
	}
	else
	{
		BlendAlpha = 1.f;
	}

	// BlendWeight를 [0, 1]를 BlendFunction에 맞게 재매핑
	const float Exponent = (0.f < BlendExponent) ? BlendExponent : 1.f;
	switch (BlendFunction)
	{
	case ELyraCameraModeBlendFunction::Linear:
		BlendWeight = BlendAlpha;
		break;
	case ELyraCameraModeBlendFunction::EaseIn:
		BlendWeight = FMath::InterpEaseIn(0.f, 1.f, BlendAlpha, Exponent);
		break;
	case ELyraCameraModeBlendFunction::EaseOut:
		BlendWeight = FMath::InterpEaseOut(0.f, 1.f, BlendAlpha, Exponent);
		break;
	case ELyraCameraModeBlendFunction::EaseInOut:
		BlendWeight = FMath::InterpEaseOut(0.f, 1.f, BlendAlpha, Exponent);
		break;
	default:
		checkf(false, TEXT("UpdateBlending(): Invalid BlendFunction [%d]\n"), (uint8)(BlendFunction));
		break;
	}
}

ULyraCameraModeStack::ULyraCameraModeStack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

ULyraCameraMode* ULyraCameraModeStack::GetCameraModeInstance(TSubclassOf<ULyraCameraMode>& CameraModeClass)
{
	check(CameraModeClass);

	// CameraModeInstances에 이전에 생성해서 넣어뒀었는지 확인 후 반환.
	for (ULyraCameraMode* CameraMode : CameraModeInstances)
	{
		// CameraMode는 UClass를 비교함.
		// - 즉, CameraMode는 클래스 타입 당 하나씩만 생성되게 됨.
		if ((nullptr != CameraMode) && (CameraMode->GetClass() == CameraModeClass))
		{
			return CameraMode;
		}
	}

	// 인자로 들어온 CameraModeClass에 알맞는 CameraMode 클래스 인스턴스가 없다면 생성.
	// 생성시 카메라모드의 오너로 GetOuter(), 즉 LyraCameraComponent를 넘겨줌.
	ULyraCameraMode* NewCameraMode = NewObject<ULyraCameraMode>(GetOuter(), CameraModeClass, NAME_None, RF_NoFlags);
	check(NewCameraMode);

	// 여기서 우리는 CameraModeInstance는 CameraModeClass 클래스 인스턴스를 관리하는 컨테이너라는 것을 알 수 있음.
	CameraModeInstances.Add(NewCameraMode);

	return NewCameraMode;
}

void ULyraCameraModeStack::PushCameraMode(TSubclassOf<ULyraCameraMode>& CameraModeClass)
{
	if (nullptr == CameraModeClass)
	{
		return;
	}

	ULyraCameraMode* CameraModeToPush = GetCameraModeInstance(CameraModeClass);
	// 조금 특이하게 동작하고 있음이 보임. CameraModeClass로 그냥 Instance를 가져오면 안될까? 왜 굳이 GetCameraModeInstance() 함수를 호출해야할까?
	// GetCameraModeInstance() 함수 내부에서는 아래 두 가지 동작이 이뤄짐.
	// 1. CameraModeClass에 해당하는 CameraModeInstance가 있는 경우 -> 바로 반환.
	// 2. CameraModeClass에 해당하는 CameraModeInstance가 있는 경우 -> 생성하고 CameraModeInstances에 저장하고 반환.
	// 위 두 가지 로직을 함수 형태로 묶어둔 것.

	int32 StackSize = CameraModeStack.Num();
	if ((0 < StackSize) && (CameraModeStack[0] == CameraModeToPush))
	{
		return;
			// CameraModeStack[0]은 가장 최근에 추가된 카메라모드 인스턴스. 이미 가장 최근에 추가되었다면 early-exit.
	}

	int32 ExistingStackIndex = INDEX_NONE;
	float ExistingStackContribution = 1.f;
		// ExistingStackIndex는 CameraModeStack에서 CameraMode에 맞는 Index를 찾음.
		// ExistingStackContribution은 위에서 아래로 최종 Blendweight 값을 찾기 위해 초기값으로 1.f 설정.
	
	// 해당 카메라모드의 BlendWeight 값이 1일수록 스택에 오래 머문 카메라모드.
	// 반대로 0일수록 스택에 최근 추가된 카메라모드.
	// 이는 뒤에서 정의하게될 UpdateBlending() 함수에서 보간 함수를 통해 이뤄짐.

	// 중요한 것은 어떻게 추가될 카메라모드가 기존 카메라모드들과 잘 어우러지게 할 것인가임.
	// 아래 반복문은 기존 카메라모드들과 이번에 추가될 카메라모드의 ExistingStackContribution 총합이 1.0f가 되게끔 해줌.
	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		if (CameraModeStack[StackIndex] == CameraModeToPush)
		{
			// 2. 이번에 추가된 카메라모드 인스턴스를 카메라모드스택의 중간에서 찾아냈다면,(이미 있었단 뜻)
			//    RemainingStackContribution에 해당 카메라모드 인스턴스의 BlendWeight를 곱함. 이를통해 비로소 진짜 의미의 ExistingStackContribution이 구해짐.
			//    이 경우, 해당 카메라모드 인스턴스의 ExistingStackContribution을 계산하여 맨 위로 올릴 예정.
		
			ExistingStackIndex = StackIndex;
			// 매 델타타임 만큼씩 카메라 블랜딩이 이뤄지는 중.
			// 이번에 추가되는 CameraMode의 인스턴스가 스택의 중간에 있는 경우. 해당 CameraMode의 인스턴스를 맨 위로 올리면서 BlendWeight를 1.f로 둘 수 없음.
			// 현재 진행되고 있는 상태 그대로 맨위로 올리고자 함.
			// BlendWeight를 CameraMode에 추가.
			// - 이번에 추가되는 CameraMode의 인스턴스라면, 누적된 ExistingStackContribution은 BlendWeight를 곱하여 루프 탈출.
			ExistingStackContribution *= CameraModeToPush->BlendWeight;
			break;
		}
		else
		{
			// 1. 이번에 추가된 카메라모드의 ExistingStackContribution을 구하려면, 다른 카메라모드들의 BlendWeight에 의한 RemainingStackContribution을 구해야함.
			//    이는 누적된 값이기 때문에 아래와 같은 수식이 필요해짐. 아래 수식은 결국 전체 ExistingStackContribution을 더하면 1.0f가 되게끔 해주는 수식.
			//    수식에는 ExistingStackContribution이지만, 의역하자면 RemainingStackContribution임.
			ExistingStackContribution *= (1.f - CameraModeStack[StackIndex]->BlendWeight);
		}
	}

	if (ExistingStackIndex != INDEX_NONE)
	{
		CameraModeStack.RemoveAt(ExistingStackIndex);
		--StackSize;
			// 이번에 추가된 카메라모드 인스턴스를 카메라모드스택의 중간에서 찾아냈다면,
			// 스택의 맨 위로 올리기 위해서 일단 제거. 
	}
	else
	{
		ExistingStackContribution = 0.f;
			// 카메라모드스택에 없던 카메라모드 인스턴스를 추가하는 경우. 
			// 이 경우에는 0.0f로 두고 맨 위에 추가 예정. 
	}

	// BlendWeight가 0보다 크다는 것은 블랜딩을 이미 어느정도 시간동안 진행함을 의미. 따라서 ExistingStackContribution을 적용.
	// 블랜딩를 하지 않는다면 BlendWeight를 1.0을 설정. 그럼 위 반복문 로직덕분에 RemainingStackContribution이 0.0f가 되며 다른 카메라모드는 블랜딩되지 않음.
	// 오로지 현재 추가된 카메라모드만 블랜딩됨.
	const bool bShouldBlend = ((0.f < CameraModeToPush->BlendWeight) && (0 < StackSize));
	const float BlendWeight = (true == bShouldBlend ? ExistingStackContribution : 1.f);
	CameraModeToPush->BlendWeight = BlendWeight;

	// TArray를 Insert-Zero 방식의 Stack으로 사용하는 것은 비효율적으로 보임.
	// - Push() & Pop() 메서드와 같이 그냥 Insert-Last에 넣는게 더 나아보임.
	// - 그러나 굳이 이렇게 하는 이유가 뒤에서 나옴.
	CameraModeStack.Insert(CameraModeToPush, 0);

	// 앞서 설명했듯이 마지막은 항상 1.f가 되어야 함.
	CameraModeStack.Last()->BlendWeight = 1.f;
}

void ULyraCameraModeStack::EvaluateStack(float DeltaSecond, FLyraCameraModeView& OutCameraModeView)
{
	// Top -> Bottom [0 -> Num]까지 순차적으로 Stack에 있는 CameraMode 업데이트
	UpdateStack(DeltaSecond);

	// Bottom [0 -> Num] -> Top까지 순차적으로 Stack에 있는 CameraMode 업데이트
	BlendStack(OutCameraModeView);
}

void ULyraCameraModeStack::UpdateStack(float DeltaSecond)
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	int32 RemoveCount = 0;
	int32 RemoveIndex = INDEX_NONE;
	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		ULyraCameraMode* CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		CameraMode->UpdateCameraMode(DeltaSecond);

		// 해당 인덱스의 카메라모드의 BlendWeight가 1.f에 도달했다면, 해당 인덱스의 카메라모드를 포함해서 이후 인덱스의 카메라모드도 제거.
		if (1.f <= CameraMode->BlendWeight)
		{
			RemoveIndex = (StackSize + 1);
			RemoveCount = (StackSize - RemoveIndex);
			break;
		}
	}

	if (0 < RemoveCount)
	{
		CameraModeStack.RemoveAt(RemoveIndex, RemoveCount);
		// 이때문에 PushCameraMode() 함수에서 0번째에 카메라모드를 추가한 것.
		// 제거 로직이 뒷 번호부터 이뤄지기 때문.
	}
	// if (0 < RemoveCount) 조건문도 킥임.
	// RemoveCount = StackSize - RemoveIndex인데, 1개만 남아있는 경우에 RemoveCount는 0이 됨.
	// 그리고 조건문은 거짓이되며 1개를 삭제하지 않게됨.
	// 즉, 시간이 지나서 1개 남은 카메라모드의 BlendWeight가 1.0f 되더라도 삭제되지 않고 유지된다는 뜻.
}

void ULyraCameraModeStack::BlendStack(FLyraCameraModeView& OutCameraModeView) const
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	const ULyraCameraMode* CameraMode = CameraModeStack[StackSize - 1];
	check(CameraMode);
	// CameraModeStack은 Bottom -> Top 순서로 Blend를 진행한다.
	// ex. 차량 탑승 상태 -> 하차 카메라(Idle 카메라) -> Crouch 카메라 순이라면
	//    현재 차량 탑승 상태 카메라 블랜딩이 가장 많은 DeltaSecond 누적.
	//    얘를 가장 먼저 블랜드 해주면서 빠지게끔 해줘야함.

	OutCameraModeView = CameraMode->View;
	// 참고로 가장 아래 Bottom은 BlendWeight가 1이다.

	// 이미 Index = [StackSize - 1] OutCameraModeView로 지정했으므로, Stack - 2부터 순회하면 됨.
	for (int32 StackIndex = (StackSize - 2); 0 <= StackIndex; --StackIndex)
	{
		CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		OutCameraModeView.Blend(CameraMode->View, CameraMode->BlendWeight);
	}
}
