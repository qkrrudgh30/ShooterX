// LyraPlayerState.h

#pragma once

#include "GameFramework/PlayerState.h"
#include "LyraPlayerState.generated.h"

class ULyraExperienceDefinition;
class ULyraPawnData;

/**
 * 
 */
UCLASS()
class SHOOTERX_API ALyraPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ALyraPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostInitializeComponents() final;

	void OnExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience);

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	void SetPawnData(const ULyraPawnData* InPawnData);

public:
	UPROPERTY()
	TObjectPtr<const ULyraPawnData> PawnData;
	// OnExperienceLoaded() 함수가 호출된다면, 익스피리언스가 로드되었단 뜻이고 익스피리언스 안에 폰데이터가 있을텐데
	// 굳이 PlayerState에서 또 캐싱하는 이유가 뭔지 궁금할 수 있음.
	// 1. 안정성 검사. 익스피리언스 로드가 된거지, 로딩된 익스피리언스 안의 데이터는 유효한지 알 수 없음. 이걸 SetPawnData()에서 체크함.
	// 2. 진입점 통일. 로딩된 익스피리언스에서 폰데이터를 가져오는게 아니라, 게터를 따로 둬서 한 곳에서만 폰데이터를 가져가게끔 하기 위함.
	//		또한 폰데이터의 수정은 플레이어 스테이트의 폰 데이터 속성만 내부적으로 수정하게끔 함. 로딩된 익스피리언스의 폰데이터를 수정하지 않음.
	//		그러니 폰데이터는 오로지 플레이어 스테이트의 게터/세터를 통해야만 함.
	
};
