// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "SXCharacterBase.generated.h"

UCLASS()
class SHOOTERX_API ASXCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ASXCharacterBase();

#pragma region Overrides Character
	
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

#pragma endregion
	
};
