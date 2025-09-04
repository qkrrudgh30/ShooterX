// LyraGameInstance.cpp


#include "System/LyraGameInstance.h"
#include "LyraGameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"

ULyraGameInstance::ULyraGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ULyraGameInstance::Init()
{
	Super::Init();

	// 앞서 정의한 InitState의 GameplayTag 등록
	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);
	// UGameFrameworkComponentManager는 말그대로 컴포넌트를 관리해주는 클래스.
	// 사실 GameFrameworkComponent <- GameStateComponent <- ExperienceManagerComponent의 구조를 가짐.
	// GameFrameworkComponentManager는 GameFrameworkComponent들의 부착을 자유자재로 컨트롤 해줌.
	// 특히, 게임의 현재 상태에 따라 컴포넌트를 탈부착 해줄수도 있음.
	// 더 중요한 것은 탈부착의 순서도 보장해줌. 그래서 우리는 게임의 상태 순서에 따라 컴포넌트를 탈부착할 예정.
	ensure(ComponentManager);
	if (IsValid(ComponentManager) == true)
	{
		const FLyraGameplayTags& GameplayTags = FLyraGameplayTags::Get();

		ComponentManager->RegisterInitState(GameplayTags.InitState_Spawned, false, FGameplayTag());
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataAvailable, false, GameplayTags.InitState_Spawned);
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataInitialized, false, GameplayTags.InitState_DataAvailable);
		ComponentManager->RegisterInitState(GameplayTags.InitState_GameplayReady, false, GameplayTags.InitState_DataInitialized);

		// 언리얼 공식 문서에 InitState 초기화 관련 로직은 GameInstance::Init()에서 하라는 권장 내용이 있음.
		// GameFeature의 초기화 스테이트는 GameplayTag로만 구현 가능함.
		
		// 이렇게 등록하게 되면, IGameFrameworkInitStateInterface 인터페이스를 상속 받은 클래스들은
		// 등록된 InitState- 태그 단계 별로 적절한 로직이 연쇄적으로 수행됨.
	}
}

void ULyraGameInstance::Shutdown()
{
	Super::Shutdown();
}
