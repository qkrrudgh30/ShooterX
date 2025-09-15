// CommonGameInstance.h

#pragma once

#include "Engine/GameInstance.h"
#include "CommonGameInstance.generated.h"

UCLASS(Abstract)
class COMMONGAME_API UCommonGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UCommonGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual int32 AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId) override;
	
	virtual bool RemoveLocalPlayer(ULocalPlayer* ExistingPlayer) override;
	
public:
	TWeakObjectPtr<ULocalPlayer> PrimaryPlayer;
	
};
