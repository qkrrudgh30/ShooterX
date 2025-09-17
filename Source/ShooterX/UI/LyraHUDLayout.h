// LyraHUDLayout.h

#pragma once

#include "LyraCommonActivatableWidget.h"
#include "LyraHUDLayout.generated.h"

// PrimaryGameLayout의 Layer에 연동할 HUD Layout. CommonActivatableWidget
UCLASS(Abstract, BlueprintType, Blueprintable, meta=(DisplayName="LC HUD Layout", Category="LyraClone|HUD"))
class SHOOTERX_API ULyraHUDLayout : public ULyraCommonActivatableWidget
{
	GENERATED_BODY()

public:
	ULyraHUDLayout(const FObjectInitializer& ObjectInitializer);

};
