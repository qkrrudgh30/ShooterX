// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SXGameModeBase.h"
#include "ShooterX/ShooterX.h"

ASXGameModeBase::ASXGameModeBase()
{
	SX_LOG_NET(LogSXNet, Log, TEXT("Begin"));
	SX_LOG_NET(LogSXNet, Log, TEXT("End"));
}

void ASXGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	SX_LOG_NET(LogSXNet, Log, TEXT("=======Connected======="));
	SX_LOG_NET(LogSXNet, Log, TEXT("Begin"));
	
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	// ErrorMessage = FString::Printf(TEXT("Server is full. Please try again later."));
	
	SX_LOG_NET(LogSXNet, Log, TEXT("End"));
}

APlayerController* ASXGameModeBase::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal,
	const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	SX_LOG_NET(LogSXNet, Log, TEXT("Begin"));

	APlayerController* NewPC = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
	
	SX_LOG_NET(LogSXNet, Log, TEXT("End"));
	
	return NewPC;
}

void ASXGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	SX_LOG_NET(LogSXNet, Log, TEXT("Begin"));
	
	Super::PostLogin(NewPlayer);
	
	SX_LOG_NET(LogSXNet, Log, TEXT("End"));
}

void ASXGameModeBase::StartPlay()
{
	SX_LOG_NET(LogSXNet, Log, TEXT("Begin"));

	Super::StartPlay();
	
	SX_LOG_NET(LogSXNet, Log, TEXT("End"));
}

void ASXGameModeBase::BeginPlay()
{
	SX_LOG_NET(LogSXNet, Log, TEXT("Begin"));
	
	Super::BeginPlay();
	
	SX_LOG_NET(LogSXNet, Log, TEXT("End"));
}
