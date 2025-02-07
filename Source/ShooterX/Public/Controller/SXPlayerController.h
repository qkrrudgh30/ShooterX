﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "SXPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERX_API ASXPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASXPlayerController();

#pragma region Overrides PlayerController
	
public:
	virtual void BeginPlay() override;
	
#pragma endregion
	
};
