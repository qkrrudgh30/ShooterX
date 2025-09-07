// LyraCharacterBase.h

#pragma once

#include "GameFramework/Character.h"
#include "LyraCharacterBase.generated.h"

class ULyraPawnExtensionComponent;
class ULyraCameraComponent;

UCLASS()
class SHOOTERX_API ALyraCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ALyraCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) final;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ALyraCharacterBase")
	TObjectPtr<ULyraPawnExtensionComponent> PawnExtensionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ALyraCharacterBase")
	TObjectPtr<ULyraCameraComponent> CameraComponent;

};
