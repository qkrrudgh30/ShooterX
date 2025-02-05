// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SXCharacterBase.h"


ASXCharacterBase::ASXCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASXCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASXCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASXCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

