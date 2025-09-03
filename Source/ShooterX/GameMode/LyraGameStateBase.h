// LyraGameStateBase.h

#pragma once

#include "GameFramework/GameStateBase.h"
#include "LyraGameStateBase.generated.h"

class ULyraExperienceManagerComponent;

/**
 * 
 */
UCLASS()
class SHOOTERX_API ALyraGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	ALyraGameStateBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UPROPERTY()
	TObjectPtr<ULyraExperienceManagerComponent> ExperienceManagerComponent;

};
