// LyraPlayerController.h

#pragma once

#include "ModularPlayerController.h"
#include "LyraPlayerController.generated.h"

class ALyraPlayerState;
class ULyraAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class SHOOTERX_API ALyraPlayerController : public AModularPlayerController
{
	GENERATED_BODY()

public:
	ALyraPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	ALyraPlayerState* GetLyraPlayerState() const;

	ULyraAbilitySystemComponent* GetLyraAbilitySystemComponent() const;

	// PlayerController Interface
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	
};
