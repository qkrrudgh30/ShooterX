// GFA_WorldActionBase.cpp

#include "GameFeature/GFA_WorldActionBase.h"

void UGFA_WorldActionBase::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	// 월드를 순회하면서
	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		// 앞서 ExperienceManagerComponent에서 GameFeatureAction을 활성화하면서 Context에 World를 넣어줌.
		// - 이를 통해 적용할 대상인지 판단
		// - 플레이 버튼을 누르는 순간 월드가 일단 하나 추가됨. 인게임 에디터가 돌아가고 있는 월드가 따로 있다???? 어쨋든 복수개의 월드를 만들수 있음. 보통은 1개.
		if (true == Context.ShouldApplyToWorldContext(WorldContext))
		{
			// WorldActionBase의 Interface인 AddToWorld 호출
			AddToWorld(WorldContext, Context);
		}
	}
}
