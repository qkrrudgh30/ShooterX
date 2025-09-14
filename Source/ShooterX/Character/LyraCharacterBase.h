// LyraCharacterBase.h

#pragma once

#include "ModularCharacter.h"
#include "AbilitySystemInterface.h"
#include "LyraCharacterBase.generated.h"

class ULyraPawnExtensionComponent;
class ULyraCameraComponent;
class UAbilitySystemComponent;

UCLASS()
class SHOOTERX_API ALyraCharacterBase
	: public AModularCharacter
	, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ALyraCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) final;

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ALyraCharacterBase")
	TObjectPtr<ULyraPawnExtensionComponent> PawnExtensionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ALyraCharacterBase")
	TObjectPtr<ULyraCameraComponent> CameraComponent;

};
