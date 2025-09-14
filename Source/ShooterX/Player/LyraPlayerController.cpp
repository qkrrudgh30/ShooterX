// LyraPlayerController.cpp


#include "Player/LyraPlayerController.h"
#include "Camera/LyraPlayerCameraManager.h"
#include "LyraPlayerState.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"

ALyraPlayerController::ALyraPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = ALyraPlayerCameraManager::StaticClass();
}

ALyraPlayerState* ALyraPlayerController::GetLyraPlayerState() const
{
	// ECastCheckedType() 함수의 NullAllowed는 NULL 반환을 의도할 경우 유용함.
	return CastChecked<ALyraPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

ULyraAbilitySystemComponent* ALyraPlayerController::GetLyraAbilitySystemComponent() const
{
	const ALyraPlayerState* LCPS = GetLyraPlayerState();
	return (LCPS ? LCPS->GetLyraAbilitySystemComponent() : nullptr);
}

void ALyraPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	// 우선 PostProcessInput() 함수가 언제 호출되는지 확인필요함.
	// - UPlayerInput::ProcessInputStack() 함수에서 호출됨.

	if (ULyraAbilitySystemComponent* LCASC = GetLyraAbilitySystemComponent())
	{
		LCASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}
