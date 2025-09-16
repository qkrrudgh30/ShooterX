// LyraHUD.h

#pragma once

#include "GameFramework/HUD.h"
#include "LyraHUD.generated.h"

UCLASS()
class SHOOTERX_API ALyraHUD : public AHUD
{
	GENERATED_BODY()

public:
	ALyraHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// GameFrameworkComponentMananger의 AddReceiver를 위한 메서드들
	virtual void PreInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
