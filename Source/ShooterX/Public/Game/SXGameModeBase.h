// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "SXGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERX_API ASXGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASXGameModeBase();

#pragma region Overrides GameModeBase

public:
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void StartPlay() override;

protected:
	virtual void BeginPlay() override;
	
#pragma endregion
	
};
