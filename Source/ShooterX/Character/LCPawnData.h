// LCPawnData.h

#pragma once

#include "Engine/DataAsset.h"
#include "LCPawnData.generated.h"

/**
 * ULyraPawnData
 *
 *	Non-mutable data asset that contains properties used to define a pawn.
 */
UCLASS()
class SHOOTERX_API ULCPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	ULCPawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
