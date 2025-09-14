// GFA_AddGameplayCuePath.h

#pragma once

#include "GameFeatureAction.h"
#include "GFA_AddGameplayCuePath.generated.h"

UCLASS()
class SHOOTERX_API UGFA_AddGameplayCuePath : public UGameFeatureAction
{
	GENERATED_BODY()

public:
	UGFA_AddGameplayCuePath();

public:
	UPROPERTY(EditAnywhere, Category="GameFeature|GameplayCues")
	TArray<FDirectoryPath> DirectoryPathsToAdd;
	// GameplayCueNotify 애셋 경로 추가

};
