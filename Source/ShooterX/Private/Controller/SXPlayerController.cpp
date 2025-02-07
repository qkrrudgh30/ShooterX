// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SXPlayerController.h"
#include "ShooterX/ShooterX.h"

ASXPlayerController::ASXPlayerController()
{
	SX_LOG_NET(LogSXNet, Log, TEXT("Begin"));
	SX_LOG_NET(LogSXNet, Log, TEXT("End"));
}

void ASXPlayerController::BeginPlay()
{
	SX_LOG_NET(LogSXNet, Log, TEXT("Begin"));
	
	Super::BeginPlay();
	
	SX_LOG_NET(LogSXNet, Log, TEXT("End"));
}
