// LyraLocalPlayer.h

#pragma once

#include "CommonGame/CommonLocalPlayer.h"
#include "LyraLocalPlayer.generated.h"

UCLASS()
class SHOOTERX_API ULyraLocalPlayer : public UCommonLocalPlayer
{
	GENERATED_BODY()

public:
	ULyraLocalPlayer(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
