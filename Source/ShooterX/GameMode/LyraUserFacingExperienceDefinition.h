// LyraUserFacingExperienceDefinition.h

#pragma once

#include "Containers/Map.h"
#include "Engine/DataAsset.h"
#include "LyraUserFacingExperienceDefinition.generated.h"

class UCommonSession_HostSessionRequest;

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
	UFUNCTION(BlueprintCallable, BlueprintPure = false) 
	UCommonSession_HostSessionRequest* CreateHostingRequest() const;
	// Map 로딩 및 Experience 전환을 위해, MapID와 ExperienceID를 활용하여, HostSessionRequest 생성.
	// BlueprintPure = true: 이벤트 그래프에서 Owning Object에 개의치 않고 실행되게끔 하겠다.

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "Map"))
	FPrimaryAssetId MapID;
	/** The specific map to load */
	// MapID 속성은 Map이 될수도 있고, ExperienceDefinition이 될수도 있지만 AllowedTypes 키워드에 의해 Map으로 제한됨.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "LyraExperienceDefinition"))
	FPrimaryAssetId ExperienceID;
	/** The gameplay experience to load */

};
