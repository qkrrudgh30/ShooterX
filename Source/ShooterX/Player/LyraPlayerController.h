// LyraPlayerController.h

#pragma once

#include "GameFramework/PlayerController.h"
#include "LyraPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERX_API ALyraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALyraPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
