// LyraPlayerState.h

#pragma once

#include "GameFramework/PlayerState.h"
#include "LyraPlayerState.generated.h"

class ULyraPawnData;
class ULyraExperienceDefinition;

/**
 * 
 */
UCLASS()
class SHOOTERX_API ALyraPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ALyraPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostInitializeComponents() final;

	void OnExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience);
	
};
