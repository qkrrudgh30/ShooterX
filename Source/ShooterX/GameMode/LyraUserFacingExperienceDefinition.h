// LyraUserFacingExperienceDefinition.h

#pragma once

#include "Containers/Map.h"
#include "Engine/DataAsset.h"
#include "LyraUserFacingExperienceDefinition.generated.h"

/** Description of settings used to display experiences in the UI and start a new session */
/**
 * ULyraUserFacingExperienceDefinition
 *   게임 상의 UI에 Experience를 시각화할 때, 그리고 새로운 Experience로 진입할 때 사용됨.
 */
UCLASS(BlueprintType)
class SHOOTERX_API ULyraUserFacingExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "Map"))
	FPrimaryAssetId MapID;
	/** The specific map to load */
	// MapID 속성은 Map이 될수도 있고, ExperienceDefinition이 될수도 있지만 AllowedTypes 키워드에 의해 Map으로 제한됨.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "LCExperienceDefinition"))
	FPrimaryAssetId ExperienceID;
	/** The gameplay experience to load */

};
