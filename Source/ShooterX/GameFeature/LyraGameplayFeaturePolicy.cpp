// LyraGameplayFeaturePolicy.cpp

#include "GameFeature/LyraGameplayFeaturePolicy.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeatureAction.h"
#include "GameFeatureData.h"
#include "GFA_AddGameplayCuePath.h"
#include "AbilitySystem/LyraGameplayCueManager.h"
#include "GameplayCueSet.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraGameplayFeaturePolicy)

ULyraGameplayFeaturePolicy::ULyraGameplayFeaturePolicy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void ULyraGameplayFeaturePolicy::InitGameFeatureManager()
{
	Observers.Add(NewObject<ULyraGameFeature_AddGameplayCuePaths>());
	// GameFeature_AddGameplayCuePaths를 등록

	UGameFeaturesSubsystem& Subsystem = UGameFeaturesSubsystem::Get();
	for (UObject* Observer : Observers)
	{
		Subsystem.AddObserver(Observer);
	}
	// GameFeaturesSubsystem에 Observers를 순회하며 등록

	Super::InitGameFeatureManager();
	// 앞서 Subsystem에 미리 Observer를 등록했으므로, GameFeatureManager가 초기화되는 과정에 반영될 것임.
}

void ULyraGameplayFeaturePolicy::ShutdownGameFeatureManager()
{
	Super::ShutdownGameFeatureManager();

	UGameFeaturesSubsystem& Subsystem = UGameFeaturesSubsystem::Get();
	for (UObject* Observer : Observers)
	{
		Subsystem.RemoveObserver(Observer);
	}
	Observers.Empty();
}

void ULyraGameFeature_AddGameplayCuePaths::OnGameFeatureRegistering(const UGameFeatureData* GameFeatureData,
	const FString& PluginName, const FString& PluginURL)
{
	IGameFeatureStateChangeObserver::OnGameFeatureRegistering(GameFeatureData, PluginName, PluginURL);

	// PluginName을 활용하여, PluginRootPath를 계산.
	const FString PluginRootPath = TEXT("/") + PluginName;

	// GameFeatureData의 Action을 순회
	// 예로들어 ShooterCore의 GameFeatureData에 Action을 추가했다면, 그 추가된 Action이 대상이됨.
	for (const UGameFeatureAction* Action : GameFeatureData->GetActions())
	{
		// 그 중에 우리는 _AddGameplayCuePath를 찾아서 처리.
		if (const UGFA_AddGameplayCuePath* AddGameplayCueGFA = Cast<UGFA_AddGameplayCuePath>(Action))
		{
			const TArray<FDirectoryPath>& DirsToAdd = AddGameplayCueGFA->DirectoryPathsToAdd;

			// GameplayCueManager를 가져와서 GFA에 등록된 DirsToAdd를 추가하면서 GCM의 데이터가 업데이트 되도록 진행.
			if (ULyraGameplayCueManager* GCM = ULyraGameplayCueManager::Get())
			{
				UGameplayCueSet* RuntimeGameplayCueSet = GCM->GetRuntimeCueSet();
				const int32 PreInitializeNumCues = RuntimeGameplayCueSet ? RuntimeGameplayCueSet->GameplayCueData.Num() : 0;

				for (const FDirectoryPath& Directory : DirsToAdd)
				{
					FString MutablePath = Directory.Path;

					// PluginPackagePath를 한번 보정해줌 -> 보정된 결과는 MutablePath로 들어옴.
					UGameFeaturesSubsystem::FixPluginPackagePath(MutablePath, PluginRootPath, false);

					// GCM의 RuntimeGameplayCueObjectLibrary의 Path에 추가.
					GCM->AddGameplayCueNotifyPath(MutablePath, /*bShouldRescanCueAssets=*/false);
				}

				// 초기화 시켜줌. 새로 경로가 추가되었으니 호출해줘야함.
				if (false == DirsToAdd.IsEmpty())
				{
					GCM->InitializeRuntimeObjectLibrary();
				}

				// Cue의 애셋이 Pre와 Post랑 비교하여 개수가 달라져 있다면 명시적으로 RefreshGameplayCuePrimaryAsset 호출.
				const int32 PostInitializeNumCues = RuntimeGameplayCueSet ? RuntimeGameplayCueSet->GameplayCueData.Num() : 0;
				if (PreInitializeNumCues != PostInitializeNumCues)
				{
					// RefreshGameplayCuePrimaryAsset() 함수는 추가된 GCN을 AssetManager에 등록해주는 역할.
					GCM->RefreshGameplayCuePrimaryAsset();
				}
			}
		}
	}
}

void ULyraGameFeature_AddGameplayCuePaths::OnGameFeatureUnregistering(const UGameFeatureData* GameFeatureData,
	const FString& PluginName, const FString& PluginURL)
{
	IGameFeatureStateChangeObserver::OnGameFeatureUnregistering(GameFeatureData, PluginName, PluginURL);

	const FString PluginRootPath = TEXT("/") + PluginName;
	for (const UGameFeatureAction* Action : GameFeatureData->GetActions())
	{
		if (const UGFA_AddGameplayCuePath* AddGameplayCueGFA = Cast<UGFA_AddGameplayCuePath>(Action))
		{
			const TArray<FDirectoryPath>& DirsToAdd = AddGameplayCueGFA->DirectoryPathsToAdd;
			if (ULyraGameplayCueManager* GCM = ULyraGameplayCueManager::Get())
			{
				int32 NumRemoved = 0;
				for (const FDirectoryPath& Directory : DirsToAdd)
				{
					FString MutablePath = Directory.Path;
					UGameFeaturesSubsystem::FixPluginPackagePath(MutablePath, PluginRootPath, false);
					NumRemoved += GCM->RemoveGameplayCueNotifyPath(MutablePath, /*bShouldRescanCueAssets=*/false);
				}

				ensure(NumRemoved == DirsToAdd.Num());

				if (0 < NumRemoved)
				{
					GCM->InitializeRuntimeObjectLibrary();
				}
			}
		}
	}
}
