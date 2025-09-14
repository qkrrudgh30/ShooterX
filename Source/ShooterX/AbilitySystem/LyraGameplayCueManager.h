// LyraGameplayCueManager.h

#pragma once

#include "GameplayCueManager.h"
#include "LyraGameplayCueManager.generated.h"

UCLASS()
class SHOOTERX_API ULyraGameplayCueManager : public UGameplayCueManager
{
	GENERATED_BODY()

public:
	ULyraGameplayCueManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static ULyraGameplayCueManager* Get();

	void RefreshGameplayCuePrimaryAsset();

};
