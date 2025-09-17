// LyraExperienceActionSet.cpp


#include "LyraExperienceActionSet.h"
#include "GameFeatureAction.h"

ULyraExperienceActionSet::ULyraExperienceActionSet()
	: Super()
{
}

#if WITH_EDITORONLY_DATA

void ULyraExperienceActionSet::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();

	for (UGameFeatureAction* Action : Actions)
	{
		if (Action)
		{
			Action->AddAdditionalAssetBundleData(AssetBundleData);
		}
	}
}

#endif
