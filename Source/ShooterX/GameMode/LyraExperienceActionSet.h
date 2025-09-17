// LyraExperienceActionSet.h

#pragma once

#include "Engine/DataAsset.h"
#include "LyraExperienceActionSet.generated.h"

class UGameFeatureAction;

// DataAsset으로서 UGameFeatureAction을 카테고리화 시킬 때 유용한 클래스
// ex. FPS 모드 액션셋, AOS 모드 액션셋, ...
UCLASS(BlueprintType)
class SHOOTERX_API ULyraExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	ULyraExperienceActionSet();

#if WITH_EDITORONLY_DATA

	virtual void UpdateAssetBundleData() override;
	
#endif

public:
	UPROPERTY(EditAnywhere, Category = "Actions to Perform")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;

};
