// LyraCameraMode_ThirdPerson.h

#pragma once

#include "LyraCameraMode.h"
#include "LyraCameraMode_ThirdPerson.generated.h"

class UCurveVector;

UCLASS(Abstract, Blueprintable)
class SHOOTERX_API ULyraCameraMode_ThirdPerson : public ULyraCameraMode
{
	GENERATED_BODY()

public:
	ULyraCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void UpdateView(float DeltaSecond) override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	TObjectPtr<const UCurveVector> TargetOffsetCurve;

};
