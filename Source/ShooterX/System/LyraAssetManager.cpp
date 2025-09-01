// LyraAssetManager.cpp


#include "System/LyraAssetManager.h"

#include "ShooterX.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraAssetManager)

ULyraAssetManager::ULyraAssetManager()
	: Super()
{
}

ULyraAssetManager& ULyraAssetManager::Get()
{
	checkf(GEngine != nullptr, TEXT("GEngine is invalid"));

	if (ULyraAssetManager* Instance = Cast<ULyraAssetManager>(GEngine->AssetManager))
	{
		return *Instance;
	}

	UE_LOG(LogSX, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini(projet settings); It must be LyraAssetManager."));

	return *NewObject<ULyraAssetManager>();
		// 위 UE_LOG(Fatal)로 인해 Crash 나기 때문에 도달하진 않지만 컴파일을 위해 더미 리턴.
}

void ULyraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	// This does all of the scanning, need to do this now even if loads are deferred
}

bool ULyraAssetManager::ShouldLogAssetLoads()
{
	/*
	알아두면 좋은 것들
	1. FParse::Param과 FCommandLine::Get()을 통해 Command Line Shell로부터 전달 받은 인자 값들을 가져올 수 있음.
	2. 함수 안에 static 변수의 선언
	  언리얼에서 자주 보이는 패턴. 보통 글로벌 변수를 선언하고 싶은 경우 두 가지 옵션이 있음
	  2-1. .h 파일에 extern 키워드로 글로벌 변수 선언.
	  2-2. static 함수 정의 후 해당 함수 안에 static 변수 선언. (대표적 예시로 Singleton 패턴)
	*/

	const TCHAR* CommandLineContent = FCommandLine::Get();
	static bool bLogAssetLoads = FParse::Param(CommandLineContent, TEXT("LogAssetLoads"));

	return bLogAssetLoads;
}

UObject* ULyraAssetManager::SynchronousLoadAsset(const FSoftObjectPath& InAssetObjectPath)
{
	// 해당 함수를 따로 만든 이유는 애셋의 동기 로딩 함수 호출이 불필요한 곳에서 이뤄지고 있지는 않은지 확인하기 위함.
	// 동기 로딩 함수를 호출하게 되면 히칭 현상(프레임 드랍)이 발생 할 수 있음.
	// 즉, 게임플레이 중 동기 로딩하는데 너무 오래걸린다면, 비동기 로딩을 한다던가 로딩 레벨에서 동기 로딩을 해야함.

	if (InAssetObjectPath.IsValid())
	{
		// FScopeLogTime을 확인하는 로직.
		TUniquePtr<FScopeLogTime> LogTimePtr;
		if (ShouldLogAssetLoads())
		{
			// 단순히 로깅하면서 초 단위로 로깅 진행.
			// 로딩에 많은 시간이 걸리지는 않는지 감지하기 위한 로직.
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("Synchronous loaded assets: %s"), *InAssetObjectPath.ToString()),
													nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		// 여기서 두 가지 분기 가능
		// 1. AssetManager가 있으면 AssetManager의 StreamableManager를 통해 동기 로딩.
		// 2. FSoftObjectPath를 통해 바로 동기 로딩.
		// if (UAssetManager::IsValid())
		// 'UAssetManager::IsValid': AssetManager is now always constructed during UEngine::InitializeObjectReferences. Call IsInitialized instead if you need to check whether it has not yet been initialized. Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile.
		if (UAssetManager::IsInitialized())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(InAssetObjectPath);
		}

		// 만약 AssetManager가 준비되지 않았다면 LoadObject() 함수 활용.
		// TryLoad() 내에는 StaticLoadObject() 함수가 호출됨을 알 수 있음.
		// 참고) 항상 StaticLoadObject() 함수가 호출되기 전에 StaticFindObject() 함수를 통해 확인하고 실패하면 로딩함.
		//   언리얼에서는 어떤 UObject를 로드 혹은 찾기를 하고 싶다면 
		//   해당 UObject의 오브젝트 패스를 가지고 StaticLoadObject() 혹은 StaticFindObject()가 호출되게 되어 있음.
		return InAssetObjectPath.TryLoad();
	}

	return nullptr;
}

void ULyraAssetManager::AddLoadedAsset(const UObject* InLoadedAsset)
{
	if (ensureAlways(InLoadedAsset))
	{
		FScopeLock LoadedAssetsLock(&LoadedAssetsCritical);
		LoadedAssets.Add(InLoadedAsset);
	}
}
