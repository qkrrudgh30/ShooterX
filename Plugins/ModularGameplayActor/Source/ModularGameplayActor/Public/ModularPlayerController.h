// ModularPlayerController.h

#pragma once

#include "GameFramework/PlayerController.h"
#include "ModularPlayerController.generated.h"

/**
 *
 */
UCLASS()
class MODULARGAMEPLAYACTOR_API AModularPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void PreInitializeComponents() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
