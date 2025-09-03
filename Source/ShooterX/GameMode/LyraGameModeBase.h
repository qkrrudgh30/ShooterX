// LyraGameModeBase.h

#pragma once

#include "GameFramework/GameModeBase.h"
#include "LyraGameModeBase.generated.h"

class ULyraExperienceDefinition;

/**
 * 
 */
UCLASS()
class SHOOTERX_API ALyraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALyraGameModeBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) final;

	void HandleMatchAssignmentIfNotExpectingOne();

	virtual void InitGameState() final;

	void OnExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience);
	
};
