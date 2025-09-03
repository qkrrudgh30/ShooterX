// LyraPawnData.h

#pragma once

#include "Engine/DataAsset.h"
#include "LyraPawnData.generated.h"

/**
 * ULyraPawnData
 *
 *	Non-mutable data asset that contains properties used to define a pawn.
 */
UCLASS()
class SHOOTERX_API ULyraPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	ULyraPawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
