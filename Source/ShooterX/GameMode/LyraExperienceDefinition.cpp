// LyraExperienceDefinition.cpp

#include "LyraExperienceDefinition.h"
#include "GameFeatureAction.h"

ULyraExperienceDefinition::ULyraExperienceDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

#if WITH_EDITORONLY_DATA

void ULyraExperienceDefinition::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();
	// UpdateAssetBundleData() 코드를 한 번 보면 좋음.

	for (UGameFeatureAction* Action : Actions)
	{
		if (Action)
		{
			// AddAdditionalAssetBundleData() 함수는 UGameFeatureAction의 메서드임.
			// - 우리가 임의로 호출해서 AssetBundleData에 추가해주도록 하자.
			Action->AddAdditionalAssetBundleData(AssetBundleData);
		}
	}
}

#endif
