// LyraCharacterBase.cpp


#include "Character/LyraCharacterBase.h"
#include "LyraPawnExtensionComponent.h"
#include "Camera/LyraCameraComponent.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"

ALyraCharacterBase::ALyraCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;

	PawnExtensionComponent = CreateDefaultSubobject<ULyraPawnExtensionComponent>(TEXT("PawnExtComponent"));
	// PawnExtensionComponent 컴포넌트는 전반적인 초기화를 담당하기 때문에 가장한 빠른 시점에서 초기화되어야 함.
	// 따라서 CDO 생성 과정에서 초기화 될 수 있게끔 생성자에서 초기화.
	// 반면에 HeroComponent는 LyraCharacterBase의 블루프린트 클래스 애셋에서 직접 추가해줌.
	// 이를 통해 HeroComponent가 PawnExtensionComponent 초기화 이후에 생성되게끔 유도함.
	// SimpleConstructionScript(Blueprint Class) -> UClass(Unreal C++) -> UObject 구조.

	CameraComponent = CreateDefaultSubobject<ULyraCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(-300.f, 0.f, 0.f));
}

void ALyraCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Pawn이 Possess로서, Controller와 PlayerState 정보 접근이 가능한 상태가 되었음.
	// - SetupPlayerInputComponent() 함수 살펴보기
	PawnExtensionComponent->SetupPlayerInputComponent();
}

UAbilitySystemComponent* ALyraCharacterBase::GetAbilitySystemComponent() const
{
	// 앞서 우리는 PawnExtensionComponent에 AbilitySystemComponent를 캐싱 했었음.
	return Cast<UAbilitySystemComponent>(PawnExtensionComponent->GetLyraAbilitySystemComponent());
}
