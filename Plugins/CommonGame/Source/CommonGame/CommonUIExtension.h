// CommonUIExtension.h

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "Templates/SubclassOf.h"
#include "CommonUIExtension.generated.h"

class ULocalPlayer;
class UCommonActivatableWidget;

UCLASS()
class COMMONGAME_API UCommonUIExtension : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UCommonUIExtension()
	{

	}

	static UCommonActivatableWidget* PushContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, FGameplayTag LayerName, TSubclassOf<UCommonActivatableWidget> WidgetClass);

};
