// LyraGameplayCueManager.cpp

#include "AbilitySystem/LyraGameplayCueManager.h"
#include "AbilitysystemGlobals.h"
#include "GameplayCueSet.h"
#include "System/LyraAssetManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraGameplayCueManager)

ULyraGameplayCueManager::ULyraGameplayCueManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

ULyraGameplayCueManager* ULyraGameplayCueManager::Get()
{
	return Cast<ULyraGameplayCueManager>(UAbilitySystemGlobals::Get().GetGameplayCueManager());
}
const FPrimaryAssetType UFortAssetManager_GameplayCueRefsType = TEXT("GameplayCueRefs");
const FName UFortAssetManager_GameplayCueRefsName = TEXT("GameplayCueReferences");
const FName UFortAssetManager_LoadStateClient = FName(TEXT("Client"));
void ULyraGameplayCueManager::RefreshGameplayCuePrimaryAsset()
{
	TArray<FSoftObjectPath> CuePaths;
	UGameplayCueSet* RuntimeGameplayCueSet = GetRuntimeCueSet();
	if (RuntimeGameplayCueSet)
	{
		RuntimeGameplayCueSet->GetSoftObjectPaths(CuePaths);
	}

	// 새로 추가된 애셋의 경로를 CuePaths에 추가
	FAssetBundleData BundleData;
	BundleData.AddBundleAssetsTruncated(UFortAssetManager_LoadStateClient, CuePaths);
	// 특수효과는 서버에 등록할 필요가 없음.

	// PrimaryAssetId를 고정된 이름으로 설정하여(const로 설정되어 있음)
	FPrimaryAssetId PrimaryAssetId = FPrimaryAssetId(UFortAssetManager_GameplayCueRefsType, UFortAssetManager_GameplayCueRefsName);

	// 애셋 매니저에 추가 진행
	UAssetManager::Get().AddDynamicAsset(PrimaryAssetId, FSoftObjectPath(), BundleData);
}
