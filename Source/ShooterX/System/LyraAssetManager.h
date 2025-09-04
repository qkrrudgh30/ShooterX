// LyraAssetManager.h

#pragma once

#include "Engine/AssetManager.h"
#include "LyraAssetManager.generated.h"

/**
 * UAssetManager
 * 
 * A singleton UObject that is responsible for loading and unloading PrimaryAssets, and maintaining game-specific asset references
 * Games should override this class and change the class reference
 */
/**
 * ULyraAssetManager
 *
 *	Game implementation of the asset manager that overrides functionality and stores game-specific types.
 *	It is expected that most games will want to override AssetManager as it provides a good place for game-specific loading logic.
 *	This class is used by setting 'AssetManagerClassName' in DefaultEngine.ini.
 */
UCLASS()
class SHOOTERX_API ULyraAssetManager : public UAssetManager
{
	GENERATED_BODY()

#pragma region AssetManager Overrides

public:
	ULyraAssetManager();
	
	static ULyraAssetManager& Get();
		// Returns the AssetManager singleton object.

	virtual void StartInitialLoading() override final;
	// 엔진 극 초반부에 호출되는 함수.
	// 라이라는 엔진 초반부에 초기화 될 것들(게임플레이태그, 게임큐매니저, 애셋이 로딩될 시점 변경, ...)을 이 함수에서 처리.
	
#pragma endregion

#pragma region Asset Loading

public:
	static bool ShouldLogAssetLoads();
	// 굳이 클론 코딩 할 필요는 없지만, 알아두면 좋은 코드들이 있음.

	static UObject* SynchronousLoadAsset(const FSoftObjectPath& InAssetObjectPath);
	// ULyraAssetManager가 로딩을 관장하게 될 것임을 알 수 있는 함수.
		
#pragma endregion
	
#pragma region Asset Caching

protected:
	void AddLoadedAsset(const UObject* InLoadedAsset);
	// Thread safe way of adding a loaded asset to keep in memory.
	// [Thread-Safe] 메모리에 로딩된 애셋 캐싱 함수.
	
private:
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;
	// Assets loaded and tracked by the asset manager.
	// UPROPERTY() 매크로에 의해 GC의 대상이지만
	// LyraAssetManager는 UEngine::AssetManager에 지정되므로 싱글톤 개체임.
	// 따라서 LoadedAssets에 등록된다는 것은 GC에 의해 정리되지 않음을 의미함.

	FCriticalSection LoadedAssetsCritical;
	// Used for a scope lock when modifying the list of load assets.
	// Object 단위 Locking

#pragma endregion

#pragma region Asset Getter

public:
	template<typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& InAssetPointer, bool bKeepsInMemory = true);
	// Returns the asset referenced by a TSoftObjectPtr. This will synchronously load the asset if it's not already loaded.
	// GetAsset() 함수는 정적 로딩으로 블루프린트 클래스와 UObject 로딩
	//   주목해야 할 부분은 bKeepInMemory

	template<typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& InAssetPointer, bool bKeepsInMemory = true);
	// Returns the subclass referenced by a TSoftClassPtr. This will synchronously load the asset if it's not already loaded.
	// 월드에 액터를 스폰할 때, 블루프린트 클래스를 넣어줘야할 때가 있음.
	// 블루프린트 클래스도 애셋이기 때문에 이런 때를 위해서 GetSubclass() 함수가 필요해짐.
	
#pragma endregion
	
};

template<typename AssetType>
inline AssetType* ULyraAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& InAssetPointer, bool bKeepsInMemory)
{
	AssetType* LoadedAsset = nullptr;
	const FSoftObjectPath& AssetObjectPath = InAssetPointer.ToSoftObjectPath();
	if (true == AssetObjectPath.IsValid())
	{
		LoadedAsset = InAssetPointer.Get();
		if (nullptr == LoadedAsset) // 로딩이 안되어 있는 경우.
		{
			LoadedAsset = Cast<AssetObjectPath>(SynchronousLoadAsset(AssetObjectPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset: %s"), *InAssetPointer.ToString());
		}

		if (nullptr != LoadedAsset && true == bKeepsInMemory)
		{
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
			// Added to loaded asset list.
			// 여기서 AssetLoadedAsset() 함수는 메모리에 로드된 애셋을 상주 시키기 위한 함수.
			// 한 번 등록되면 직접 내리지 않는 한 Unload가 되지 않음. 이를 캐싱이라고 함.
		}
	}

	return LoadedAsset;
}

template<typename AssetType>
inline TSubclassOf<AssetType> ULyraAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& InAssetPointer, bool bKeepsInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;
	const FSoftObjectPath& AssetObjectPath = InAssetPointer.ToSoftObjectPath();
	if (true == AssetObjectPath.IsValid())
	{
		LoadedSubclass = InAssetPointer.Get();
		if (nullptr == LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetObjectPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class: %s"), *InAssetPointer.ToString());
		}

		if (nullptr != LoadedSubclass && true == bKeepsInMemory)
		{
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
			// Added to loaded asset list.
		}
	}

	return LoadedSubclass;
}
