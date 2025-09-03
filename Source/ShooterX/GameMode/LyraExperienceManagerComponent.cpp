// LyraExperienceManagerComponent.cpp

#include "LyraExperienceManagerComponent.h"
#include "Net/UnrealNetwork.h"
#include "LyraExperienceDefinition.h"
#include "System/LyraAssetManager.h"
#include "GameFeaturesSubsystemSettings.h"

ULyraExperienceManagerComponent::ULyraExperienceManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void ULyraExperienceManagerComponent::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentExperience);
}

void ULyraExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnLyraExperienceLoaded::FDelegate&& Delegate)
{
	// CallOrRegister_OnExperienceLoaded() 함수를 호출하는 시점에 이미 익스피리언스가 로드되었다면 곧바로 실행.
	// 로드되지 않은 경우에만 델리게이트 추가.

	if (true == IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	else
	{
		OnExperienceLoaded.Add(MoveTemp(Delegate));
		// Delegate 사용시에 Lambda Capture를 아래와 같이 사용하게 됨.
		// 만약 Lambda Capture하는 변수가 상당히 클 수 있음. 그 변수는 다시 복사 비용을 일으킴.
		// 즉, 아래 예제에서 Arr은 Delegate_Type 내부에 new로 할당되게 되고 복사가 일어남.
		// 복사 비용을 낮추기 위해 Move()를 활용하는 것을 잊지 말자.
		// TArray<int> Arr = {1, 2, 3, 4};
		// Delegate_Type Delegate = [Arr]() -> int32
		// {
		//     return Arr.Num();
		// }
	}
}

void ULyraExperienceManagerComponent::OnExperienceFullLoadComplete()
{
	check(ELyraExperienceLoadState::Loaded != LoadState);

	LoadState = ELyraExperienceLoadState::Loaded;
	
	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();
}

void ULyraExperienceManagerComponent::OnExperienceLoadComplete()
{
	// StartExperienceLoad() 함수의 마지막 FrameNumber와 아래 FrameNumber를 비교해보자.
	// - 로딩하는 시점과 완료 호출 시점이 동일한 프레임인 것을 알 수 있음.
	static int32 OnExperienceLoadComplete_FrameNumber = GFrameNumber;

	// 라이라 원본의 경우에는 이 함수에서 많은 것들을 로드함.

	// 해당 함수가 불리는 것은 앞서 보았던 StreamableDelegateDelayHelper에 의해 불림.
	OnExperienceFullLoadComplete();
}

void ULyraExperienceManagerComponent::StartExperienceLoad()
{
	check(true == ::IsValid(CurrentExperience));
	check(ELyraExperienceLoadState::Unloaded == LoadState);

	LoadState = ELyraExperienceLoadState::Loading;

	ULyraAssetManager& AssetManager = ULyraAssetManager::Get();

	TSet<FPrimaryAssetId> BundleAssetList;

	
	BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());
		// SetCurrentExperience() 함수에 ExperienceId를 넘겨주었는데, 이제서야 CDO를 활용하여 GetPrimaryAssetId를 로딩할 대상으로 등록.
		// 왜 CDO를 가지고 SetCurrentExperience()에서 한 번에 로딩하지 않고 이렇게 하는 걸까?
		// GetPrimaryAssetId() 함수의 동작 원리 때문. GetPrimaryAssetId() 함수를 좀 더 살펴보자. 
		// 1. CDO를 가져와서, GetPrimaryAssetId() 함수를 호출한 이유.
		//   -> GetPrimaryAssetId() 내부에서 CDO가 아니면 해당 애셋의 이름 그자체를 가지고 ID를 얻어서 반환해버림.
		//   -> 지금 상황에서는 "B_DefaultExperience"라는 이름을 가지고 ID를 얻어서 반환하는 격.
		//   -> 근데 프로젝트 세팅 > 애셋 매니저 > 스캔목록에 B_DefaultExperience 애셋은 안 넣어둠.
		// 2. B_DefaultExperience를 Data Asset이 아니라 BP Class로 만든 이유.
		//   -> BP Class여야 GetPrimaryAssetId() 내부에서 한 단계 위 부모의 Native C++ 클래스를 반환하기 때문.
		//   -> 지금 상황에서는 B_DefaultExperience가 BP Class이고, 그때문에 한 단계 위 부모의 Native C++ 클래스 LyraExperienceDefinition이 반환됨.
		//   -> 동시에 우리가 프로젝트 세팅 > 애셋 매니저 > 스캔목록에 등록한 것도 LyraExperienceDefinition임. 아다리가 딱 맞음.

	// load assets associated with the experience
	// 아래는 우리가 후일 GameFeature를 사용하여, Experience에 바인딩된 GameFeature Plugin을 로딩할 Bundle 이름을 추가함.
	// - Bundle이라는게 후일 우리가 로딩할 애셋의 카테고리 이름이라고 생각하면 됨. 후일 다시 다룰 예정.
	TArray<FName> BundlesToLoad;
	{
		// 여기서 주목해야 할 부분은 OwnerNetMode가 NM_StandAlone이라면? Client/Server 둘 다 로딩에 추가됨.
		const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
		bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
		bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);
		if (true == bLoadClient)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
		}
		if (true == bLoadServer)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
		}
	}

	FStreamableDelegate OnAssetLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);

	// 아래도 후일 Bundle을 우리가 GaemFeature에 연동하면서 더 깊게 알아보기로 하고, 지금은 앞서 B_DefaultExperience를 로딩해주는 함수로 생각하자.
	TSharedPtr<FStreamableHandle> Handle = AssetManager.ChangeBundleStateForPrimaryAssets(
		BundleAssetList.Array(),
		BundlesToLoad,
		{}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority
	);
		// ChangeBundleStateForPrimaryAssets() 함수는 비동기 로딩 함수. 번들(카테고리)에 해당하는 애셋만 로딩할 수 있게 도와주는 함수.
		// - ex. ExperienceDefinition이 "계절별 이벤트 데이터"라고 가정한다면, 모든 데이터를 로드하는게 아니라 "겨울" 카테고리에 해당하는 데이터만 로드하고자 하는 것.
		// - 이 코드 기준으론 "클라이언트", "서버"에 해당하는 애셋들이 로딩되는 셈.

	if (false == Handle.IsValid() || true == Handle->HasLoadCompleted())
	{
		// 로딩이 완료되었으면, ExecuteDelegate를 통해 OnAssetsLoadedDelegate를 호출.
		// - 잠시 아래 델리게이트 클래스를 살펴보자.
		// - 조건이 충족되면 즉시 해당 델리게이트에 바인드된 함수가 호출되는게 아님.
		// - 조건이 충족되면 그 다음 월드 틱에서 바인드된 함수들을 모두 모아서 실행하여 성능상의 이점을 챙김.
		FStreamableHandle::ExecuteDelegate(OnAssetLoadedDelegate);
	}
	else
	{
		Handle->BindCompleteDelegate(OnAssetLoadedDelegate);
		Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetLoadedDelegate]() -> void {
			OnAssetLoadedDelegate.ExecuteIfBound();
		}));
			// 비동기 로딩이기 때문에 로딩과정에서 실패할 수도 있음. 그에대한 예외처리.
			// 델리게이트가 삭제되었을 수도 있음. 
	}

	// 아래에서 OnExperienceLoadComplete() 함수와 OnExperienceFullLoadComplete() 함수를 정의 후 프레임 번호 디버깅 할 예정.
	static int32 StartExperienceLoad_FrameNumber = GFrameNumber;
}

void ULyraExperienceManagerComponent::SetCurrentExperience(FPrimaryAssetId ExperienceId)
{
	ULyraAssetManager& AssetManager = ULyraAssetManager::Get();

	TSubclassOf<ULyraExperienceDefinition> ExperienceDefinitionClassToLoad;
	{
		FSoftObjectPath ObjectPathToLoad = AssetManager.GetPrimaryAssetPath(ExperienceId);
		ExperienceDefinitionClassToLoad = Cast<UClass>(ObjectPathToLoad.TryLoad());
			// 동기 로딩.
	}

	const ULyraExperienceDefinition* ExperienceCDO = GetDefault<ULyraExperienceDefinition>(ExperienceDefinitionClassToLoad);
	check(IsValid(ExperienceCDO) == true);
	
	check(nullptr == CurrentExperience);
	{
		// ExperienceCDO가 nullptr이라면,
		// 애셋매니저가 ExperienceId에 해당하는 프라이머리 애셋(ExperienceDefinition 애셋)을 못찾았다는 뜻.
		// 이는 ProjectSettings > Asset Manager에서 스캔에 잘못된 철자를 작성한 것이 원인일수도.
		
		CurrentExperience = ExperienceCDO;
			// 그리고 CDO로 CurrentExperience를 설정함.
			// 어떤 의도로 이렇게 코드를 작성한지는 코드를 쭉 읽어보고(StartExperienceLoad() 함수 까지 읽어봐야함) 다시 생각해보자.
	}

	StartExperienceLoad();
}

void ULyraExperienceManagerComponent::OnRep_CurrentExperience()
{
	StartExperienceLoad();
}
