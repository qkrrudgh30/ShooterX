// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SXCharacterBase.h"
#include "InputActionValue.h"
#include "SXPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class SHOOTERX_API ASXPlayerCharacter : public ASXCharacterBase
{
	GENERATED_BODY()

public:
	ASXPlayerCharacter();
	
#pragma region Overrides Character

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay() override;
	
#pragma endregion
	
#pragma region SXPlayerCharacter
	
public:
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }
	
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SXPlayerCharacter|Components")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SXPlayerCharacter|Components")
	TObjectPtr<UCameraComponent> Camera;

#pragma endregion

#pragma region Input

private:
	void HandleMoveInput(const FInputActionValue& InValue);

	void HandleLookInput(const FInputActionValue& InValue);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SXPlayerCharacter|Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SXPlayerCharacter|Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SXPlayerCharacter|Input")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SXPlayerCharacter|Input")
	TObjectPtr<UInputAction> JumpAction;
	
#pragma endregion
	
};
