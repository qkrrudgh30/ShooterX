// LyraExperienceManagerComponent.cpp

#include "LyraExperienceManagerComponent.h"
#include "LyraExperienceDefinition.h"

ULyraExperienceManagerComponent::ULyraExperienceManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
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
